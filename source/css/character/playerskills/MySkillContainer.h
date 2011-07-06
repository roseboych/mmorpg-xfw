/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MYSKILLCONTAINER__H__
#define __MYSKILLCONTAINER__H__

#include "../../css_pre.h"

#include <reslib/skills/SkillTree.h>
#include "SkillRuntime.h"

#include <map>

class Player;

/**
* @class MySkillContainer
* 
* @brief 我的技能容器
**/
class MySkillContainer
{
	typedef NS_STL::map<S_INT_32, SkillRuntime*>	SKILLRUNTIME_MAP;
public:
	MySkillContainer();
	~MySkillContainer();

	void set_owner( Player* p);

	void reset();

	void regist_finish();

	void set_curskill( SkillRuntime* sr);

	void tick( const ACE_UINT64& tnow);

	SkillRuntime* get_skillruntime( S_INT_32 skid);

	SkillDesc* get_skillfromtree( S_INT_32 skid);

	/**
	* 学习一个技能
	* @param pdesc
	**/
	SkillRuntime* study_skill( SkillDesc* pdesc);

protected:
	//当前的技能
	SkillRuntime*		cur_skill_;

	//可用的技能
	SKILLRUNTIME_MAP	runtimes_;

	Player*	owner_player_;
	//所属的技能树
	SkillTree*	skill_tree_;
};

#endif	//__MYSKILLCONTAINER__H__
