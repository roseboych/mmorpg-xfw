/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLTREE__H__
#define __SKILLTREE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>
#include "reslib/skills/SkillDesc.h"

#include <string>
#include <map>
#include <list>

class TiXmlElement;

NAMESPACE_APPSCRIPT_BEGIN

class ScriptContext;

NAMESPACE_APPSCRIPT_END

/**
* @class SkillTree
* 
* @brief 技能树配置
**/
class SkillTree
{
public:
	typedef NS_STL::map<int, SkillDesc>	SKILLS_MAP;

public:
	SkillTree();

	void release();

	/**
	* 注册一个技能
	* @param e
	* @return
	**/
	bool regist_skill( TiXmlElement* e, NS_STL::string npath);

	/**
	* 根据编号id查找技能描述
	* @param sid
	* @return
	**/
	SkillDesc* get_skilldesc( int sid);

	/**
	* 获取所有的技能配置信息
	* @param sks
	**/
	void get_allskills( NS_STL::list<SkillDesc*>& sks);

	/**
	* 注册所有的技能脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

protected:
	SKILLS_MAP	skills_;
};

#endif	//__SKILLTREE__H__
