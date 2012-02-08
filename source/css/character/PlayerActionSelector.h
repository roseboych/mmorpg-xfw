/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PLAYERACTIONSELECTOR__H__
#define __PLAYERACTIONSELECTOR__H__

#include "../css_pre.h"

#include "BaseActionState.h"

#include "states/NoFightSitdownAction.h"
#include "states/NoFightStandAction.h"
#include "states/NoFightWalkAction.h"
#include "states/NoFightRunAction.h"
#include "states/NoFightRideAction.h"
#include "states/NoFightFlyAction.h"
#include "states/FightStandAction.h"
#include "states/FightWalkAction.h"
#include "states/FightRunAction.h"
#include "states/FightRideAction.h"
#include "states/FightFlyAction.h"
#include "states/NavigateFlyAction.h"
#include "states/DeathStandAction.h"
#include "states/DeathWalkAction.h"
#include "states/DeathRunAction.h"

class Player;

/**
* @class PlayerActionSelector
* 
* @brief player状态机
**/
class PlayerActionSelector
{
public:
	PlayerActionSelector();

	void set_owner( Player* p);

	/**
	* 重置selector和所有的状态
	**/
	void reset_selector();

	/**
	* 根据状态编号获取状态
	* @param id
	* @return
	**/
	BaseActionState* get_statebyid( int id);

	/**
	* 获取缺省状态
	* @return
	**/
	BaseActionState* get_default_state();

	/**
	* 切换到新的状态
	* @param newstate
	* @return
	**/
	BaseActionState* goto_state( int newstate);

	/**
	* 在player完成数据注册之后调用
	* 初始化状态对应的技能信息等
	**/
	void regist_finish();

private:
	Player*	owner_;

	//分配所有已知的状态，减少动态分配的系统开销
	NoFightSitdownAction	nofight_sitdown_;
	NoFightStandAction	nofight_stand_;
	NoFightWalkAction	nofight_walk_;
	NoFightRunAction	nofight_run_;
	NoFightRideAction	nofight_ride_;
	NoFightFlyAction	nofight_fly_;
	FightStandAction	fight_stand_;
	FightWalkAction		fight_walk_;
	FightRunAction		fight_run_;
	FightRideAction		fight_ride_;
	FightFlyAction		fight_fly_;
	NavigateFlyAction	nav_fly_;
	DeathStandAction	death_stand_;
	DeathWalkAction		death_walk_;
	DeathRunAction		death_run_;
};

#endif	//__PLAYERACTIONSELECTOR__H__
