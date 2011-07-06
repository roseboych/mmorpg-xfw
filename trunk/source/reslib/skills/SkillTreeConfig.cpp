/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/skills/SkillTreeConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

SkillTreeConfig::SkillTreeConfig()
{

}

SkillTreeConfig::~SkillTreeConfig()
{
	unload_skilltree();
}

int SkillTreeConfig::state_name2iid( const NS_STL::string& name)
{
	for( int ii =0; ii < CHRSTATE_MAX; ++ii)
	{
		if( g_chrstate_desc[ii] == name)
			return ii;
	}

	return NO_INITVALUE;
}

bool SkillTreeConfig::load_skilltree()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	skillbase_script_ =dsrc->get_txtfilecontent( "appdata/skills", "skillbase.lua");
	if( skillbase_script_.size() <= 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load skillbase lua script file(appdata/skills/skillbase.lua) failed");
		return false;
	}

	//human resource
	for( int rc =0; rc < PROFESSION_HUMAN_MAX; ++rc)
	{
		NS_STL::string npath ="appdata/skills/" + g_chr_race_desc[CHR_RACE_HUMAN];
		npath +="/";
		npath +=g_profession_human_desc[rc];

		NS_STL::string cnt =dsrc->get_txtfilecontent( npath.c_str(), "skilltree.xml");
		TiXmlDocument doc;
		if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load %s/skilltree.xml failed", npath.c_str());
			return false;
		}

		TiXmlElement* root =doc.RootElement();

		//获取所有的技能配置
		TiXmlElement* sks =root->FirstChildElement( "skills");
		if( sks == 0)
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load %s/skilltree.xml failed - skills node don't exist!", npath.c_str());
			return false;
		}

		SkillTree& st =human_skilltree_[rc];
		for( TiXmlElement* e =sks->FirstChildElement( "skill"); e != NULL; e =e->NextSiblingElement( "skill"))
		{
			if( !st.regist_skill( e, npath))
				return false;
		}

		//获取和状态相关的技能配置
		TiXmlElement* psp =root->FirstChildElement( "states");
		if( psp == 0)
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load %s/skilltree.xml failed - states node don't exist!", npath.c_str());
			return false;
		}

		for( TiXmlElement* e =psp->FirstChildElement( "state"); e != NULL; e =e->NextSiblingElement( "state"))
		{
			NS_STL::string sname =XmlUtil::GetXmlAttrStr( e, "name");
			int stype =state_name2iid( sname);
			if( stype == NO_INITVALUE)
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load %s/skilltree.xml failed - states node(%s) isn't a validate state!", npath.c_str(), sname.c_str());
				return false;
			}

			PlayerStateSkills& ps =human_state_skills_[rc][stype];
			if( !ps.regist_skills( e, st))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load %s/skilltree.xml failed - states node(%s) have some invalidate skill!", npath.c_str(), sname.c_str());
				return false;
			}
		}
	}

	return true;
}

void SkillTreeConfig::unload_skilltree()
{
	for( int i1 =0; i1 < PROFESSION_HUMAN_MAX; ++i1)
	{
		for( int i2 =0; i2 < CHRSTATE_MAX; ++i2)
		{
			human_state_skills_[i1][i2].release();
		}

		human_skilltree_[i1].release();
	}
}

PlayerStateSkills* SkillTreeConfig::get_playerstateskills( S_INT_8 race, S_INT_8 profession, S_INT_8 state)
{
	if( race == CHR_RACE_HUMAN)
	{
		if( profession < 0 || profession >= PROFESSION_HUMAN_MAX)
			return 0;
		if( state < 0 || state >= CHRSTATE_MAX)
			return 0;

		return &(human_state_skills_[profession][state]);
	}
	else
		return 0;
}

SkillTree* SkillTreeConfig::get_skilltree( S_INT_8 race, S_INT_8 profession)
{
	if( race == CHR_RACE_HUMAN)
	{
		if( profession < 0 || profession >= PROFESSION_HUMAN_MAX)
			return 0;

		return &(human_skilltree_[profession]);
	}
	else
		return 0;
}

bool SkillTreeConfig::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	//注册基础类
	if( !context.run_script( skillbase_script_.c_str()))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "run skill appdata/skills/skillbase.lua script file failed");
		return false;
	}

	//regist human skills
	for( int ind =0; ind < PROFESSION_HUMAN_MAX; ++ind)
	{
		if( !human_skilltree_[ind].regist_to_storyscriptcontext( context))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "above script file error happen under the appdata/skills/%s/%s fold", 
				g_chr_race_desc[CHR_RACE_HUMAN].c_str(), g_profession_human_desc[ind].c_str());
			return false;
		}
	}

	return true;
}
