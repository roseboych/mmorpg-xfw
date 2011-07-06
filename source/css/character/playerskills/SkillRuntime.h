/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLRUNTIME__H__
#define __SKILLRUNTIME__H__

#include "../../css_pre.h"
#include <reslib/skills/SkillDesc.h>
#include <prolib/SerializeObject.h>
#include "SkillDataPackageBase.h"

class Player;

/**
* @class SkillRuntime
* 
* @brief 技能运行时
**/
class SkillRuntime
{
public:
	SkillRuntime( SkillDesc* sk);
	~SkillRuntime();

	/**
	* 根据技能配置信息，构造运行时数据包
	**/
	void build_runtimepkg();

	SkillDataPackageBase* get_runtimedata(){ return runtime_data_;}

	bool is_cooldown();

	void update_lastruntime();

	bool is_running(){ return false;}

	SerializeObject* clone_runtimedata();

	SkillDesc* get_skilldesc(){ return skill_desc_;}

	/**
	* 非瞬发技能的连续处理函数入口
	* @param tnow
	* @param user	技能所有人
	* @return true:技能处理结束 false:需要继续处理
	**/
	bool process( const ACE_UINT64& tnow, Player* user);

	//-------------------wrapper skilldesc property access-------------------------
	bool is_driving(){ return skill_desc_->driving_;}
	bool is_singleattack(){ return skill_desc_->singleattack_;}
	bool is_immediate(){ return skill_desc_->immediate_;}

private:
	//运行时数据包
	SkillDataPackageBase*	runtime_data_;
	//技能描述
	SkillDesc*	skill_desc_;
};

#endif	//__SKILLRUNTIME__H__
