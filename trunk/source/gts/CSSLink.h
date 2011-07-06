/**
* gts application
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

#include "gts_pre.h"

#include <corelib/core/LinkBase.h>
#include <corelib/net/GuardConnector.h>
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

	virtual bool connect_to_server();

	virtual bool is_connected();
	virtual bool need_connected();

	virtual bool is_waitregistack();

	virtual void reset();

	void set_option( CSSOption* s);

	SERVER_REGIST_STATUS_ENUM get_status(){ return status_;}
	void set_status( SERVER_REGIST_STATUS_ENUM s){ status_ =s;}

public:
	SERVER_REGIST_STATUS_ENUM	status_;
	//配置
	CSSOption* opt_;

	GuardConnector<CSS_Service> connector_;
};

#endif	//__CSSLINK__H__
