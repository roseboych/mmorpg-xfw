/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGSLINK__H__
#define __RGSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <corelib/core/LinkBase.h>
#include <corelib/net/GuardConnector.h>
#include <reslib/deploy/RGSOption.h>

#include "RGS_Service.h"

class RGSLink : public LinkBase<RGS_Service>
{
	typedef LinkBase<RGS_Service> inherit;
public:
	RGSLink();

	virtual bool connect_to_server();

	virtual bool is_connected();
	virtual bool need_connected();

	virtual bool is_waitregistack();

	virtual void reset();

	void set_option( RGSOption* s);

	SERVER_REGIST_STATUS_ENUM get_status(){ return status_;}
	void set_status( SERVER_REGIST_STATUS_ENUM s){ status_ =s;}

public:
	SERVER_REGIST_STATUS_ENUM	status_;
	//配置
	RGSOption* opt_;

	GuardConnector<RGS_Service> connector_;
};

#endif	//__RGSLINK__H__
