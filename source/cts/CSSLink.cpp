/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSLink.h"

CSSLink::CSSLink():inherit(),opt_( 0)
{
}

void CSSLink::reset()
{
	inherit::reset();
}

void CSSLink::set_option( CSSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}
