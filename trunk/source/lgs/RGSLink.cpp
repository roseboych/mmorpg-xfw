/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGSLink.h"

RGSLink::RGSLink():inherit(),opt_( 0),status_( SVRREGIST_ST_NONE)
{
}

void RGSLink::reset()
{
	inherit::reset();

	status_ =SVRREGIST_ST_NONE;
}

bool RGSLink::connect_to_server()
{
	if( !connector_.connect_2_server( opt_->ip_.c_str(), opt_->port_, (LinkBase<RGS_Service>*)this))
		return false;

	return true;
}

bool RGSLink::is_connected()
{
	return status_ == SVRREGIST_ST_SUCC;
}

bool RGSLink::need_connected()
{
	return status_ == SVRREGIST_ST_NONE;
}

bool RGSLink::is_waitregistack()
{
	return status_ == SVRREGIST_ST_WAITACK;
}

void RGSLink::set_option( RGSOption* s)
{
	opt_ =s;
}
