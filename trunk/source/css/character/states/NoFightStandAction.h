/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTSTANDACTION__H__
#define __NOFIGHTSTANDACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightStandAction
* 
* @brief 非战斗状态下的站立
**/
class NoFightStandAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightStandAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_NOFIGHT_STAND;}
};

#endif	//__NOFIGHTSTANDACTION__H__