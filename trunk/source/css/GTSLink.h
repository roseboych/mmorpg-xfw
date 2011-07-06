/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSLINK__H__
#define __GTSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <corelib/core/LinkBase.h>
#include <reslib/deploy/GTSOption.h>

#include "CSS_Service.h"

/**
* @class GTSLink
*
* @brief 
*
**/
class GTSLink : public LinkBase<CSS_Service>
{
	typedef LinkBase<CSS_Service> inherit;
public:
	GTSLink();

	virtual void set_linkservice( CSS_Service* s);
	virtual void send_protocol( BasicProtocol* p);
	virtual void disconn_to_server();
	virtual void reset();

	void set_option( GTSOption* s);

public:
	//配置
	GTSOption* opt_;

	//锁
	ACE_Thread_Mutex	svr_mutex_;
};

#endif	//__GTSLINK__H__
