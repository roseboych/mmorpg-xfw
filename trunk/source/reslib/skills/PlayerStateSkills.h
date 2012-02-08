/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PLAYERSTATESKILLS__H__
#define __PLAYERSTATESKILLS__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include "reslib/skills/SkillDesc.h"
#include "reslib/skills/SkillTree.h"

#include <map>

class TiXmlElement;

/**
* @class PlayerStateSkills
* 
* @brief player状态和技能映射关系
**/
class PlayerStateSkills
{
public:
	typedef NS_STL::map<int, SkillDesc*>	SKILLS_MAP;
public:
	PlayerStateSkills();

	void release();

	/**
	* 注册和状态相关的技能
	**/
	bool regist_skills( TiXmlElement* e, SkillTree& st);

	SkillDesc* get_skilldesc( int id);

protected:
	//支持的所有技能
	SKILLS_MAP	skills_;
};

#endif	//__PLAYERSTATESKILLS__H__
