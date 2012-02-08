/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __FIGHTRIDEACTION__H__
#define __FIGHTRIDEACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class FightRideAction
* 
* @brief 战斗状态下的骑乘
**/
class FightRideAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	FightRideAction();

	virtual char get_statetype(){ return ( char)CHRSTATE_FIGHT_RIDE;}
};

#endif	//__FIGHTRIDEACTION__H__