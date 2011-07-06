/**
* corelib
*
* @category		net module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __IOCPMUTEXTALLOCATOR__H__
#define __IOCPMUTEXTALLOCATOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <corelib/core/MutexAllocator.h>

/**
* @class IOCPMutexAllocator
* 
* @brief mutexallocator的iocp分配实现
**/
class IOCPMutexAllocator : public MutexAllocator
{
	typedef MutexAllocator inherit;

public:
	/**
	* 构造函数
	* @param init_size	系统锁的总数
	**/
	IOCPMutexAllocator( int init_size =8);
	virtual ~IOCPMutexAllocator();

	virtual ACE_Thread_Mutex * alloc_one();

	virtual void free_one( ACE_Thread_Mutex * m);

private:
	//预先分配的线程锁池
	ACE_Auto_Array_Ptr<MutexDescription> mutexs_;
	//mutexs_数组的大小
	int	mutex_size_;

	ACE_Thread_Mutex	allocator_mutex_;
};

#define IOCP_MutexApplocator ACE_Singleton<IOCPMutexAllocator, ACE_Null_Mutex>::instance()

#endif //__IOCPMUTEXTALLOCATOR__H__

