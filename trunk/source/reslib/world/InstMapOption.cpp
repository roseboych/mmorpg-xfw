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

#include "reslib/deploy/GlobalConfig.h"
#include "reslib/world/EnterInstParamBase.h"
#include "reslib/world/StoryMapOption.h"

#include <ace/Assert.h>

InstMapOption::InstMapOption():
ctrl_classname_( "")
{
	is_check_enterinst_ =false;
	enterinst_fun_ =FUNSUPPORT_NONE;
	enterinst_param_ =0;
	enterinst_condition_ =0;
	inst_script_ ="";
}

bool InstMapOption::load_instmapopt( TiXmlElement* e, StoryMapOption* storyopt)
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	ACE_ASSERT( dsrc != 0);

	inst_script_ =dsrc->get_txtfilecontent( storyopt->get_respath().c_str(), "inst-impl.lua");

	TiXmlElement* entnode = e->FirstChildElement( "enter_condition");
	if( entnode && !load_enterinstopt( entnode))
		return false;

	TiXmlElement* ctrlnode =e->FirstChildElement( "inst_ctrl");
	if( ctrlnode)
		ctrl_classname_ =XmlUtil::GetXmlAttrStr( ctrlnode, "ctrlclass");

	return true;
}

bool InstMapOption::load_enterinstopt( TiXmlElement* e)
{
	is_check_enterinst_ =XmlUtil::GetXmlAttrYesNo( e, "check", false);
	if( !is_check_enterinst_)
		return true;

	NS_STL::string condt =XmlUtil::GetXmlAttrStr( e, "condtype", "");
	if( condt == "class")
	{
		enterinst_class_ =XmlUtil::GetXmlAttrStr( e, "classimpl", "");
		if( enterinst_class_ == "")
			return false;

		TiXmlElement* t1 =e->FirstChildElement( "classimpl-condition");
		if( t1 == 0)
			return false;
		t1 =t1->FirstChildElement( enterinst_class_);
		if( t1 == 0)
			return false;

		enterinst_param_ =EnterInstParamBase::create( enterinst_class_);
		if( enterinst_param_ == 0)
			return false;

		if( !enterinst_param_->load_params( t1))
			return false;

		enterinst_fun_ =FUNSUPPORT_CLASSIMPL;
	}
	else if( condt == "condition")
	{
		TiXmlElement* t1 =e->FirstChildElement( "sys-condition");
		if( t1 == 0)
			return false;

		enterinst_condition_ =ConditionElement::construct_condition( t1);
		if( enterinst_condition_ == 0)
			return false;

		enterinst_fun_ =FUNSUPPORT_CONDITION;
	}
	else
	{
		//script
		enterinst_fun_ =FUNSUPPORT_SCRIPT;
	}

	return true;
}
