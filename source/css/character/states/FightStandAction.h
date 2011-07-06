/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __FIGHTSTANDACTION__H__
#define __FIGHTSTANDACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class FightStandAction
* 
* @brief 战斗状态下的站立
**/
class FightStandAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	FightStandAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_FIGHT_STAND;}
};

#endif	//__FIGHTSTANDACTION__H__
