/**
* css application
*
* @category		NPC
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "NPCEntity.h"

#include <ace/OS_NS_sys_time.h>
#include <prolib/Pro_all.h>
#include "../../GTSLink.h"
#include "../../BaseStoryService.h"
#include "../../CSSModule.h"

NPCEntity::NPCEntity():inherit()
{

}

NPCEntity::~NPCEntity()
{

}

bool NPCEntity::can_beattacked( UnitBase* pattack)
{
	return false;
}
