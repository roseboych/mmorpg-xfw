/**
* css
*
* @category		enter inst
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "EnterInstTask.h"

#include <reslib/world/StoryMapOption.h>
#include <reslib/world/InstMapOption.h>
#include <reslib/world/EnterInstTaskParam.h>

#include "../character/Player.h"

EnterInstTask::EnterInstTask()
{
}

bool EnterInstTask::join_instance( EnterInstParamBase* param, Player* curplayer)
{
	EnterInstTaskParam* ptr =dynamic_cast<EnterInstTaskParam*>( param);
	if( curplayer->get_level() >= ptr->level_)
		return true;

	return false;
}
