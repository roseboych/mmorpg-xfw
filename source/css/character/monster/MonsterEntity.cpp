/**
* css application
*
* @category		monster
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "MonsterEntity.h"

#include <ace/OS_NS_sys_time.h>
#include <prolib/Pro_all.h>
#include "../../GTSLink.h"
#include "../../BaseStoryService.h"
#include "../../CSSModule.h"

MonsterEntity::MonsterEntity():inherit()
{

}

MonsterEntity::~MonsterEntity()
{

}

bool MonsterEntity::can_beattacked( UnitBase* pattack)
{
	return ( pattack->get_actortype() == ACTOR_TYPE_PLAYER);
}
