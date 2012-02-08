/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLNEARFIGHTDP__H__
#define __SKILLNEARFIGHTDP__H__

#include "../../css_pre.h"

#include <prolib/datastore/dtpkg_skill.h>
#include "SkillDataPackageBase.h"

/**
* @class SkillNearFightDP
* 
* @brief 近战技能包
**/
class SkillNearFightDP : public SkillDataPackageBase
{
	typedef SkillDataPackageBase inherit;
public:
	SkillNearFightDP();
	virtual ~SkillNearFightDP();

	virtual void init_skillruntime();

	virtual void set_skillid( int id){ dpsk_data_.skillid_ =id;}
	virtual void set_lastruntime( S_TIMESTAMP st){ dpsk_data_.last_runtime_ =st;}
	virtual S_TIMESTAMP get_lastruntime(){ return dpsk_data_.last_runtime_;}

	virtual SerializeObject* clone_runtimedata(){ return dpsk_data_.clone();}

public:
	static void script_regist( lua_State* l);

protected:
	//协议数据
	PRO::skpkg_nearfight	dpsk_data_;
};

#endif	//__SKILLNEARFIGHTDP__H__
