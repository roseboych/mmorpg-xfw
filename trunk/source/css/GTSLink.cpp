/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTSLink.h"

GTSLink::GTSLink():inherit(),opt_( 0)
{
}

void GTSLink::set_linkservice( CSS_Service* s)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::set_linkservice( s);
}

void GTSLink::send_protocol( BasicProtocol* p)
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::send_protocol( p);
}

void GTSLink::disconn_to_server()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::disconn_to_server();
}

void GTSLink::reset()
{
	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	inherit::reset();
}

void GTSLink::set_option( GTSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}
