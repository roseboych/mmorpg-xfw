/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskDefaultImplParam.h"

#include <corelib/xml/XmlUtil.h>
#include <corelib/log/logmacro.h>

TaskDefaultImplParam::TaskDefaultImplParam()
{
	minlevel_ =1;
	maxlevel_ =1;
}

bool TaskDefaultImplParam::load_params( TiXmlElement* ptr)
{
	minlevel_ =XmlUtil::GetXmlAttrInt( ptr, "minlevel", 1);
	maxlevel_ =XmlUtil::GetXmlAttrInt( ptr, "maxlevel", 1);

	return true;
}
