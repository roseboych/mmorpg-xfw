/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/core/TimerContainer.h"

#include "ace/OS_NS_sys_time.h"

TimerContainer::TimerContainer()
{
}

TimerContainer::~TimerContainer()
{
}

void TimerContainer::reset()
{
	timer_id_generator_ =0;
	timers_.clear();
}

void TimerContainer::timer_tick()
{
	ACE_UINT64 now_t =ACE_OS::gettimeofday().get_msec();

	timer_tick( now_t);
}

void TimerContainer::timer_tick( S_TIMESTAMP now_t)
{
	for( TIMER_LIST::iterator iter =timers_.begin(); iter != timers_.end(); ++iter)
	{
		timer_object& obj =(*iter);

		if( obj.oldtime_ + obj.timestep_ > now_t)
			continue;

		obj.oldtime_ =now_t;
		obj.fun_( now_t);
	}
}

int TimerContainer::add_timer( int step, MODULETIMER_FUN_MAP f)
{
	timer_object obj( generate_timerid(), step, f);
	timers_.push_back( obj);

	return obj.tid_;
}

void TimerContainer::del_timer( int tid)
{
	for( TIMER_LIST::iterator iter =timers_.begin(); iter != timers_.end(); ++iter)
	{
		timer_object& obj =(*iter);
		if( obj.tid_ == tid)
		{
			timers_.erase( iter);
			return;
		}
	}
}

void TimerContainer::delall_timer()
{
	timers_.clear();
}
