/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILERFILESTREAM__H__
#define __PROFILERFILESTREAM__H__

#include "corelib/corelibdef.h"
#include "corelib/profiler/profiler_define.h"
#include <prolib/core_type.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

#ifdef ENABLE_PROFILING

class ProfilerThreadCollector;

/**
* @class ProfilerFileStream
*
* @brief 本地文件的序列化存储器
**/
class ProfilerFileStream
{
public:
	ProfilerFileStream();
	~ProfilerFileStream();

public:
	bool init_filestream();
	void uninit_filestream();

	void serialize( ProfilerThreadCollector* pcollector);

private:
	NS_STL::string	local_file_;

	NS_STL::ofstream	file_stream_;
	bool	file_stream_initialized_;

	char	block_buffer_[PROFILER_UDPPACKAGE_MAX_];
};

#endif	//ENABLE_PROFILING

#endif	//__PROFILERFILESTREAM__H__
