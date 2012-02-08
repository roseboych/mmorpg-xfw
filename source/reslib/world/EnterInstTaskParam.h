/**
* reslib
*
* @category		副本配置
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTPARAMOFTASK__H__
#define __ENTERINSTPARAMOFTASK__H__

#include "reslib/world/EnterInstParamBase.h"

/**
* @class EnterInstTaskParam
* 
* @brief 进入任务副本条件判断参数
* 
**/
class EnterInstTaskParam : public EnterInstParamBase
{
	ENTERINSTCHECK_DEFINE( EnterInstTaskParam)
public:
	EnterInstTaskParam();

	virtual bool load_params( TiXmlElement* ptr);

public:
	int		level_;
};

#endif	//__ENTERINSTPARAMOFTASK__H__
