/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTSMODULE__H__
#define __CTSMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

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

#include "CTSSvr.h"
#include "CTS_Service.h"

#define CTS_GETPLAYER_FROMCACHE( usr, pro)						\
	Player* ##usr =get_player( ##pro->get_uuidglobalindex());	\
	if( ##usr == 0 || ! ##p->same_session( ##usr->uuid_))		\
		return

/**
* @class CTSModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class CTSModule : public BasicModule, public CTSSvr
{
	friend class CTS_Service;

public:
	typedef BasicModule inherit;
	typedef CTSSvr inherit2;

public:
	CTSModule(void);
	virtual ~CTSModule(void);

	virtual int before_module_init( int argc, ACE_TCHAR *argv[]);
	virtual int before_proactor_thread();

	virtual int main_loop();

	virtual void fini (void);

	virtual void exit_module( void*);

	virtual void post_exit_cmd();

	virtual ACE_Thread_Mutex& get_mutex(){
		return module_mutex_;
	}

	/**
	* cts服务器定时连接
	* @param t
	**/
	void ctssvr_connect_timer( ACE_UINT64 t);

	/**
	* 选服定时器
	* @param t
	**/
	void ctssvr_teamproxy_timer( ACE_UINT64 t);

	void ctssvr_teamupdate_timer( ACE_UINT64 t);

	/**
	* 在线用户tick
	**/
	void ctssvr_onlineplayer_tick( ACE_UINT64 t);

private:
	//监听器
	GuardAcceptor<CTS_Service>	accepter_;

	//------------------------Global_Data-----------------------------------//
public:
};

#define CTSMODULE ACE_Singleton<CTSModule, ACE_Null_Mutex>::instance()

#endif //__CTSMODULE__H__
