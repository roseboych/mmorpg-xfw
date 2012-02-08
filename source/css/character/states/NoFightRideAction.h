/**
* css application
*
* @category		player behavior tree
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NOFIGHTRIDEACTION__H__
#define __NOFIGHTRIDEACTION__H__

#include "../../css_pre.h"
#include "../BaseActionState.h"

/**
* @class NoFightRideAction
* 
* @brief 非战斗状态下的骑乘
**/
class NoFightRideAction : public BaseActionState
{
	typedef BaseActionState inherit;
public:
	NoFightRideAction();

	virtual char get_statetype(){ return ( char)CHRSTATE_NOFIGHT_RIDE;}
};

#endif	//__NOFIGHTRIDEACTION__H__