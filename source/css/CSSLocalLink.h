/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CSSLOCALLINK__H__
#define __CSSLOCALLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "css_pre.h"

#include <reslib/deploy/CSSOption.h>
#include <corelib/core/INetLink.h>

/**
* @class CSSLocalLink
*
* @brief 
*
**/
class CSSLocalLink : public INetLink
{
public:
	CSSLocalLink();

	virtual void send_protocol( BasicProtocol* p);

	virtual S_INT_32 get_serverindex();

	void set_option( CSSOption* s);

public:
	CSSOption* opt_;
};

#endif //__CSSLOCALLINK__H__
