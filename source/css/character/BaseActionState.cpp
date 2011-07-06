/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "BaseActionState.h"

#include <reslib/skills/SkillTreeConfig.h>

#include "Player.h"

BehaviorTreeNode BaseActionState::s_behavior_tree_[CHRSTATE_MAX];

BaseActionState::BaseActionState():
owner_player_( 0),
state_skills_( 0)
{
}

void BaseActionState::set_owner( Player* p)
{
	this->owner_player_ =p;
}

bool BaseActionState::can_gotonextstate( S_INT_32 s)
{
	BehaviorTreeNode& node =s_behavior_tree_[ this->get_statetype()];
	return node.is_ingotostate( s);
}

void BaseActionState::reset_state()
{
	state_skills_ =0;
}

void BaseActionState::regist_finish()
{
	state_skills_ =SKILLTREE_CFG->get_playerstateskills( owner_player_->get_race(), owner_player_->get_profession(), this->get_statetype());
	ACE_ASSERT( state_skills_ != 0);
}

bool BaseActionState::is_skillinstate( S_INT_32 skid)
{
	return state_skills_->get_skilldesc( skid) != 0;
}

bool BaseActionState::is_nofightstate()
{
	S_INT_8 st =get_statetype();
	return st >= CHRSTATE_NOFIGHT_SITDOWN && st <= CHRSTATE_NOFIGHT_FLY;
}

bool BaseActionState::is_fightstate()
{
	S_INT_8 st =get_statetype();
	return st >= CHRSTATE_FIGHT_STAND && st <= CHRSTATE_FIGHT_FLY;
}
