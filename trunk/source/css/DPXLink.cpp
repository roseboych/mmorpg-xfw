/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DPXLink.h"

DPXLink::DPXLink():inherit(),opt_( 0),status_( SVRREGIST_ST_NONE)
{
}

void DPXLink::set_linkservice( DPX_Service* s)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::set_linkservice( s);
}

void DPXLink::send_protocol( BasicProtocol* p)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::send_protocol( p);
}

void DPXLink::disconn_to_server()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::disconn_to_server();
}

void DPXLink::reset()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::reset();

	status_ =SVRREGIST_ST_NONE;
}

bool DPXLink::connect_to_server()
{
	if( !connector_.connect_2_server( opt_->ip_.c_str(), opt_->port_, (LinkBase<DPX_Service>*)this))
		return false;

	return true;
}

bool DPXLink::is_connected()
{
	return status_ == SVRREGIST_ST_SUCC;
}

bool DPXLink::need_connected()
{
	return status_ == SVRREGIST_ST_NONE;
}

bool DPXLink::is_waitregistack()
{
	return status_ == SVRREGIST_ST_WAITACK;
}

void DPXLink::set_option( DPXOption* s)
{
	opt_ =s;
}
