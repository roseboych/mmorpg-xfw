/**
* corelib
*
* @category		net module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/net/IOCPMutexAllocator.h"

#include "corelib/corelibdef.h"
#include "ace/Guard_T.h"

IOCPMutexAllocator::IOCPMutexAllocator( int init_size)
:inherit(),
mutex_size_( init_size)
{
	mutexs_.reset( FRAMEWK_NEW MutexDescription[mutex_size_]);
}

IOCPMutexAllocator::~IOCPMutexAllocator()
{
}

ACE_Thread_Mutex * IOCPMutexAllocator::alloc_one()
{
	ACE_Guard<ACE_Thread_Mutex>	guard( allocator_mutex_);

	int domutx =0;
	for( int minm =mutexs_[0].reference_, ii =1; ii < mutex_size_; ++ii)
	{
		if( minm > mutexs_[ii].reference_)
		{
			domutx =ii;
			minm =mutexs_[ii].reference_;
		}
	}

	return mutexs_[domutx];
}

void IOCPMutexAllocator::free_one( ACE_Thread_Mutex * m)
{
	ACE_Guard<ACE_Thread_Mutex>	guard( allocator_mutex_);

	for( int ii =0; ii < mutex_size_; ++ii)
	{
		if( &(mutexs_[ii].mutex_) == m)
		{
			-- mutexs_[ii].reference_;
			break;
		}
	}
}
