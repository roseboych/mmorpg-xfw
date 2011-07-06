/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __DEATHSTANDACTION__H__
#define __DEATHSTANDACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class DeathStandAction
* 
* @brief 死亡状态下的站立
**/
class DeathStandAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	DeathStandAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_DEATH_STAND;}
};

#endif	//__DEATHSTANDACTION__H__