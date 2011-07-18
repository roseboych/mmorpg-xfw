/**
* reslib
*
* @category		��������
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTPARAMOFTASK__H__
#define __ENTERINSTPARAMOFTASK__H__

#include "reslib/world/EnterInstParamBase.h"

/**
* @class EnterInstTaskParam
* 
* @brief �������񸱱������жϲ���
* 
**/
class EnterInstTaskParam : public EnterInstParamBase
{
	ENTERINSTCHECK_DEFINE( EnterInstTaskParam)
public:
	EnterInstTaskParam();

	virtual bool load_params( TiXmlElement* ptr);

public:
	S_INT_32	level_;
};

#endif	//__ENTERINSTPARAMOFTASK__H__
