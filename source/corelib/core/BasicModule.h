/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICMODULE__H__
#define __BASICMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#if defined (_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4267)
#endif /* _MSC_VER */

#include "corelib/corelibdef.h"

#include <ace/Timer_Heap.h>
#include <ace/Timer_Queue_Adapters.h>

#include "corelib/core/BasicService.h"
#include "corelib/core/MutexAllocator.h"
#include "corelib/core/Module_Proactor.h"
#include "corelib/core/BasicCommand.h"
#include "corelib/core/TimerContainer.h"
#include "corelib/db/Asynch_DBPool.h"
#include "corelib/core/TLSContext.h"

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

#include <list>

//服务注册回调
typedef boost::function< void( BasicService*, bool&)>	SERVICE_REGIST_FUN_MAP;
//服务器断线回调
typedef boost::function< void( BasicService*)> SERVICE_DISCONN_FUN_MAP;


/**
* @class BasicModule
*
* @brief 功能模块的基础类
**/
class BasicModule
{
	friend class BasicService;
	friend class Asynch_DBPools;
public:
	typedef NS_STL::list<BasicCommand* > COMMAND_LIST;
	typedef NS_STL::list<BasicCommand* >::iterator COMMAND_LIST_ITER;

public:
	explicit BasicModule( MutexAllocator *allocator_, bool delete_allocator);
	virtual ~BasicModule(void);

	/**
	* 初始化函数
	* @param argc
	* @param argv
	* @return
	**/
	int init (int argc, ACE_TCHAR *argv[]);

	/**
	* 模块初始化准备
	*
	* @param argc
	* @param argv
	* @return
	**/
	virtual int before_module_init( int argc, ACE_TCHAR *argv[]) =0;

	/**
	* 开始线程池的循环之前
	* @return
	**/
	virtual int before_proactor_thread() =0;

	/**
	* 注册定时器
	**/
	virtual void register_timer(){}

	/**
	* 主事件循环
	* @return
	**/
	virtual int main_loop() =0;

	/**
	* 释放资源函数
	**/
	virtual void fini();

	/**
	* 设置数据库连接池属性
	**/
	virtual void set_dbpoolprop(){}

	/**
	* 开始一个服务注册,用来关联socket和处理入口
	* @param s
	**/
	void begin_registservice( BasicService* s);

	/**
	* 结束一个服务注册
	* @param s
	**/
	void fin_registservice( BasicService* s);

	/**
	* 取消服务注册
	* @param s
	**/
	void undo_registservice( BasicService* s);

	/**
	* 服务断线
	* @param s
	**/
	void disconnect_service( BasicService* s);

	void set_serviceregistfun( SERVICE_REGIST_FUN_MAP f);
	void set_servicedisconnfun( SERVICE_DISCONN_FUN_MAP f);

	/**
	* 释放等待注册的服务
	**/
	void release_waitregistservice();

	/**
	* 退出模块
	* @param void*
	**/
	virtual void exit_module( void*);

	void set_mainloop( int ll){
		if( ll > 0 && ll < 8000)
			main_loop_ =ll;
	}

	static BasicModule* base_instance(){ return base_instance_;}

#ifdef CORELIB_WIN32
	ACE_Proactor* proactor(){
		return (ACE_Proactor*)proactor_;
	}

#else
	//used reactor handler
	ACE_Reactor* reactor(){
		return (ACE_Reactor*)proactor_;
	}

#endif

	//------------------------------------------主线程定时器------------------------------------------
	void moduletimer_reset(){
		module_timer_.reset();
	}

	/**
	* 增加一个定时器
	* @param step
	* @param f
	* @return int 定时器句柄
	**/
	int add_moduletimer( int step, MODULETIMER_FUN_MAP f){
		return module_timer_.add_timer( step, f);
	}

	/**
	* 清除一个定时器
	* @param tid
	**/
	void del_moduletimer( int tid){
		module_timer_.del_timer( tid);
	}

	/**
	* 清除所有的定时器
	**/
	void delall_moduletimer(){
		module_timer_.delall_timer();
	}

	//------------------------------------------多线程定时器------------------------------------------
	//timer function
	/**
	* 增加一个定时器
	* @param handler
	* @param act
	* @param sec
	* @param usec
	* @return
	**/
	long schedule( ACE_Event_Handler *handler, const void *act, time_t sec, suseconds_t usec);

	/**
	* 取消一个定时器
	* @param timer_id
	* @param act
	* @return
	**/
	int cancel (long timer_id, const void **act = 0);

protected:
	void set_cpu_num( int n){
		cpu_num_ =n;
	};

protected:
	//proactor
	int					cpu_num_;
	int					main_loop_;

	//proactor/reactor共用该类
	Module_Proactor		proactor_;

	//iocp锁分派
	MutexAllocator*		mutex_allocator_;
	bool				delete_mutex_allocator_;

	//主线程定时器
	ACE_Thread_Timer_Queue_Adapter< ACE_Timer_Heap>	timer_;

	//退出模块标志
	bool				exit_module_;

	//应用锁
	ACE_Thread_Mutex	module_mutex_;

	//监听连接逻辑
	NS_STL::list<BasicService*>	waiting_registservice_;
	SERVICE_REGIST_FUN_MAP		waiting_registfun_;
	SERVICE_DISCONN_FUN_MAP		disconn_fun_;

	//数据库线程池
	Asynch_DBPools	dbpool_;
	bool			support_dbpool_;

	//主线程定时器
	TimerContainer	module_timer_;

	//线程变量
	TLSContext		tls_context_;

	//--------------------------------消息队列相关--------------------------//
public:

	/**
	* 注册系统命令
	* @param p
	**/
	void regist_syscmd( BasicCommand* p);
	void regist_netcmd( BasicCommand *p);
	void regist_usercmd( BasicCommand *p);

	BasicCommand* pop_one_cmd();

	void append_returntask( BasicPoolContext *p){
		dbpool_.append_returntask( p);
	}
	void append_noreturntask( BasicPoolContext *p){
		dbpool_.append_noreturntask( p);
	}
	void append_returntask_sort( int id, BasicPoolContext *p){
		dbpool_.append_returntask_sort( id, p);
	}
	void append_noreturntask_sort( int id, BasicPoolContext *p){
		dbpool_.append_noreturntask_sort( id, p);
	}

protected:

	//释放未处理的命令队列
	void release_cmds();

protected:
	//1级队列
	//系统命令队列
	COMMAND_LIST	system_cmds_1_;
	//网络命令队列
	COMMAND_LIST	net_cmds_1_;
	//玩家命令队列
	COMMAND_LIST	user_cmds_1_;
	//2级队列
	//系统命令队列
	COMMAND_LIST	system_cmds_2_;
	//网络命令队列
	COMMAND_LIST	net_cmds_2_;
	//玩家命令队列
	COMMAND_LIST	user_cmds_2_;

	//实例，需要在实现类中初始化
	static BasicModule*	base_instance_;
};

#endif //__BASICMODULE__H__
