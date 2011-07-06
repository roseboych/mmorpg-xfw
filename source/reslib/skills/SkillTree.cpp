/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/skills/SkillTree.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

SkillTree::SkillTree()
{
}

void SkillTree::release()
{
	skills_.clear();
}

bool SkillTree::regist_skill( TiXmlElement* e, NS_STL::string npath)
{
	NS_STL::string sid =XmlUtil::GetXmlAttrStr( e, "id");
	int iid =ShareUtil::atoi( sid.c_str(), 16);
	NS_STL::pair< SKILLS_MAP::iterator, bool> mp1 =
		skills_.insert( NS_STL::make_pair( iid, SkillDesc()));

	if( !mp1.second)
		return false;

	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();

	SkillDesc& sd =mp1.first->second;
	sd.skillid_ =iid;
	sd.skillidsn_ =sid;
	sd.immediate_ =XmlUtil::GetXmlAttrBool( e, "immediate");
	sd.cooltime_ =XmlUtil::GetXmlAttrInt( e, "cooltime", 0);
	sd.data_package_ =XmlUtil::GetXmlAttrInt( e, "data_package", 0);
	sd.driving_ =XmlUtil::GetXmlAttrBool( e, "driving", true);
	sd.singleattack_ =XmlUtil::GetXmlAttrBool( e, "singleattack", true);
	sd.desc_ =XmlUtil::GetXmlAttrStr( e, "desc");

	sid +=".lua";
	sd.skill_script_ =dsrc->get_txtfilecontent( npath.c_str(), sid.c_str());

	if( sd.skill_script_.size() <= 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load skill(%s/%s) lua script file failed", npath.c_str(), sid.c_str());
		return false;
	}

	return true;
}

SkillDesc* SkillTree::get_skilldesc( int sid)
{
	SKILLS_MAP::iterator fiter =skills_.find( sid);
	if( fiter == skills_.end())
		return 0;

	return &( fiter->second);
}

void SkillTree::get_allskills( NS_STL::list<SkillDesc*>& sks)
{
	SKILLS_MAP::iterator iter =skills_.begin(), eiter =skills_.end();
	for( ; iter != eiter; ++iter)
		sks.push_back( &(iter->second));
	sks.unique();
}

bool SkillTree::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	SKILLS_MAP::iterator iter =skills_.begin(), eiter =skills_.end();
	for( ; iter != eiter; ++iter)
	{
		SkillDesc& sk =iter->second;
		if( !context.run_script( sk.skill_script_.c_str()))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "run skill(%s) lua script file failed", sk.skillidsn_.c_str());
			return false;
		}
	}

	return true;
}