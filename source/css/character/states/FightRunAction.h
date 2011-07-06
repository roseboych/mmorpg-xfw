/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __FIGHTRUNACTION__H__
#define __FIGHTRUNACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class FightRunAction
* 
* @brief 战斗状态下的奔跑
**/
class FightRunAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	FightRunAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_FIGHT_RUN;}

};

#endif	//__FIGHTRUNACTION__H__