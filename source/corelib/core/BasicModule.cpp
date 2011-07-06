/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/core/BasicModule.h"

#include <ace/Log_Msg.h>
#include <ace/OS_NS_unistd.h>
#include <ace/Proactor.h>

#include <reslib/deploy/GlobalConfig.h>

#include "corelib/log/logmacro.h"
#include "corelib/net/IOCPMutexAllocator.h"
#include "corelib/script/ScriptContext.h"

BasicModule::BasicModule( MutexAllocator *allocator_, bool delete_allocator)
:mutex_allocator_( allocator_),
delete_mutex_allocator_( delete_allocator),
cpu_num_( 1),
exit_module_( false),
main_loop_( 100),
waiting_registfun_(0),
disconn_fun_( 0),
support_dbpool_( false)
{
	if( mutex_allocator_ == NULL)
	{
		mutex_allocator_ =IOCP_MutexApplocator;
		delete_allocator =true;
	}
}

BasicModule::~BasicModule(void)
{
	//删除mutex分派器
	if( delete_mutex_allocator_)
		delete mutex_allocator_;
}

int BasicModule::init (int argc, ACE_TCHAR *argv[])
{
	//初始化随机数
	ACE_OS::srand( (u_int)ACE_OS::gettimeofday().sec());

	if( !TLSContext::init_tls_support())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "initialize tls support failed!");
		return -1;
	}

	//加载配置文件
	if( !GLOBALCONFIG_INS->init_config())
	{
		MODULE_LOG_WARN( MODULE_BOOT, "load global config file failed!");
		return -1;
	}

	//set iocp thread numbers
	set_cpu_num( GLOBALCONFIG_INS->get_cpunum());
	set_mainloop( GLOBALCONFIG_INS->get_loopnum());

	//对主线程tls进行初始化
	if( !tls_context_.init_tlscontext())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "initialize main thread tls failed!");
	}

	//完成模块初始化
	if( before_module_init( argc, argv) != 0)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "before_module_init call failed");
		return -1;
	}

	//初始化proactor框架
	proactor_.set_thread_num( cpu_num_);

#ifdef CORELIB_WIN32
	proactor_.initialize();
#endif

	if( before_proactor_thread() != 0)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "proactor before_proactor_thread call failed");
		return -4;
	}

	//启动proactor的处理线程
	proactor_.start();

	//如果支持数据库，则启动它
	this->set_dbpoolprop();
	if( support_dbpool_ && dbpool_.start_pools() != 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "start database pool failed!................");
		return -1;
	}

	return 0;
}

void BasicModule::fini()
{
	if( support_dbpool_)
		dbpool_.wait();

	release_cmds();

	tls_context_.uninit_tlscontext();

	TLSContext::uninit_tls_support();
}

long BasicModule::schedule( ACE_Event_Handler *handler, const void *act, 
									time_t sec, suseconds_t usec)
{
	ACE_Time_Value interval( sec, usec);
	return timer_.schedule( handler, act, ACE_OS::gettimeofday()+ACE_Time_Value(sec, usec), 
		interval);
}

int BasicModule::cancel (long timer_id, const void **act)
{
	return timer_.cancel( timer_id, act);
}

void BasicModule::exit_module( void*)
{
	exit_module_ =true;

	//exit proactor
	proactor_.stop();

	//exit dbpool
	if( support_dbpool_)
		dbpool_.end_pools();
}

//--------------------------------消息队列相关--------------------------//
void BasicModule::release_cmds()
{
	ACE_Guard<ACE_Thread_Mutex> guard( module_mutex_);

	for( COMMAND_LIST_ITER iter =system_cmds_1_.begin(); iter != system_cmds_1_.end(); ++iter)
		delete (*iter);
	system_cmds_1_.clear();

	for( COMMAND_LIST_ITER iter2 =net_cmds_1_.begin(); iter2 != net_cmds_1_.end(); ++iter2)
		delete (*iter2);
	net_cmds_1_.clear();

	for( COMMAND_LIST_ITER iter3 =user_cmds_1_.begin(); iter3 != user_cmds_1_.end(); ++iter3)
		delete (*iter3);
	user_cmds_1_.clear();

	for( COMMAND_LIST_ITER iter =system_cmds_2_.begin(); iter != system_cmds_2_.end(); ++iter)
		delete (*iter);
	system_cmds_2_.clear();

	for( COMMAND_LIST_ITER iter2 =net_cmds_2_.begin(); iter2 != net_cmds_2_.end(); ++iter2)
		delete (*iter2);
	net_cmds_2_.clear();

	for( COMMAND_LIST_ITER iter3 =user_cmds_2_.begin(); iter3 != user_cmds_2_.end(); ++iter3)
		delete (*iter3);
	user_cmds_2_.clear();
}

BasicCommand* BasicModule::pop_one_cmd()
{
	if( exit_module_) return 0;

	BasicCommand* pCmd =0;

	if( system_cmds_1_.size() > 0)
	{
		pCmd =system_cmds_1_.front();
		system_cmds_1_.pop_front();
	}
	else if( net_cmds_1_.size() > 0)
	{
		pCmd =net_cmds_1_.front();
		net_cmds_1_.pop_front();
	}
	else if( user_cmds_1_.size() > 0)
	{
		pCmd =user_cmds_1_.front();
		user_cmds_1_.pop_front();
	}

	if( system_cmds_1_.size() == 0)
	{
		if( system_cmds_2_.size() > 0)
		{
			ACE_Guard<ACE_Thread_Mutex> guard( module_mutex_);
			NS_STL::copy( system_cmds_2_.begin(), system_cmds_2_.end(), NS_STL::back_inserter( system_cmds_1_));
			system_cmds_2_.clear();
		}
		else if( net_cmds_1_.size() == 0)
		{
			if( net_cmds_2_.size() > 0)
			{
				ACE_Guard<ACE_Thread_Mutex> guard( module_mutex_);
				NS_STL::copy( net_cmds_2_.begin(), net_cmds_2_.end(), NS_STL::back_inserter( net_cmds_1_));
				net_cmds_2_.clear();
			}
			else if( user_cmds_1_.size() == 0)
			{
				if( user_cmds_2_.size() > 0)
				{
					ACE_Guard<ACE_Thread_Mutex> guard( module_mutex_);
					NS_STL::copy( user_cmds_2_.begin(), user_cmds_2_.end(), NS_STL::back_inserter( user_cmds_1_));
					user_cmds_2_.clear();
				}
			}
		}
	}

	return pCmd;
}

void BasicModule::regist_syscmd( BasicCommand* p)
{
	ACE_Auto_Ptr<BasicCommand> p_p( p);

	if( exit_module_) return;

	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);
	system_cmds_2_.push_back( p_p.release());
}

void BasicModule::regist_netcmd( BasicCommand *p)
{
	ACE_Auto_Ptr<BasicCommand> p_p( p);

	if( exit_module_) return;

	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	net_cmds_2_.push_back( p_p.release());
}

void BasicModule::regist_usercmd( BasicCommand *p)
{
	ACE_Auto_Ptr<BasicCommand> p_p( p);

	if( exit_module_) return;

	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	user_cmds_2_.push_back( p_p.release());
}

//----------------------------------------iocp相关---------------------------------------------

void BasicModule::begin_registservice( BasicService* s)
{
	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	waiting_registservice_.push_back( s);
}

void BasicModule::fin_registservice( BasicService* s)
{
	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	NS_STL::list<BasicService*>::iterator fiter =
		NS_STL::find( waiting_registservice_.begin(), waiting_registservice_.end(), s);

	if( fiter == waiting_registservice_.end())
		return;

	waiting_registservice_.erase( fiter);

	bool autorelease =true;

	//如果注册成功autorelease设置为false，不释放BasicService
	if( waiting_registfun_)
		waiting_registfun_( s, autorelease);

	if( autorelease && s)
		delete s;
}

void BasicModule::undo_registservice( BasicService* s)
{
	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	NS_STL::list<BasicService*>::iterator fiter =
		NS_STL::find( waiting_registservice_.begin(), waiting_registservice_.end(), s);

	if( fiter == waiting_registservice_.end())
		return;

	waiting_registservice_.erase( fiter);
}

void BasicModule::disconnect_service( BasicService* s)
{
	ACE_Guard<ACE_Thread_Mutex> mon( module_mutex_);

	ACE_Auto_Ptr<BasicService> p_ps( s);

	NS_STL::list<BasicService*>::iterator fiter =
		NS_STL::find( waiting_registservice_.begin(), waiting_registservice_.end(), s);

	if( fiter != waiting_registservice_.end())
		waiting_registservice_.erase( fiter);
	
	if( disconn_fun_)
		disconn_fun_( s);
}

void BasicModule::release_waitregistservice()
{
	for( NS_STL::list<BasicService*>::iterator iter =waiting_registservice_.begin(); 
		iter != waiting_registservice_.end(); ++iter)
	{
		BasicService *ps =(*iter);

		if( ps)
			delete ps;
	}
	waiting_registservice_.clear();
}

void BasicModule::set_serviceregistfun( SERVICE_REGIST_FUN_MAP f)
{
	this->waiting_registfun_ =f;
}

void BasicModule::set_servicedisconnfun( SERVICE_DISCONN_FUN_MAP f)
{
	this->disconn_fun_ =f;
}
