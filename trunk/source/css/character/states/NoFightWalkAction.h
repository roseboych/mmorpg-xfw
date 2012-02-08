/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTWALKACTION__H__
#define __NOFIGHTWALKACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightWalkAction
* 
* @brief 非战斗状态下的行走
**/
class NoFightWalkAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightWalkAction();

	virtual char get_statetype(){ return ( char)CHRSTATE_NOFIGHT_WALK;}
};

#endif	//__NOFIGHTWALKACTION__H__