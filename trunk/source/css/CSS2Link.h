/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSS2LINK__H__
#define __CSS2LINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <corelib/core/LinkBase.h>
#include <corelib/net/GuardConnector.h>
#include <reslib/deploy/CSSOption.h>

#include "CSS2_Service.h"

/**
* @class CSS2Link
*
* @brief 
*
**/
class CSS2Link : public LinkBase<CSS2_Service>
{
	typedef LinkBase<CSS2_Service> inherit;
public:
	CSS2Link();

	virtual void set_linkservice( CSS2_Service* s);
	virtual void send_protocol( BasicProtocol* p);
	virtual void disconn_to_server();
	virtual void reset();

	virtual bool connect_to_server();

	virtual bool is_connected();
	virtual bool need_connected();

	virtual bool is_waitregistack();

	void set_option( CSSOption* s);

	SERVER_REGIST_STATUS_ENUM get_status(){ return status_;}
	void set_status( SERVER_REGIST_STATUS_ENUM s){ status_ =s;}

public:
	SERVER_REGIST_STATUS_ENUM	status_;
	//配置
	CSSOption* opt_;

	GuardConnector<CSS2_Service> connector_;

	//锁
	ACE_Thread_Mutex	svr_mutex_;
};

#endif	//__CSS2LINK__H__
