/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTRUNACTION__H__
#define __NOFIGHTRUNACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightRunAction
* 
* @brief 非战斗状态下的奔跑
**/
class NoFightRunAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightRunAction();

	virtual S_INT_8 get_statetype(){ return ( S_INT_8)CHRSTATE_NOFIGHT_RUN;}
};

#endif	//__NOFIGHTRUNACTION__H__