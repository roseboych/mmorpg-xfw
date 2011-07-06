/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSMODULE__H__
#define __CSSMODULE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

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

#include "CSSSvr.h"
#include "CSS_Service.h"

/**
* @class CSSModule
* 
* @brief 定义了消息队列、系统维护协议处理等功能。
* 
**/
class CSSModule : public BasicModule, public CSSSvr
{
	friend class CSS_Service;

public:
	typedef BasicModule inherit;
	typedef CSSSvr inherit2;

public:
	CSSModule(void);
	virtual ~CSSModule(void);

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
	* css服务器定时连接
	* @param t
	**/
	void csssvr_connect_timer( ACE_UINT64 t);

private:
	//监听器
	GuardAcceptor<CSS_Service>	accepter_;

	//------------------------Global_Data-----------------------------------//
public:
};

#define CSSMODULE ACE_Singleton<CSSModule, ACE_Null_Mutex>::instance()

#endif //__CSSMODULE__H__
