/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MUTEXTALLOCATOR__H__
#define __MUTEXTALLOCATOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <ace/Thread_Mutex.h>

/**
* @class MutexAllocator
* 
* @brief 同步对象管理接口类。
**/
class MutexAllocator
{
public:
	class MutexDescription
	{
	public:
		MutexDescription();
		virtual ~MutexDescription();

		operator ACE_Thread_Mutex* (){
			++reference_;
			return &mutex_;
		};

		ACE_Thread_Mutex mutex_;
		int	reference_;
	};

public:
	virtual ~MutexAllocator(){};

	//获取一个线程锁
	virtual ACE_Thread_Mutex * alloc_one() =0;

	//释放一个线程锁
	virtual void free_one( ACE_Thread_Mutex * m) =0;
};

#endif //__MUTEXTALLOCATOR__H__
