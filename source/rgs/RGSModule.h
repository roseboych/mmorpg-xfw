/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGSMODULE__H__
#define __RGSMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "rgs_pre.h"

#include <ace/Service_Types.h>
#include <ace/svc_export.h>
#include <ace/Asynch_Connector.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <prolib/BasicProtocol.h>

#include <corelib/core/BasicModule.h>
#include <corelib/core/BasicService.h>
#include <corelib/core/BasicCommand.h>
#include <corelib/net/GuardAcceptor.h>

#include "RGSSvr.h"
#include "RGS_Service.h"
#include "servercontroller/ControllerAcceptor.h"

#define RGS_GETPLAYER_FROMCACHE( usr, pro)							\
	PlayerInfo* ##usr =find_byuserid( ##pro->get_uuiduserid());		\
	if( ##usr == 0 || !##pro->same_session( ##usr->uuid_))			\
		return

/**
* @class RGSModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class RGSModule : public BasicModule, public RGSSvr
{
	friend class RGS_Service;
public:
	typedef BasicModule inherit;
	typedef RGSSvr inherit2;

public:
	RGSModule(void);
	virtual ~RGSModule(void);

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
	* svrteam更新定时器
	**/
	void svrteam_update_timer( ACE_UINT64 t);

	/**
	* 玩家心跳计时器，用来计算退服重连的有效期
	**/
	void svrteam_playertick_timer( ACE_UINT64 t);

private:
	//监听器
	GuardAcceptor<RGS_Service>	accepter_;

	ControllerAcceptor	controller_accepter_;
};

#define RGSMODULE ACE_Singleton<RGSModule, ACE_Null_Mutex>::instance()

#endif //__RGSMODULE__H__
