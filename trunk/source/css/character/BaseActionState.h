/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BASEACTIONSTATE__H__
#define __BASEACTIONSTATE__H__

#include "../css_pre.h"

#include <prolib/datastore/character_define.h>
#include <reslib/skills/PlayerStateSkills.h>
#include "BehaviorTreeNode.h"

#include <list>

class Player;

/**
* @class BaseActionState
* 
* @brief player状态机基类
**/
class BaseActionState
{
public:
	BaseActionState();

	void set_owner( Player* p);

	/**
	* 获取state类型
	* @return 类型在<prolib/datastore/character_define.h>中定义
	**/
	virtual S_INT_8 get_statetype() =0;

	bool is_nofightstate();
	bool is_fightstate();

	/**
	* 判断是否能过渡到指定的状态
	* @param s
	* @return
	**/
	bool can_gotonextstate( S_INT_32 s);

	void regist_finish();

	/**
	* 技能是否存在于状态的技能列表中.
	* @param skid
	* @return
	**/
	bool is_skillinstate( S_INT_32 skid);

	/**
	* 在切换到该状态之前，判断是否能进入该状态
	* @return
	**/
	virtual bool can_beginstate(){ return true;}

	/**
	* 重置状态，为复用做准备
	**/
	virtual void reset_state();

	/**
	* 状态开始时调用的函数
	**/
	virtual void begin_state(){}

	/**
	* 状态结束时调用的函数
	**/
	virtual void end_state(){}

	static BehaviorTreeNode& get_behaviortreenode( S_INT_32 s){ return s_behavior_tree_[s];}

protected:
	//可用的技能列表
	PlayerStateSkills*	state_skills_;

	//状态机所属的player
	Player* owner_player_;

protected:
	//配置信息
	static BehaviorTreeNode	s_behavior_tree_[CHRSTATE_MAX];
};

#endif	//__BASEACTIONSTATE__H__
