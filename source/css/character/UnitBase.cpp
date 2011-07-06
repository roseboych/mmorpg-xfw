/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "UnitBase.h"

#include <corelib/memory/CoreMemory.h>
#include "../scene/ActorSceneExt.h"
#include "../scene/mappaneimpl/ActorSceneExtPane.h"

UnitBase::UnitBase():
scene_ext_( 0)
{
#ifdef SCENE_PANE_IMPL
	scene_ext_ =FRAMEWK_NEW ActorSceneExtPane( this);
#endif
	ACE_ASSERT( scene_ext_ != 0);
}

UnitBase::~UnitBase()
{
	reset();

	if( scene_ext_)
		delete scene_ext_;
	scene_ext_ =0;
}

void UnitBase::reset()
{
	scene_ext_->reset();
}

void UnitBase::regist_eventlistener( UnitEventListener* lnsr)
{
	if( lnsr == 0)
		return;

	event_listener_.push_back( lnsr);
	event_listener_.unique();
}

void UnitBase::unregist_eventlistener( UnitEventListener* lnsr)
{
	if( lnsr == 0)
		return;

	event_listener_.remove( lnsr);
}

void UnitBase::trigger_unitentityreset()
{
	UNITEVENT_LISTENER_LIST::iterator iter =event_listener_.begin(), eiter =event_listener_.end();
	for( ; iter != eiter;)
	{
		UnitEventListener* plnsr =(*iter);
		++iter;

		plnsr->event_unitentity_reset( this);
	}
}
