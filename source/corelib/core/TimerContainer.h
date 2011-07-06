/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __TIMERCONTAINER__H__
#define __TIMERCONTAINER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include <ace/Basic_Types.h>

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

#include <list>

//主线程定时器函数指针
typedef boost::function<void( ACE_UINT64)>	MODULETIMER_FUN_MAP;

/**
* @class TimerContainer
*
* @brief 模块定时器
**/
class TimerContainer
{
private:
	struct timer_object
	{
		timer_object( int id, int t, MODULETIMER_FUN_MAP f):
			tid_(id),timestep_( t),fun_(f){ oldtime_ =0;}

		ACE_UINT64 get_checkt(){
 			return oldtime_ + timestep_;
		}

		//句柄
		int tid_;
		//入口函数
		MODULETIMER_FUN_MAP fun_;
		//定时器时间量
		int	timestep_;
		//上一次运行时间
		ACE_UINT64	oldtime_;
	};
	
public:
	typedef NS_STL::list< timer_object>	TIMER_LIST;
public:
	TimerContainer();
	~TimerContainer();

	void reset();

	/**
	* 增加一个定时器
	* @param step
	* @param f
	* @return int 定时器句柄
	**/
	int add_timer( int step, MODULETIMER_FUN_MAP f);

	/**
	* 清除一个定时器
	* @param tid
	**/
	void del_timer( int tid);

	/**
	* 清除所有的定时器
	**/
	void delall_timer();

	/**
	* 定时器心跳
	**/
	void timer_tick();
	void timer_tick( S_TIMESTAMP now_t);

protected:
	/**
	* 获取一个可用的timerid
	* @return
	**/
	inline int generate_timerid();

private:
	//所有的定时器
	TIMER_LIST	timers_;
	//定时器id计数器
	int timer_id_generator_;
};

inline
int TimerContainer::generate_timerid()
{
	return ++timer_id_generator_;
}

#endif	//__TIMERCONTAINER__H__
