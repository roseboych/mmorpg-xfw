/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSLINK__H__
#define __CSSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <corelib/core/LinkBase.h>
#include <reslib/deploy/CSSOption.h>

#include "CSS_Service.h"

/**
* @class CSSLink
*
* @brief 
*
**/
class CSSLink : public LinkBase<CSS_Service>
{
	typedef LinkBase<CSS_Service> inherit;
public:
	CSSLink();

	virtual void set_linkservice( CSS_Service* s);
	virtual void send_protocol( BasicProtocol* p);
	virtual void disconn_to_server();
	virtual void reset();

	void set_option( CSSOption* s);

public:
	//配置
	CSSOption* opt_;

	//锁
	ACE_Thread_Mutex	svr_mutex_;
};

#endif	//__CSSLINK__H__
