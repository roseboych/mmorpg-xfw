/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLDESC__H__
#define __SKILLDESC__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include <string>

/**
* @class SkillDesc
* 
* @brief 技能配置
**/
class SkillDesc
{
public:
	SkillDesc();

	/**
	* 技能在脚本系统中的实例名称
	* @return NS_STL::string
	**/
	NS_STL::string get_skillinstance();

public:
	//技能编号
	int				skillid_;
	NS_STL::string	skillidsn_;
	//是否主动技能
	bool		driving_;
	//是否单体技能
	bool		singleattack_;
	//瞬发技能
	bool		immediate_;
	//冷却时间(单位秒),缺省0
	int			cooltime_;
	//使用的技能数据包类型
	short		data_package_;
	NS_STL::string	desc_;

	//技能脚本
	NS_STL::string	skill_script_;
};

#endif	//__SKILLDESC__H__
