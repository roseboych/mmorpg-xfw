/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DPXLink.h"

DPXLink::DPXLink():inherit(),opt_( 0)
{
}

void DPXLink::reset()
{
	inherit::reset();
}

void DPXLink::set_option( DPXOption* s)
{
	opt_ =s;
}
