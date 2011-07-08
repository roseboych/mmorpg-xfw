/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSMODULE__H__
#define __GTSMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "gts_pre.h"

#include <ace/Service_Types.h>
#include <ace/svc_export.h>
#include <ace/Asynch_Connector.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <corelib/core/BasicModule.h>
#include <corelib/core/BasicService.h>
#include <corelib/core/BasicCommand.h>
#include <prolib/BasicProtocol.h>

#include "GTSSvr.h"
#include "GTS_Service.h"
#include "GTSAcceptor.h"

#define GTS_GETPLAYER_FROMCACHE( usr, pro)						\
	Player* ##usr =get_player( ##pro->get_uuidglobalindex());	\
	if( ##usr == 0 || !##pro->same_session( ##usr->uuid_))		\
		return

/**
* @class LGSModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class GTSModule : public BasicModule, public GTSSvr
{
	friend class GTS_Service;

public:
	typedef BasicModule inherit;
	typedef GTSSvr inherit2;

public:
	GTSModule(void);
	virtual ~GTSModule(void);

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
	* gts服务器定时连接
	* @param t
	**/
	void gtssvr_connect_timer( ACE_UINT64 t);

	/**
	* 玩家定时检测
	* @param t
	**/
	void player_onlinetick_timer( ACE_UINT64 t);

private:
	//监听器
	GTSAcceptor	accepter_;

	//------------------------Global_Data-----------------------------------//
public:
};

#define GTSMODULE ACE_Singleton<GTSModule, ACE_Null_Mutex>::instance()

#endif //__GTSMODULE__H__
