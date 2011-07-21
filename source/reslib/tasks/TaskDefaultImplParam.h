/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKDEFAULTIMPLPARAM__H__
#define __TASKDEFAULTIMPLPARAM__H__

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>

#include "reslib/tasks/TaskParamBase.h"

class TaskDefaultImplParam : public TaskParamBase
{
	TASKIMPLPARAM_DEFINE( TaskDefaultImplParam)

public:
	TaskDefaultImplParam();

	virtual bool load_params( TiXmlElement* ptr);

public:
	//可以接任务的最低和最高等级
	S_INT_32	minlevel_;
	S_INT_32	maxlevel_;
};

#endif	//__TASKDEFAULTIMPLPARAM__H__
