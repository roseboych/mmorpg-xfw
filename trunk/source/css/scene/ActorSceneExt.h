/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __ACTORSCENEEXT__H__
#define __ACTORSCENEEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"
#include <ace/Assert.h>

class UnitBase;

/**
* @class ActorSceneExt
* 
* @brief Actor场景信息附加数据
**/
class ActorSceneExt
{
public:
	ActorSceneExt( UnitBase* pOwner):owner_actor_( pOwner){
		ACE_ASSERT( owner_actor_ != 0);
	}

	virtual void reset() =0;

public:
	//所属的移动对象
	UnitBase*	owner_actor_;
};

#endif	//__ACTORSCENEEXT__H__
