/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ASYNCH_DBPOOLS__H__
#define __ASYNCH_DBPOOLS__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#ifdef ACE_WIN32
	#pragma comment(lib, "odbc32.lib")
#endif

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

#include "ace/Thread_Mutex.h"
#include "ace/Task.h"
#include "ace/Auto_Ptr.h"
#include "ace/Manual_Event.h"

#include "corelib/corelibdef.h"
#include "corelib/db/BasicPoolContext.h"
#include "corelib/core/BasicCommand.h"

#include <list>
#include <string>
#include <map>

class otl_connect;

//exit command id
#define EXITPOOL_CMD	-1

/**
* @class ExitPoolCommand
* 
* @brief command to exit system
**/
class ExitPoolCommand : public BasicPoolContext
{
public:
	ExitPoolCommand():BasicPoolContext(){};
	virtual ~ExitPoolCommand(){};

	virtual int get_type(){ return EXITPOOL_CMD;};
	virtual void run_it( otl_connect *pconn){};
	virtual bool validate(){ return true;};
};

//callback function when a database error happen
typedef boost::function<void()>	DBERROR_CALLBACK_FUN;

/**
* @class Asynch_DBPools
* 
* @brief database thread pools. command can sort on the userid/roleid
*
**/
class Asynch_DBPools : public ACE_Task_Base
{
	typedef ACE_Task_Base	inherit;
	typedef NS_STL::list<BasicPoolContext*>	CONTEXT_LIST;
	typedef NS_STL::list<BasicPoolContext*>::iterator CONTEXT_LIST_ITER;
	typedef NS_STL::map<int, BasicPoolContext*>	CONTEXT_MAP;
	typedef NS_STL::map<int, BasicPoolContext*>::iterator	CONTEXT_MAP_ITER;

	typedef NS_STL::list<otl_connect*>	UNUSED_CONNECTION;
	typedef NS_STL::list<otl_connect*>::iterator	UNUSED_CONNECTION_ITER;
public:
	Asynch_DBPools( int num =4);
	virtual ~Asynch_DBPools(void);

	/**
	* database thread enterpoint function
	*
	* @return
	**/
	virtual int svc (void);

	/**
	* boost thread pools
	* @return 0:success 1:db failed
	**/
	int start_pools();

	/**
	* end thread pools
	**/
	void end_pools();

	/**
	* set database information which used to connect to database
	* include database name, user name and password
	* @param dbname
	* @param dbuser
	* @param dbpwd
	**/
	void set_dbinfo( const char* dbname, const char* dbuser, const char* dbpwd);

	/**
	* set thread numbers of the thread pool's supported
	* @param n
	**/
	void set_threadnum( int n){
		thread_poolnum_ =n;
	}

	/**
	* set thread step number, all of the threads use the same setting.
	* @param ll
	**/
	void set_dbloop( int ll){
		if( ll > 0 && ll < 8000)
			db_loop_ =ll;
	}

	/**
	* get a database connection, not multithread safe
	* @param p	if return = true, p will return the validate database connection
	* @return 
	**/
	bool get_nothreadsafe_conn( otl_connect* p);

	/**
	* 设置数据库错误回调函数
	* @param f	callback function
	**/
	void set_callfun( DBERROR_CALLBACK_FUN f){
		err_fun_ =f;
	}

	/**
	* 增加一个需要返回的数据库访问命令
	* @param p
	**/
	void append_returntask( BasicPoolContext *p){
		p->return_it_ =true;
		append_task( p);
	}

	/**
	* 增加一个不需要返回的数据库访问命令
	* @param p
	**/
	void append_noreturntask( BasicPoolContext *p){
		p->return_it_ =false;
		append_task( p);
	}

	/**
	* 增加一个需要根据指定id进行排序访问的返回的数据库访问命令
	* @param id	排序标准，通常是角色id
	* @param p
	**/
	void append_returntask_sort( int id, BasicPoolContext *p){
		p->return_it_ =true;

		append_task_sort( id, p);
	}

	/**
	* 增加一个需要根据指定id进行排序访问的不需要返回的数据库访问命令
	* @param id	排序标准，通常是角色id
	* @param p
	**/
	void append_noreturntask_sort( int id, BasicPoolContext *p){
		p->return_it_ =false;

		append_task_sort( id, p);
	}

protected:
	/**
	* 增加一个不需要排序的数据库任务
	* @param p
	**/
	void append_task( BasicPoolContext *p);

	/**
	* 增加一个需要排序的数据库任务
	* @param id
	* @param p
	**/
	void append_task_sort( int id, BasicPoolContext *p);

	/**
	* 弹出一个需要处理的数据库任务
	* @return
	**/
	BasicPoolContext* pop_one_task();

	/**
	* 完成一个数据库任务
	* @param p
	* @param del	是否需要自动释放
	**/
	void finish_one_task( BasicPoolContext *p, bool del =true);

	/**
	* release resource
	**/
	void release();

	/**
	* 连接数据库
	* @param p	操作的连接
	* @return
	**/
	bool connect_db( otl_connect* p);

	/**
	* move database command form 2th queue to 1th queue
	**/
	void from_2_to_1();

protected:
	int db_loop_;
	int	thread_poolnum_;

	//initialize flag of the database thread pool 
	bool initialized_;
	//flag of the exit from threads
	bool will_exit_;

	//任务列表
	CONTEXT_LIST	task_items_;
	CONTEXT_MAP		task_maps_;
	//2级队列
	CONTEXT_LIST	task_items_2_;

	//2th queue's mutex
	ACE_Thread_Mutex	mutex_;
	//database command's mutex
	ACE_Thread_Mutex	svc_mutex_;

	//odbc's configuation
	NS_STL::string dbname_;
	NS_STL::string dbuser_;
	NS_STL::string dbpwd_;

	//all database connect objects
	ACE_Auto_Array_Ptr<otl_connect>	connects_;
	//empty connections
	UNUSED_CONNECTION	unused_connects_;
	//event signal object
	ACE_Manual_Event	events_;

	//callback function when a database error happend
	DBERROR_CALLBACK_FUN	err_fun_;
};


#endif	//__ASYNCH_DBPOOLS__H__
