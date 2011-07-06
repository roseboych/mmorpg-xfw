/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "corelib/profiler/ProfilerThreadCollector.h"

#include "corelib/core/TLSContext.h"
#include "corelib/util/CRCUtil.h"

#include <ace/OS_NS_sys_time.h>
#include <ace/OS_NS_string.h>

#ifdef ENABLE_PROFILING

ProfilerThreadCollector::ProfilerThreadCollector( int thid, const char* name, int maxlevel, EProfileScope sc):
thread_id_( thid),
thread_name_( name),
max_level_( maxlevel),
active_profilescope_( sc)
{
	current_level_ =0;

	ACE_OS::gettimeofday().to_usec( start_time_);
	end_time_ =start_time_;
}

void ProfilerThreadCollector::regist_2_tls_profileruserdata()
{
	TLSContext::set_profiler_userdata( (void*)this);
}

ProfilerThreadCollector* ProfilerThreadCollector::get_profiler_userdata()
{
	return (ProfilerThreadCollector*)TLSContext::get_profiler_userdata();
}

bool ProfilerThreadCollector::is_validevent( EProfileScope sc, EProfileEventType etype)
{
	//test scope valid
	if( (active_profilescope_ & sc) > 0)
		return false;

	//==-1 means no limited on level requirement
	if( max_level_ != -1 && current_level_ > max_level_)
		return false;

	return true;
}

void ProfilerThreadCollector::add_event( const char* profileid, unsigned long profileid_crc, EProfileEventType etype, EProfileScope sc)
{
	if( etype == EProfileEventType_EventBegin)
		++current_level_;
	else
		--current_level_;

	if( !is_validevent( sc, etype))
		return;

	if( etype == EProfileEventType_EventBegin)
		profile_table_[profileid_crc] =profileid;

	ACE_UINT64 tnow;
	ACE_OS::gettimeofday().to_usec( tnow);

	Profiler_EventData ev( profileid_crc, etype, tnow);
	event_data_.push_back( ev);
}

bool ProfilerThreadCollector::is_timeout()
{
	ACE_OS::gettimeofday().to_usec( end_time_);
	//10 msecond
	return ( start_time_ + 1000*10 <= end_time_);
}

int ProfilerThreadCollector::serialize_collector( char* buf)
{
	if( buf == 0)
		return 0;

	int cur_pos =0;
	char* ptr =buf;

#define TEMP_INT8_ASSIGN( val)	\
	cur_pos += sizeof( S_INT_8);	\
	ACE_ASSERT( cur_pos < PROFILER_UDPPACKAGE_MAX_);	\
	*((S_INT_8*)ptr) =val;	\
	ptr =buf + cur_pos;

#define TEMP_INT32_ASSIGN( val)	\
	cur_pos += sizeof( S_INT_32);	\
	ACE_ASSERT( cur_pos < PROFILER_UDPPACKAGE_MAX_);	\
	*((S_INT_32*)ptr) =val;	\
	ptr =buf + cur_pos;

#define TEMP_ULONG32_ASSIGN( val)	\
	cur_pos += sizeof( S_ULONG_32);	\
	ACE_ASSERT( cur_pos < PROFILER_UDPPACKAGE_MAX_);	\
	*((S_ULONG_32*)ptr) =val;	\
	ptr =buf + cur_pos;

#define TEMP_TIME_ASSIGN( val)	\
	cur_pos += sizeof( ACE_UINT64);	\
	ACE_ASSERT( cur_pos < PROFILER_UDPPACKAGE_MAX_);	\
	*((ACE_UINT64*)ptr) =val;	\
	ptr =buf + cur_pos;

#define TEMP_STR_ASSIGN( val, len)	\
	cur_pos += PROFILER_MAX_NAME_LENGTH_;	\
	ACE_ASSERT( cur_pos < PROFILER_UDPPACKAGE_MAX_);	\
	ACE_OS::memcpy( ptr, val, len);	\
	ptr =buf + cur_pos;

	//put thread information
	TEMP_INT32_ASSIGN( thread_id_)
	TEMP_STR_ASSIGN( thread_name_.c_str(), thread_name_.size())
	TEMP_TIME_ASSIGN( start_time_)
	TEMP_TIME_ASSIGN( end_time_)

	//put profile event head information
	int s1 =(int)profile_table_.size();
	TEMP_INT32_ASSIGN( s1)

	{
		NS_STL::map< S_ULONG_32, NS_STL::string>::iterator iter =profile_table_.begin(), eiter =profile_table_.end();
		for( ; iter != eiter; ++iter)
		{
			S_ULONG_32 pid =iter->first;
			NS_STL::string& str =iter->second;

			TEMP_ULONG32_ASSIGN( pid)
			TEMP_STR_ASSIGN( str.c_str(), str.size())
		}
	}

	//put profile information
	s1 =(int)event_data_.size();
	TEMP_INT32_ASSIGN( s1)

	{
		NS_STL::list<Profiler_EventData>::iterator iter =event_data_.begin(), eiter =event_data_.end();
		for( ; iter != eiter; ++iter)
		{
			Profiler_EventData& dat =(*iter);

			TEMP_ULONG32_ASSIGN( dat.profile_id_)
			TEMP_TIME_ASSIGN( dat.time_)
			TEMP_INT8_ASSIGN( dat.type_)
		}
	}

	return cur_pos;
}

#endif	//ENABLE_PROFILING
