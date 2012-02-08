/**
* reslib
*
* @category		副本配置
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/world/EnterInstTaskParam.h"

#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>

EnterInstTaskParam::EnterInstTaskParam()
{
	level_ =1;
}

bool EnterInstTaskParam::load_params( TiXmlElement* ptr)
{
	level_ =XmlUtil::GetXmlAttrInt( ptr, "level", 1);

	return true;
}
