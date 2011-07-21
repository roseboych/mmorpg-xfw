/**
* css
*
* @category		task system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "TaskDefaultImpl.h"

#include <reslib/tasks/TaskDefaultImplParam.h>

#include "../character/Player.h"

TaskDefaultImpl::TaskDefaultImpl()
{
}

S_BOOL TaskDefaultImpl::get_task( Player* curplayer, TaskInfo* tparam)
{
	TaskDefaultImplParam* tdef =dynamic_cast<TaskDefaultImplParam*>(tparam->clsimpl_param_);
	if( curplayer->get_level() >= tdef->minlevel_ && curplayer->get_level() <= tdef->maxlevel_)
		return true;

	return false;
}

S_BOOL TaskDefaultImpl::submit_task( Player* curplayer, TaskInfo* tparam)
{
	return true;
}
