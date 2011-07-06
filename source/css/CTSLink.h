/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTSLINK__H__
#define __CTSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <corelib/core/LinkBase.h>
#include <corelib/net/GuardConnector.h>
#include <reslib/deploy/CTSOption.h>

#include "CTS_Service.h"

/**
* @class CTSLink
*
* @brief 
*
**/
class CTSLink : public LinkBase<CTS_Service>
{
	typedef LinkBase<CTS_Service> inherit;
public:
	CTSLink();

	virtual void set_linkservice( CTS_Service* s);
	virtual void send_protocol( BasicProtocol* p);
	virtual void disconn_to_server();
	virtual void reset();

	virtual bool connect_to_server();

	virtual bool is_connected();
	virtual bool need_connected();

	virtual bool is_waitregistack();

	void set_option( CTSOption* s);

	SERVER_REGIST_STATUS_ENUM get_status(){ return status_;}
	void set_status( SERVER_REGIST_STATUS_ENUM s){ status_ =s;}

public:
	SERVER_REGIST_STATUS_ENUM	status_;
	//配置
	CTSOption* opt_;

	GuardConnector<CTS_Service> connector_;

	//锁
	ACE_Thread_Mutex	svr_mutex_;
};

#endif	//__CTSLINK__H__
