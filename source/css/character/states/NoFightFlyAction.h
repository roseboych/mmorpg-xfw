/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTFLYACTION__H__
#define __NOFIGHTFLYACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightFlyAction
* 
* @brief 非战斗状态下的飞行
**/
class NoFightFlyAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightFlyAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_NOFIGHT_FLY;}
};

#endif	//__NOFIGHTFLYACTION__H__