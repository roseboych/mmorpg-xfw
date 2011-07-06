/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/GTSOption.h"

GTSOption::GTSOption():server_index_(0)
{
}

GTSOption::~GTSOption()
{
}

bool GTSOption::is_userglobalin( int gid)
{
	return gid >= startuser_ && gid < (startuser_ + usernum_);
}
