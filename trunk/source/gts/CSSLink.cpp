/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSLink.h"

CSSLink::CSSLink():inherit(),opt_( 0),status_( SVRREGIST_ST_NONE)
{
}

void CSSLink::reset()
{
	inherit::reset();

	status_ =SVRREGIST_ST_NONE;
}

bool CSSLink::connect_to_server()
{
	if( !connector_.connect_2_server( opt_->ip_.c_str(), opt_->port_, (LinkBase<CSS_Service>*)this))
		return false;

	return true;
}

bool CSSLink::is_connected()
{
	return status_ == SVRREGIST_ST_SUCC;
}

bool CSSLink::need_connected()
{
	return status_ == SVRREGIST_ST_NONE;
}

bool CSSLink::is_waitregistack()
{
	return status_ == SVRREGIST_ST_WAITACK;
}

void CSSLink::set_option( CSSOption* s)
{
	opt_ =s;
	set_serverindex( opt_->server_index_);
}
