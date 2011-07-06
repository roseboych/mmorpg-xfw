/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NAVIGATEFLYACTION__H__
#define __NAVIGATEFLYACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NavigateFlyAction
* 
* @brief 巡航状态下的飞行
**/
class NavigateFlyAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NavigateFlyAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_NAV_FLY;}
};

#endif	//__NAVIGATEFLYACTION__H__