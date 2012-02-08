/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __FIGHTWALKACTION__H__
#define __FIGHTWALKACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class FightWalkAction
* 
* @brief 战斗状态下的行走
**/
class FightWalkAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	FightWalkAction();

	virtual char get_statetype(){ return ( char)CHRSTATE_FIGHT_WALK;}

};

#endif	//__FIGHTWALKACTION__H__