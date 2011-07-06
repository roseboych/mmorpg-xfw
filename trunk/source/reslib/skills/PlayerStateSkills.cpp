/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/skills/PlayerStateSkills.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

PlayerStateSkills::PlayerStateSkills()
{
}

void PlayerStateSkills::release()
{
	skills_.clear();
}

bool PlayerStateSkills::regist_skills( TiXmlElement* e, SkillTree& st)
{
	for( TiXmlElement* e2 =e->FirstChildElement( "skill"); e2 != NULL; e2 =e2->NextSiblingElement( "skill"))
	{
		NS_STL::string sid =XmlUtil::GetXmlAttrStr( e2, "id");
		int iid =ShareUtil::atoi( sid.c_str(), 16);

		SkillDesc* sk =st.get_skilldesc( iid);
		if( sk == 0)
			return false;

		skills_[sk->skillid_] =sk;
	}

	return true;
}

SkillDesc* PlayerStateSkills::get_skilldesc( int id)
{
	SKILLS_MAP::iterator fiter =skills_.find( id);
	if( fiter == skills_.end())
		return 0;
	return fiter->second;
}
