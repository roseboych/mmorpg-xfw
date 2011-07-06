/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "corelib/profiler/ProfilerNetStream.h"

#include "corelib/profiler/ProfilerThreadCollector.h"

#ifdef ENABLE_PROFILING

ProfilerNetStream::ProfilerNetStream():
analysis_socket_initialized_( false)
{
}

ProfilerNetStream::~ProfilerNetStream()
{
	uninit_netstream();
}

bool ProfilerNetStream::init_netstream( NS_STL::string ip, int port)
{
	this->uninit_netstream();

	this->net_ip_ =ip;
	this->net_port_ =port;

	ACE_INET_Addr remote_addr( net_port_, net_ip_.c_str());

	analysis_socket_initialized_ =( analysis_socket_.open( remote_addr) == 0);

	return analysis_socket_initialized_;
}

void ProfilerNetStream::uninit_netstream()
{
	analysis_socket_initialized_ =false;
	analysis_socket_.close();
}

void ProfilerNetStream::serialize( ProfilerThreadCollector* pcollector)
{
	if( !analysis_socket_initialized_ || pcollector == 0)
		return;

	int cnt =pcollector->serialize_collector( &(udp_buffer_[0]));

	analysis_socket_.send( udp_buffer_, cnt);
}

#endif	//ENABLE_PROFILING
