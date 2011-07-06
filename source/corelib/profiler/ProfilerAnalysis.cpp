/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "ProfilerAnalysis.h"

#include <ace/Guard_T.h>
#include <ace/OS_NS_unistd.h>

#include "corelib/util/CRCUtil.h"
#include "corelib/core/TLSContext.h"
#include "corelib/log/logmacro.h"

#ifdef ENABLE_PROFILING

ACE_Cached_Allocator<ProfilerThreadCollector, ACE_Null_Mutex> ProfilerAnalysis::cache_collector_( 60000);

ProfilerAnalysis::ProfilerAnalysis():
active_profilingscope_( EProfileScope_All),
active_profilethreadtype_( EProfileThreadType_All),
max_level_( -1),
start_analysis_( false),
save_net_( false),
save_localfile_( false)
{
}

ProfilerAnalysis::~ProfilerAnalysis()
{
}

int ProfilerAnalysis::svc()
{
	static ACE_Time_Value sv( 0, 1000);

	while( start_analysis_)
	{
		ACE_OS::sleep( sv);

		ProfilerThreadCollector* pcollector =0;

		{
			ACE_Guard<ACE_Thread_Mutex> mon( threadcollector_mutex_);

			if( waiting_send_collector_.size() == 0)
				continue;

			pcollector =waiting_send_collector_.front();
			waiting_send_collector_.pop_front();
		}

		if( save_net_)
			remote_netstream_.serialize( pcollector);

		if( save_localfile_)
			local_filestream_.serialize( pcollector);

		if( pcollector)
		{
			pcollector->~ProfilerThreadCollector();
			cache_collector_.free( (void*)pcollector);
		}
	}

	remote_netstream_.uninit_netstream();
	local_filestream_.uninit_filestream();

	return 0;
}

void ProfilerAnalysis::start_analysis( bool net, bool localfile, NS_STL::string ip, int port)
{
	if( start_analysis_)
		return;

	start_analysis_ =true;

	save_localfile_ =localfile;
	save_net_ =net;

	if( save_net_)
	{
		if( !remote_netstream_.init_netstream( ip, port))
		{
			MODULE_LOG_ERROR( MODULE_RUNTIME, "performance analysis netstream initialize failed!");
		}
	}

	if( save_localfile_)
	{
		if( !local_filestream_.init_filestream())
		{
			MODULE_LOG_ERROR( MODULE_RUNTIME, "performance analysis filestream initialize failed!");
		}
	}

	this->activate( THR_NEW_LWP|THR_JOINABLE, 1, 1);
}

void ProfilerAnalysis::stop_analysis()
{
	start_analysis_ =false;

	this->wait();
}

bool ProfilerAnalysis::is_startanalysis( EProfileThreadType t)
{
	if( !start_analysis_)
		return false;

	if( (active_profilethreadtype_ & t ) != t)
		return false;
	else
		return true;
}

ProfilerThreadCollector* ProfilerAnalysis::start_threadcollector( const char* threadname)
{
	ProfilerThreadCollector* oldcollector =ProfilerThreadCollector::get_profiler_userdata();

	if( oldcollector)
	{
		if( oldcollector->is_timeout())
		{
			ACE_Guard<ACE_Thread_Mutex> mon( threadcollector_mutex_);

			if( !start_analysis_)
			{
				oldcollector->~ProfilerThreadCollector();
				cache_collector_.free( (void*)oldcollector);
				TLSContext::set_profiler_userdata( 0);
			}
			else
			{
				waiting_send_collector_.push_back( oldcollector);
				TLSContext::set_profiler_userdata( 0);
			}
		}
		else
			return oldcollector;
	}

	{
		ACE_Guard<ACE_Thread_Mutex> mon( threadcollector_mutex_);

		ProfilerThreadCollector* pCollector =static_cast<ProfilerThreadCollector*>(cache_collector_.malloc());
		ACE_ASSERT( pCollector != 0);

		try{
			ACE_thread_t tid =ACE_OS::thr_self();
			new( pCollector)ProfilerThreadCollector( tid, threadname, max_level_, active_profilingscope_);

			pCollector->regist_2_tls_profileruserdata();
		}
		catch( ...)
		{
			cache_collector_.free( (void*)pCollector);
			ACE_ASSERT( false);

			return 0;
		}

		return pCollector;
	}
}

void ProfilerAnalysis::stop_threadcollector()
{
	ProfilerThreadCollector* oldcollector =ProfilerThreadCollector::get_profiler_userdata();
	if( oldcollector == 0)
		return;

	ACE_OS::gettimeofday().to_usec( oldcollector->end_time_);
}

#endif	//ENABLE_PROFILING
