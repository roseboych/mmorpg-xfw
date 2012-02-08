/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __FIGHTFLYACTION__H__
#define __FIGHTFLYACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class FightFlyAction
* 
* @brief 战斗状态下的飞行
**/
class FightFlyAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	FightFlyAction();

	virtual char get_statetype(){ return ( char)CHRSTATE_FIGHT_FLY;}
};

#endif	//__FIGHTFLYACTION__H__