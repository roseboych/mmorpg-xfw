/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "PlayerActionSelector.h"

#include "Player.h"

PlayerActionSelector::PlayerActionSelector():
owner_( 0)
{
}

void PlayerActionSelector::set_owner( Player* p)
{
	this->owner_ =p;

	nofight_sitdown_.set_owner( p);
	nofight_stand_.set_owner( p);
	nofight_walk_.set_owner( p);
	nofight_run_.set_owner( p);
	nofight_ride_.set_owner( p);
	nofight_fly_.set_owner( p);
	fight_stand_.set_owner( p);
	fight_walk_.set_owner( p);
	fight_run_.set_owner( p);
	fight_ride_.set_owner( p);
	fight_fly_.set_owner( p);
	nav_fly_.set_owner( p);
	death_stand_.set_owner( p);
	death_walk_.set_owner( p);
	death_run_.set_owner( p);
}

void PlayerActionSelector::reset_selector()
{
	nofight_sitdown_.reset_state();
	nofight_stand_.reset_state();
	nofight_walk_.reset_state();
	nofight_run_.reset_state();
	nofight_ride_.reset_state();
	nofight_fly_.reset_state();
	fight_stand_.reset_state();
	fight_walk_.reset_state();
	fight_run_.reset_state();
	fight_ride_.reset_state();
	fight_fly_.reset_state();
	nav_fly_.reset_state();
	death_stand_.reset_state();
	death_walk_.reset_state();
	death_run_.reset_state();
}

BaseActionState* PlayerActionSelector::get_statebyid( int id)
{
	if( id == CHRSTATE_NOFIGHT_SITDOWN)
		return &nofight_sitdown_;
	else if( id == CHRSTATE_NOFIGHT_STAND)
		return &nofight_stand_;
	else if( id == CHRSTATE_NOFIGHT_WALK)
		return &nofight_walk_;
	else if( id == CHRSTATE_NOFIGHT_RUN)
		return &nofight_run_;
	else if( id == CHRSTATE_NOFIGHT_RIDE)
		return &nofight_ride_;
	else if( id == CHRSTATE_NOFIGHT_FLY)
		return &nofight_fly_;
	else if( id == CHRSTATE_FIGHT_STAND)
		return &fight_stand_;
	else if( id == CHRSTATE_FIGHT_WALK)
		return &fight_walk_;
	else if( id == CHRSTATE_FIGHT_RUN)
		return &fight_run_;
	else if( id == CHRSTATE_FIGHT_RIDE)
		return &fight_ride_;
	else if( id == CHRSTATE_FIGHT_FLY)
		return &fight_fly_;
	else if( id == CHRSTATE_NAV_FLY)
		return &nav_fly_;
	else if( id == CHRSTATE_DEATH_STAND)
		return &death_stand_;
	else if( id == CHRSTATE_DEATH_WALK)
		return &death_walk_;
	else if( id == CHRSTATE_DEATH_RUN)
		return &death_run_;

	return 0;
}

void PlayerActionSelector::regist_finish()
{
	nofight_sitdown_.regist_finish();
	nofight_stand_.regist_finish();
	nofight_walk_.regist_finish();
	nofight_run_.regist_finish();
	nofight_ride_.regist_finish();
	nofight_fly_.regist_finish();
	fight_stand_.regist_finish();
	fight_walk_.regist_finish();
	fight_run_.regist_finish();
	fight_ride_.regist_finish();
	fight_fly_.regist_finish();
	nav_fly_.regist_finish();
	death_stand_.regist_finish();
	death_walk_.regist_finish();
	death_run_.regist_finish();
}

BaseActionState* PlayerActionSelector::get_default_state()
{
	if( owner_->get_died() == 0)
	{
		nofight_stand_.begin_state();
		return &nofight_stand_;
	}
	else
	{
		death_stand_.begin_state();
		return &death_stand_;
	}
}

BaseActionState* PlayerActionSelector::goto_state( int newstate)
{
	BaseActionState* curstate =owner_->get_curstate();
	ACE_ASSERT( curstate != 0);

	if( !curstate->can_gotonextstate( newstate))
		return 0;

	BaseActionState* nextstate =get_statebyid( newstate);
	if( !nextstate->can_beginstate())
		return 0;

	curstate->end_state();

	owner_->set_curstate( nextstate);
	nextstate->begin_state();

	return nextstate;
}
