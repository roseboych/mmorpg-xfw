/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "BaseSceneManage.h"

#include <reslib/world/StoryMapOption.h>
#include "../BaseStoryService.h"

BaseSceneManage::BaseSceneManage( BaseStoryService* pService):
owner_service_( pService)
{
	ACE_ASSERT( owner_service_ != 0);
}

BaseSceneManage::~BaseSceneManage()
{

}

bool BaseSceneManage::init_scenemanage()
{
	return true;
}

void BaseSceneManage::uninit_scenemanage()
{

}

void BaseSceneManage::world_to_local( S_FLOAT_32& x, S_FLOAT_32& y)
{
	owner_service_->get_storyoption()->world_to_local( x, y);
}

void BaseSceneManage::local_to_world( S_FLOAT_32& x, S_FLOAT_32& y)
{
	owner_service_->get_storyoption()->local_to_world( x, y);
}
