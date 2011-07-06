/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTSLink.h"

GTSLink::GTSLink():inherit(),opt_( 0)
{
}

void GTSLink::reset()
{
	inherit::reset();
}

void GTSLink::set_option( GTSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}

bool GTSLink::is_useringts( S_INT_32 userindex)
{
	if( userindex >= opt_->startuser_ && userindex < (opt_->startuser_ + opt_->usernum_))
		return true;
	return false;
}
