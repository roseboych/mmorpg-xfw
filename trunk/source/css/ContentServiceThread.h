/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CONTENTSERVICETHREAD__H__
#define __CONTENTSERVICETHREAD__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Task.h>

class BaseStoryService;

/**
* @class MainContentThread
* 
* @brief 内容服务线程池
**/
class ContentServiceThread : public ACE_Task_Base
{
	typedef ACE_Task_Base inherit;

public:
	ContentServiceThread( S_INT_32 n =5);
	virtual ~ContentServiceThread();

	/**
	* 初始化服务内容
	* @param opt
	* @param ms
	**/
	bool start_threads();
	void end_threads();

	/**
	* 线程入口函数
	**/
	virtual int svc();

	/**
	* 设置线程数
	* @param n
	**/
	void set_threadnum( S_INT_32 n){ 
		if( n <= 0)
			thread_nums_ =1;
		else
			thread_nums_ =n;
	}

protected:
	friend BaseStoryService* get_thiscontext();

	//支持的线程数
	S_INT_32	thread_nums_;
	bool	initialized_;
	//是否退出线程
	bool volatile	will_exit_;
};

#endif	//__CONTENTSERVICETHREAD__H__
