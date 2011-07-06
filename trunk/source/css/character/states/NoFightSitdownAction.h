/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTSITDOWNACTION__H__
#define __NOFIGHTSITDOWNACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightSitdownAction
* 
* @brief 非战斗状态下的坐
**/
class NoFightSitdownAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightSitdownAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_NOFIGHT_SITDOWN;}
};

#endif	//__NOFIGHTSITDOWNACTION__H__