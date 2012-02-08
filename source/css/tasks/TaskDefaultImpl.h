/**
* css
*
* @category		task system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKDEFAULTIMPL__H__
#define __TASKDEFAULTIMPL__H__

#include <reslib/reslib_def.h>

#include <corelib/corelibdef.h>
#include <string>

#include "TaskBase.h"

class TaskDefaultImpl : public TaskBase
{
	TASKCLASSIMPL_DEFINE( TaskDefaultImpl)
public:
	TaskDefaultImpl();

	virtual bool get_task( Player* curplayer, TaskInfo* tparam);
	virtual bool submit_task( Player* curplayer, TaskInfo* tparam);

};

#endif	//__TASKDEFAULTIMPL__H__
