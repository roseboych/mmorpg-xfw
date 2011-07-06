/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __DEATHRUNACTION__H__
#define __DEATHRUNACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class DeathRunAction
* 
* @brief 死亡状态下的跑动
**/
class DeathRunAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	DeathRunAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_DEATH_RUN;}

};

#endif	//__DEATHRUNACTION__H__