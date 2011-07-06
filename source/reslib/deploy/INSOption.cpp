/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/INSOption.h"

#include "reslib/deploy/CSSOption.h"
#include "reslib/deploy/INSCellOption.h"
#include <algorithm>

INSOption::INSOption():
cssopt_( 0), nums_( 0)
{
}

INSOption::~INSOption()
{
	inscells_.clear();
}
