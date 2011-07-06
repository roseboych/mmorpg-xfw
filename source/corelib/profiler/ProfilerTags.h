/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILERTAGS__H__
#define __PROFILERTAGS__H__

#include <prolib/core_type.h>
#include "corelib/profiler/profiler_define.h"

#ifdef ENABLE_PROFILING

class ProfilerThreadCollector;

/**
* @class ProfilerThreadTags
*
* @brief thread collector的辅助类
**/
class ProfilerThreadTags
{
public:
	ProfilerThreadTags( const char* profileid, EProfileThreadType threadtype);
	~ProfilerThreadTags();
};

/**
* @class ProfilerTags
*
* @brief event的辅助类
**/
class ProfilerTags
{
public:
    ProfilerTags( const char* profileId, unsigned long procrc, EProfileScope profileType);
	~ProfilerTags();

private:
	//event名字
    const char*		profile_id_;
	//生成的crc值
	unsigned long	profile_crc_;
	//所属的scope
    EProfileScope	profile_scope_;

	//所属的collector
	ProfilerThreadCollector*	thread_collector_;
};

#endif	//ENABLE_PROFILING

#endif // __PROFILERTAGS__H__
