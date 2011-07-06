/**
* lgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGSMODULE__H__
#define __LGSMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <ace/Service_Types.h>
#include <ace/svc_export.h>
#include <ace/Asynch_Connector.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <corelib/core/BasicModule.h>
#include <corelib/core/BasicService.h>
#include <corelib/core/BasicCommand.h>
#include <prolib/BasicProtocol.h>

#include "LGSSvr.h"
#include "LGS_Service.h"
#include "LGSAcceptor.h"

#define LGS_GETPLAYER_FROMCACHE( usr, pro)															\
	Player* ##usr =get_player( ##pro->get_uuidglobalindex());										\
	if( ##usr == 0 || !##usr->is_samesession( ##pro->get_uuiduserid(), ##pro->get_uuidinitstmp()))	\
		return
/**
* @class LGSModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class LGSModule : public BasicModule, public LGSSvr
{
	friend class LGS_Service;

public:
	typedef BasicModule inherit;
	typedef LGSSvr inherit2;

public:
	LGSModule(void);
	virtual ~LGSModule(void);

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
	* rgs服务器定时连接
	* @param t
	**/
	void rgssvr_connect_timer( ACE_UINT64 t);

	/**
	* 玩家定时检测
	* @param t
	**/
	void player_onlinetick_timer( ACE_UINT64 t);

private:
	//监听器
	LGSAcceptor	accepter_;

	//------------------------Global_Data-----------------------------------//
public:
};

#define LGSMODULE ACE_Singleton<LGSModule, ACE_Null_Mutex>::instance()

#endif //__LGSMODULE__H__
