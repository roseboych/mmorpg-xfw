/**
* css application
*
* @category		monster
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MONSTERENTITY__H__
#define __MONSTERENTITY__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../../css_pre.h"

#include <ace/Thread_Mutex.h>
#include <prolib/BasicProtocol.h>

#include "../UnitBase.h"

class GTSLink;

/**
* @class MonsterEntity
* 
* @brief 怪物对象
**/
class MonsterEntity : public UnitBase
{
	typedef UnitBase inherit;
public:
	MonsterEntity();
	virtual ~MonsterEntity();

	//---------------------------------UnitBase virtual function---------------------------//
	virtual ACTOR_TYPE_ENUM get_actortype(){ return ACTOR_TYPE_MONSTER;}

	virtual bool can_beattacked( UnitBase* pattack);
};

#endif	//__MONSTERENTITY__H__
