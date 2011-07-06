/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILERNETSTREAM__H__
#define __PROFILERNETSTREAM__H__

#include "corelib/corelibdef.h"
#include "corelib/profiler/profiler_define.h"

#include <ace/SOCK_CODgram.h>

#ifdef ENABLE_PROFILING

class ProfilerThreadCollector;

/**
* @class ProfilerNetStream
*
* @brief 通过udp发送的序列化器
**/
class ProfilerNetStream
{
public:
	ProfilerNetStream();
	~ProfilerNetStream();

	bool init_netstream( NS_STL::string ip, int port);
	void uninit_netstream();

	void serialize( ProfilerThreadCollector* pcollector);

private:
	NS_STL::string	net_ip_;
	int				net_port_;

	ACE_SOCK_CODgram	analysis_socket_;
	bool				analysis_socket_initialized_;

	//udp buffer
	char	udp_buffer_[PROFILER_UDPPACKAGE_MAX_];
};

#endif	//ENABLE_PROFILING

#endif	//__PROFILERNETSTREAM__H__
