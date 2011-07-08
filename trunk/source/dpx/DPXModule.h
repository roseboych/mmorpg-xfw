/**
* dpx application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPXMODULE__H__
#define __DPXMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "dpx_pre.h"

#include <ace/Service_Types.h>
#include <ace/svc_export.h>
#include <ace/Asynch_Connector.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <corelib/core/BasicModule.h>
#include <corelib/core/BasicService.h>
#include <corelib/core/BasicCommand.h>
#include <prolib/BasicProtocol.h>
#include <corelib/net/GuardAcceptor.h>

#include "DPXSvr.h"
#include "DPX_Service.h"

#define DPX_GETPLAYER_FROMCACHE( usr, pro)						\
	Player* ##usr =get_player( ##pro->get_uuidglobalindex());	\
	if( ##usr == 0 || !##pro->same_session( ##usr->uuid_))		\
		return

/**
* @class DPXModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class DPXModule : public BasicModule, public DPXSvr
{
	friend class DPX_Service;

public:
	typedef BasicModule inherit;
	typedef DPXSvr inherit2;

public:
	DPXModule(void);
	virtual ~DPXModule(void);

	virtual int before_module_init( int argc, ACE_TCHAR *argv[]);
	virtual int before_proactor_thread();

	virtual int main_loop();

	virtual void fini (void);

	virtual void set_dbpoolprop();

	virtual void exit_module( void*);

	virtual void post_exit_cmd();

	virtual ACE_Thread_Mutex& get_mutex(){
		return module_mutex_;
	}

	/**
	* 数据库错误事件回调
	**/
	void dberror_callback();

	/**
	* dpx服务器定时连接
	* @param t
	**/
	void dpxsvr_connect_timer( ACE_UINT64 t);

private:
	//监听器
	GuardAcceptor<DPX_Service>	accepter_;

	//------------------------Global_Data-----------------------------------//
public:
};

#define DPXMODULE ACE_Singleton<DPXModule, ACE_Null_Mutex>::instance()

#endif //__LGSMODULE__H__
