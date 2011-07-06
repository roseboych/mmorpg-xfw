/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "corelib/profiler/ProfilerTags.h"

#include "corelib/profiler/ProfilerAnalysis.h"
#include "corelib/profiler/ProfilerThreadCollector.h"
#include "corelib/util/CRCUtil.h"

#ifdef ENABLE_PROFILING

ProfilerThreadTags::ProfilerThreadTags( const char* profileid, EProfileThreadType threadtype)
{
	if( G_PROFILEANALYSIS->is_startanalysis( threadtype))
	{
		ProfilerThreadCollector* pcollector =G_PROFILEANALYSIS->start_threadcollector( profileid);
		ACE_ASSERT( pcollector != 0);
	}
}

ProfilerThreadTags::~ProfilerThreadTags()
{
	G_PROFILEANALYSIS->stop_threadcollector();
}

ProfilerTags::ProfilerTags( const char* profileId, unsigned long procrc, EProfileScope profileType):
profile_id_( profileId),
profile_crc_( procrc),
profile_scope_( profileType),
thread_collector_( 0)
{
	thread_collector_ =ProfilerThreadCollector::get_profiler_userdata();
	if( thread_collector_ == 0)
		return;

	thread_collector_->add_event( profile_id_, profile_crc_, EProfileEventType_EventBegin, profile_scope_);
}

ProfilerTags::~ProfilerTags()
{
	if( thread_collector_)
		thread_collector_->add_event( profile_id_, profile_crc_, EProfileEventType_EventEnd, profile_scope_);
}

#endif	//ENABLE_PROFILING
