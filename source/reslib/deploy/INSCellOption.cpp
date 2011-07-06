/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/deploy/INSCellOption.h"

#include "reslib/deploy/INSOption.h"
#include "reslib/deploy/CSSOption.h"

INSCellOption::INSCellOption():
ins_opt_( 0),
inscell_id_( 0)
{

}

CSSOption* INSCellOption::get_cssopt()
{
	if( ins_opt_ == 0)
		return 0;

	return ins_opt_->cssopt_;
}

S_INT_32 INSCellOption::get_mapid()
{
	return ins_opt_->sn_;
}

S_INT_32 INSCellOption::get_cssserverid()
{
	return ins_opt_->cssopt_->server_index_;
}
