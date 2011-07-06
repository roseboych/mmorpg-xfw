/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSLink.h"

CSSLink::CSSLink():inherit(),opt_( 0)
{
}

void CSSLink::set_linkservice( CSS_Service* s)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::set_linkservice( s);
}

void CSSLink::send_protocol( BasicProtocol* p)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::send_protocol( p);
}

void CSSLink::disconn_to_server()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::disconn_to_server();
}

void CSSLink::reset()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::reset();
}

void CSSLink::set_option( CSSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}
