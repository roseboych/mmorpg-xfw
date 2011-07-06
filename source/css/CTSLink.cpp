﻿/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTSLink.h"

CTSLink::CTSLink():inherit(),opt_( 0),status_( SVRREGIST_ST_NONE)
{
}

void CTSLink::set_linkservice( CTS_Service* s)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::set_linkservice( s);
}

void CTSLink::send_protocol( BasicProtocol* p)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::send_protocol( p);
}

void CTSLink::disconn_to_server()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::disconn_to_server();
}

void CTSLink::reset()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::reset();

	status_ =SVRREGIST_ST_NONE;
}

bool CTSLink::connect_to_server()
{
	if( !connector_.connect_2_server( opt_->ip_.c_str(), opt_->port_, (LinkBase<CTS_Service>*)this))
		return false;

	return true;
}

bool CTSLink::is_connected()
{
	return status_ == SVRREGIST_ST_SUCC;
}

bool CTSLink::need_connected()
{
	return status_ == SVRREGIST_ST_NONE;
}

bool CTSLink::is_waitregistack()
{
	return status_ == SVRREGIST_ST_WAITACK;
}

void CTSLink::set_option( CTSOption* s)
{
	opt_ =s;
	set_serverindex( 0);
}
