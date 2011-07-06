/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/RGSOption.h"

RGSOption::RGSOption():max_users_(0)
{
}

bool RGSOption::has_privilege( NS_STL::string name, NS_STL::string pwd)
{
	CONTROLLER_USERS_MAP::iterator fiter =controller_users_.find( name);
	if( fiter == controller_users_.end())
		return false;

	NS_STL::string p =fiter->second;

	return p == pwd;
}
