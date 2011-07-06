/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "corelib/profiler/ProfilerFileStream.h"

#include "corelib/profiler/ProfilerThreadCollector.h"
#include <ace/OS_NS_time.h>

#ifdef ENABLE_PROFILING

ProfilerFileStream::ProfilerFileStream():
file_stream_initialized_( false)
{
}

ProfilerFileStream::~ProfilerFileStream()
{
	uninit_filestream();
}

bool ProfilerFileStream::init_filestream()
{
	uninit_filestream();

	{
		struct tm* tnow =ACE_OS::localtime(0);
		char buf[128] ={'\0',};
		ACE_OS::strftime( buf, 128, "%m%d_%H%M%S.profiler", tnow);
		local_file_ =buf;
	}

	NS_STL::string upath( "./");
	upath += local_file_;

	file_stream_.open( upath.c_str(), NS_STL::ios_base::out|NS_STL::ios_base::binary|NS_STL::ios_base::trunc);

	if( !file_stream_.is_open() || file_stream_.bad())
		file_stream_initialized_ =false;
	else
		file_stream_initialized_ =true;

	return file_stream_initialized_;
}

void ProfilerFileStream::uninit_filestream()
{
	file_stream_initialized_ =false;

	if( file_stream_.is_open())
		file_stream_.close();
}

void ProfilerFileStream::serialize( ProfilerThreadCollector* pcollector)
{
	if( !file_stream_initialized_ || pcollector == 0)
		return;

	int cnt =pcollector->serialize_collector( &(block_buffer_[0]));
	file_stream_.write( (char*)&cnt, sizeof( cnt));
	if( cnt > 0)
		file_stream_.write( block_buffer_, cnt);
	file_stream_ << '#';
}

#endif	//ENABLE_PROFILING
