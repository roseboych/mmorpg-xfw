/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/db/Asynch_DBPool.h"

#include "ace/OS_NS_unistd.h"
#include "ace/Guard_T.h"
#include "ace/Auto_Ptr.h"

#include "corelib/log/logmacro.h"
#include "corelib/core/BasicModule.h"

Asynch_DBPools::Asynch_DBPools( int num):
inherit(),
thread_poolnum_( num),
initialized_( false),
will_exit_( false),
err_fun_( 0),
db_loop_( 20)
{
}

Asynch_DBPools::~Asynch_DBPools(void)
{
	release();
}

void Asynch_DBPools::set_dbinfo( const char* dbname, const char* dbuser, const char* dbpwd)
{
	this->dbname_ =dbname;
	this->dbuser_ =dbuser;
	this->dbpwd_ =dbpwd;
}

int Asynch_DBPools::start_pools()
{
	if( initialized_) return 0;

	MODULE_LOG_WARN( MODULE_BOOT, ">>>>begin connect to database...");

	//initialize ODBC environment,and set multithread mode
	otl_connect::otl_initialize( 1);

	//初始化数据库连接
	connects_.reset( DATABASE_NEW otl_connect[thread_poolnum_]);
	//set unsignal
	events_.signal();

	for( int ss =0; ss < thread_poolnum_; ++ss)
	{
		if( !connect_db( &(connects_[ss])))
			return 1;
		unused_connects_.push_back( &( connects_[ss]));
	}

	MODULE_LOG_WARN( MODULE_BOOT, ">>>>end connect to database[pool:%d]...", thread_poolnum_);

	//创建线程池
	this->activate( THR_NEW_LWP|THR_JOINABLE, thread_poolnum_, 1);

	initialized_ =true;

	return 0;
}

void Asynch_DBPools::end_pools()
{
	//设置退出标志
	will_exit_ =true;

	this->append_task( TASKCMD_NEW ExitPoolCommand());

	//插入退出命令
	this->wait();

	initialized_ =false;
}

void Asynch_DBPools::release()
{
	if( initialized_)
		end_pools();

	task_maps_.clear();
	CONTEXT_LIST_ITER iter =task_items_.begin();
	for( ; iter != task_items_.end(); ++iter)
		delete (*iter);
	task_items_.clear();

	iter =task_items_2_.begin();
	for( ; iter != task_items_2_.end(); ++iter)
		delete (*iter);
	task_items_2_.clear();

	for( int ss =0; ss < thread_poolnum_; ++ss)
	{
		try{
			if( connects_[ss].connected)
				connects_[ss].logoff();
		}
		catch( otl_exception& oe){
		}
	}

	initialized_ =false;
}

bool Asynch_DBPools::get_nothreadsafe_conn( otl_connect* p)
{
	if( p == 0)
		return false;

	return connect_db( p);
}

bool Asynch_DBPools::connect_db( otl_connect* p)
{
	try
	{
		char strDNS[128] ={0,};
		ACE_OS::sprintf( strDNS, "%s/%s@%s", dbuser_.c_str(), dbpwd_.c_str(), dbname_.c_str());
		p->rlogon( strDNS);

		return true;
	}
	catch( otl_exception& e){
		MODULE_LOG_WARN( MODULE_BOOT, "connect to database failed!");
		return false;
	}
}

void Asynch_DBPools::append_task_sort( int id, BasicPoolContext *p)
{
	if( will_exit_ && p->get_type() != EXITPOOL_CMD)
	{
		delete p;
		return;
	}

	p->set_mapid( id);

	ACE_Guard<ACE_Thread_Mutex> mon( mutex_);
	task_items_2_.push_back( p);

	if( task_items_2_.size() > 0)
		events_.signal();
}

void Asynch_DBPools::append_task( BasicPoolContext *p)
{
	if( will_exit_ && p->get_type() != EXITPOOL_CMD)
	{
		delete p;
		return;
	}

	ACE_Guard<ACE_Thread_Mutex> mon( mutex_);
	task_items_2_.push_back( p);

	if( task_items_2_.size() > 0)
		events_.signal();
}

int Asynch_DBPools::svc(void)
{
	//获取和线程关联的数据库连接
	otl_connect *pconn =0;
	{
		if( unused_connects_.size() == 0)
			return 1;
		ACE_Guard<ACE_Thread_Mutex> guard( svc_mutex_);
		if( unused_connects_.size() == 0)
			return 1;
		pconn =unused_connects_.front();
		unused_connects_.pop_front();
	}

	if( pconn == 0) return 1;

	MODULE_LOG_DEBUG( MODULE_TEMP, "start dbpool thread = %t....");

	//连接断线次数
	int disconnect_times =0;

	ACE_Time_Value sv( 0, 1000);
	ACE_Time_Value dbslp( 2, 0);
	int sleep_count =0;
	while( 1)
	{
		if( sleep_count >= db_loop_)
		{
			ACE_OS::sleep( sv);
			sleep_count =0;
		}
		++sleep_count;

		//判断数据库连接是否断开
		if( !(pconn->connected))
		{
			++ disconnect_times;
			//重新连接
			if( !connect_db( pconn))
			{
				//如果失败线程sleep 2秒钟
				ACE_OS::sleep( dbslp);
				continue;
			}
			else
				disconnect_times =0;
		}

		//数据库超过5次断线关闭程序
		if( disconnect_times == 5)
		{
			//TODO:数据库错误
			//TODO:是否要保存未处理的数据库操作
			if( err_fun_)
				err_fun_();

			return 2;
		}

		BasicPoolContext *pcmd =pop_one_task();
		if( pcmd == 0)
			continue;

		if( pcmd->get_type() == EXITPOOL_CMD)
		{
			//结束一个命令
			finish_one_task( pcmd, true);
			events_.signal();
			break;
		}
		else
		{
			pcmd->run_it( pconn);
			if( pcmd->return_it_)
			{
				finish_one_task( pcmd, false);
				BasicCommand *pcmd2 =dynamic_cast<BasicCommand*>(pcmd);

				BasicModule::base_instance_->regist_netcmd( pcmd2);
			}
			else
				finish_one_task( pcmd, true);
		}
	}

	MODULE_LOG_DEBUG( MODULE_TEMP, "end dbpool thread = %t....");

	return 0;
}

BasicPoolContext* Asynch_DBPools::pop_one_task()
{
	events_.wait();

	ACE_Guard<ACE_Thread_Mutex> guard( svc_mutex_);

	if( task_items_.size() == 0)
	{
		//移动处理的命令
		from_2_to_1();

		//依然没有需要处理的
		if( task_items_.size() == 0)
		{
			events_.reset();
			return 0;
		}
	}

	BasicPoolContext *ret =0;
	CONTEXT_LIST_ITER iter =task_items_.begin();
	for( ; iter != task_items_.end(); ++iter)
	{
		BasicPoolContext *r =(*iter);
		//支持队列的时序性
		if( r->get_pre() == 0 && r->not_used())
		{
			ret =r;
			break;
		}
	}

	if( ret == 0)
		return 0;

	//必须在所有的数据库操作完成后才能取出结束命令
	if( ret->get_type() == EXITPOOL_CMD && task_items_.size() > 1)
		return 0;

	ret->set_used( true);

	return ret;
}

void Asynch_DBPools::from_2_to_1()
{
	if( task_items_2_.size() == 0)
		return;

	ACE_Guard<ACE_Thread_Mutex> mon( mutex_);

	CONTEXT_LIST_ITER iter =task_items_2_.begin();
	int iid =NO_INITVALUE;
	for( ; iter != task_items_2_.end(); ++iter)
	{
		BasicPoolContext *r =(*iter);
		iid =r->get_mapid();
		if( iid == NO_INITVALUE)
		{
			//不需要时序支持
			task_items_.push_back( r);
		}
		else
		{
			task_items_.push_back( r);

			//需要时序支持的
			CONTEXT_MAP_ITER fiter =task_maps_.find( iid);
			if( fiter == task_maps_.end())
			{
				task_maps_[iid] =r;
			}
			else
			{
				BasicPoolContext *pitem =fiter->second;
				while( pitem->get_next())
					pitem =pitem->get_next();

				pitem->set_next( r);
				r->set_pre( pitem);
			}
		}
	}

	task_items_2_.clear();

	if( task_items_.size() == 1)
		events_.signal();
}

void Asynch_DBPools::finish_one_task( BasicPoolContext *p, bool del)
{
	int id =p->get_mapid();
	if( id == NO_INITVALUE)
	{
		if( p->get_type() == EXITPOOL_CMD)
		{
			p->set_used( false);
			return;
		}

		ACE_Guard<ACE_Thread_Mutex> mon( svc_mutex_);

		CONTEXT_LIST_ITER fiter =NS_STL::find( task_items_.begin(), task_items_.end(), p);
		if( fiter != task_items_.end())
			task_items_.erase( fiter);

		if( del)
			delete p;

		return;
	}
	else
	{
		ACE_Guard<ACE_Thread_Mutex> mon( svc_mutex_);

		CONTEXT_LIST_ITER fiter =NS_STL::find( task_items_.begin(), task_items_.end(), p);
		if( fiter != task_items_.end())
			task_items_.erase( fiter);

		CONTEXT_MAP_ITER f2 =task_maps_.find( id);
		if( f2 != task_maps_.end())
		{
			task_maps_.erase( f2);

			BasicPoolContext *phead =p->get_next();
			if( phead)
			{
				phead->set_pre( 0);
				task_maps_[id] =phead;
			}
		}

		p->set_pre( 0);
		p->set_next( 0);
		if( del)
			delete p;
	}
}
