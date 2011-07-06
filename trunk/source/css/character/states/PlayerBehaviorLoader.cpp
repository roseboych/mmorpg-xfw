/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "PlayerBehaviorLoader.h"

#include <prolib/datastore/character_define.h>
#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/GlobalConfig.h>

bool PlayerBehaviorLoader::load_behaviorconfig()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	TiXmlDocument doc;

	NS_STL::string bstr =dsrc->get_txtfilecontent( "appdata/character", "character_behavior.xml");
	if( bstr.size() <= 0 || !XmlUtil::loadfromstring( (char*)bstr.c_str(), doc))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load character behavior config file(appdata/character/character_behavior.xml) failed");
		return false;
	}

	TiXmlElement* root =doc.RootElement();

	for( TiXmlElement* e =root->FirstChildElement( "state"); e != NULL; e =e->NextSiblingElement( "state"))
	{
		NS_STL::string fstr =XmlUtil::GetXmlAttrStr( e, "from", "");
		S_INT_8 ftype =CHRSTATE_NONE;
		if( fstr == "" || ( ftype =get_behavior_type( fstr)) == CHRSTATE_NONE)
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "character behavior config file(appdata/character/character_behavior.xml) error, this [%s] state doesn't exist ", fstr.c_str());
			return false;
		}

		BehaviorTreeNode& nod =BaseActionState::get_behaviortreenode( ftype);

		for( TiXmlElement* e2 =e->FirstChildElement( "goto"); e2 != NULL; e2 =e2->NextSiblingElement( "goto"))
		{
			NS_STL::string gotostr =XmlUtil::GetXmlAttrStr( e2, "to", "");
			S_INT_8 totype =NO_INITVALUE;
			if( gotostr == "" || ( totype =get_behavior_type( gotostr)) == CHRSTATE_NONE)
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "character behavior config file(appdata/character/character_behavior.xml) error, this [%s] state doesn't exist ", fstr.c_str());
				return false;
			}

			nod.regist_nextstate( totype);
		}

		TiXmlElement* e3 =e->FirstChildElement( "fight");
		S_INT_8 tofight =CHRSTATE_NONE;
		if( e3)
		{
			NS_STL::string gotostr =XmlUtil::GetXmlAttrStr( e3, "to", "");
			if( gotostr != "")
				tofight =get_behavior_type( gotostr);
		}
		nod.refer_fightstate_ =tofight;

	}

	return true;
}

S_INT_8 PlayerBehaviorLoader::get_behavior_type( const NS_STL::string& name)
{
	for( int ii =0; ii < CHRSTATE_MAX; ++ii)
	{
		if( g_chrstate_desc[ii] == name)
			return ii;
	}

	return CHRSTATE_NONE;
}
