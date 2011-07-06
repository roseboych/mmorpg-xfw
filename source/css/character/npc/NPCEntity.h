/**
* css application
*
* @category		NPC
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __NPCENTITY__H__
#define __NPCENTITY__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../../css_pre.h"

#include <ace/Thread_Mutex.h>
#include <prolib/BasicProtocol.h>

#include "../UnitBase.h"

class GTSLink;

/**
* @class NPCEntity
* 
* @brief NPC对象
**/
class NPCEntity : public UnitBase
{
	typedef UnitBase inherit;
public:
	NPCEntity();
	virtual ~NPCEntity();

	//---------------------------------UnitBase virtual function---------------------------//
	virtual ACTOR_TYPE_ENUM get_actortype(){ return ACTOR_TYPE_NPC;}

	virtual bool can_beattacked( UnitBase* pattack);
};

#endif	//__NPCENTITY__H__
