/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSS2Link.h"

CSS2Link::CSS2Link():inherit(),opt_( 0),status_( SVRREGIST_ST_NONE)
{
}

void CSS2Link::set_linkservice( CSS2_Service* s)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::set_linkservice( s);
}

void CSS2Link::send_protocol( BasicProtocol* p)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::send_protocol( p);
}

void CSS2Link::disconn_to_server()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::disconn_to_server();
}

void CSS2Link::reset()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::reset();

	status_ =SVRREGIST_ST_NONE;
}

bool CSS2Link::connect_to_server()
{
	if( !connector_.connect_2_server( opt_->ip_.c_str(), opt_->port_, (LinkBase<CSS2_Service>*)this))
		return false;

	return true;
}

bool CSS2Link::is_connected()
{
	return status_ == SVRREGIST_ST_SUCC;
}

bool CSS2Link::need_connected()
{
	return status_ == SVRREGIST_ST_NONE;
}

bool CSS2Link::is_waitregistack()
{
	return status_ == SVRREGIST_ST_WAITACK;
}

void CSS2Link::set_option( CSSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}
