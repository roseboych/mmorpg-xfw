/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLFARFIGHTDP__H__
#define __SKILLFARFIGHTDP__H__

#include "../../css_pre.h"

#include <prolib/datastore/dtpkg_skill.h>
#include "SkillDataPackageBase.h"

/**
* @class SkillFarFightDP
* 
* @brief 远战技能包
**/
class SkillFarFightDP : public SkillDataPackageBase
{
	typedef SkillDataPackageBase inherit;
public:
	SkillFarFightDP();
	virtual ~SkillFarFightDP();

	virtual void init_skillruntime();

	virtual void set_skillid( S_INT_32 id){ dpsk_data_.skillid_ =id;}
	virtual void set_lastruntime( S_TIMESTAMP st){ dpsk_data_.last_runtime_ =st;}
	virtual S_TIMESTAMP get_lastruntime(){ return dpsk_data_.last_runtime_;}

	virtual SerializeObject* clone_runtimedata(){ return dpsk_data_.clone();}

public:
	static void script_regist( lua_State* l);

protected:
	PRO::skpkg_farfight	dpsk_data_;
};

#endif	//__SKILLFARFIGHTDP__H__
