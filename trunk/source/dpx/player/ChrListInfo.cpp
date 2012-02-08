/**
* dpx application
*
* @category		player data
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ChrListInfo.h"

ChrListInfo::ChrListInfo()
{
	equiplen_ =CHRITEMS_MAX;
	memset( (void*)&equips_[0], -1, sizeof( int)*CHRITEMS_MAX);
}
