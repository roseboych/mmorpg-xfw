/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MUTEXTALLOCATOR__CPP__
#define __MUTEXTALLOCATOR__CPP__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "ace/Guard_T.h"
#include "ace/Static_Object_Lock.h"

#include "corelib/core/MutexAllocator.h"

MutexAllocator::MutexDescription::MutexDescription()
:reference_(0)
{
}

MutexAllocator::MutexDescription::~MutexDescription()
{
}

//----------------------MutexAllocator--------------------//

#endif //__MUTEXTALLOCATOR__CPP__
