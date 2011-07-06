/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __DEATHWALKACTION__H__
#define __DEATHWALKACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class DeathWalkAction
* 
* @brief 死亡状态下的行走
**/
class DeathWalkAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	DeathWalkAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_DEATH_WALK;}
};

#endif	//__DEATHWALKACTION__H__