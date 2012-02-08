/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLTREECONFIG__H__
#define __SKILLTREECONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include <corelib/corelibdef.h>

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <map>
#include <string>

#include <prolib/Pro_define.h>

#include "reslib/skills/SkillDesc.h"
#include "reslib/skills/SkillTree.h"
#include "reslib/skills/PlayerStateSkills.h"
#include "reslib/character/character_define.h"

NAMESPACE_APPSCRIPT_BEGIN

class ScriptContext;

NAMESPACE_APPSCRIPT_END

/**
* @class SkillTreeConfig
* 
* @brief 角色技能树配置
**/
class SkillTreeConfig
{
public:
	SkillTreeConfig();
	~SkillTreeConfig();

	/**
	* 加载角色技能树配置
	* @return
	**/
	bool load_skilltree();
	void unload_skilltree();

	/**
	* 注册所有的技能脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	/**
	* 根据种族，职业和状态获取可用的技能列表
	* @param race
	* @param profession
	* @param state
	* @return
	**/
	PlayerStateSkills* get_playerstateskills( char race, char profession, char state);

	/**
	* 根据种族和职业获取技能树配置
	* @param race
	* @param profession
	* @return
	**/
	SkillTree* get_skilltree( char race, char profession);

private:
	/**
	* 状态转化
	* @param name
	* @return
	**/
	int state_name2iid( const NS_STL::string& name);

protected:
	//根据种族和职业划分的技能配置
	SkillTree			human_skilltree_[PROFESSION_HUMAN_MAX];
	PlayerStateSkills	human_state_skills_[PROFESSION_HUMAN_MAX][CHRSTATE_MAX];

	//skill基础类初始化脚本
	NS_STL::string	skillbase_script_;
};

#define SKILLTREE_CFG	ACE_Singleton< SkillTreeConfig, ACE_Null_Mutex>::instance()

#endif	//__SKILLTREECONFIG__H__
