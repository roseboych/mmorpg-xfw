/**
* css
*
* @category		task system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "TaskHelper.h"

#include <corelib/script/ScriptContext.h>
#include "../BaseStoryService.h"
#include "../character/Player.h"

#include "TaskBase.h"
#include "TaskDefaultImpl.h"

TaskHelper::TaskHelper()
{
	//regist task impl class
	taskimpl_.push_back( FRAMEWK_NEW TaskDefaultImpl());
}

TaskHelper::~TaskHelper()
{
	release();
}

void TaskHelper::release()
{
	for( size_t ii =0; ii < taskimpl_.size(); ++ii)
		delete taskimpl_[ii];
	taskimpl_.clear();
}

TaskBase* TaskHelper::get_taskbyclsname( NS_STL::string clsname)
{
	for( size_t ii =0; ii < taskimpl_.size(); ++ii)
	{
		if( taskimpl_[ii]->get_classname() == clsname)
			return taskimpl_[ii];
	}

	return 0;
}

char TaskHelper::get_task( int tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc)
{
	char ret =0;

	return ret;
}

char TaskHelper::submit_task( int tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc)
{
	char ret =0;

	return ret;
}
