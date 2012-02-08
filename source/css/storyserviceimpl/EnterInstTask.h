/**
* css
*
* @category		enter inst
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTTASK__H__
#define __ENTERINSTTASK__H__

#include "EnterInstConditionBase.h"

/**
* @class EnterInstTask
* 
* @brief 进入任务副本条件判定实现
* 
**/
class EnterInstTask : public EnterInstConditionBase
{
	ENTERINSTCHECKIMPL_DEFINE( EnterInstTask)

public:
	EnterInstTask();

	virtual bool join_instance( EnterInstParamBase* param, Player* curplayer);
};

#endif	//__ENTERINSTTASK__H__
