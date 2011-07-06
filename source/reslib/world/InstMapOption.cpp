/**
* reslib
*
* @category		map config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/world/InstMapOption.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

InstMapOption::InstMapOption():
ctrl_classname_( "")
{
}

S_BOOL InstMapOption::load_instmapopt( TiXmlElement* e)
{
	TiXmlElement* ctrlnode =e->FirstChildElement( "inst_ctrl");
	if( ctrlnode)
		ctrl_classname_ =XmlUtil::GetXmlAttrStr( ctrlnode, "ctrlclass");

	return true;
}
