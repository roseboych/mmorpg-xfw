/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILERANALYSIS__H__
#define __PROFILERANALYSIS__H__

#include "corelib/corelibdef.h"

#include <prolib/core_type.h>

#include "corelib/profiler/profiler_define.h"
#include "corelib/profiler/ProfilerThreadCollector.h"
#include "corelib/profiler/ProfilerTags.h"
#include "corelib/profiler/ProfilerFileStream.h"
#include "corelib/profiler/ProfilerNetStream.h"

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>
#include <ace/Malloc_T.h>
#include <ace/Task.h>

#include <string>
#include <map>
#include <list>

#ifdef ENABLE_PROFILING

/**
* @class ProfilerAnalysis
*
* @brief profiler分析器，单实例对象
**/
class ProfilerAnalysis : public ACE_Task_Base
{
public:
	ProfilerAnalysis();
	~ProfilerAnalysis();

    EProfileScope get_activeprofilescope(){ return active_profilingscope_;}
    void set_activeprofilescope( EProfileScope profileScope){ active_profilingscope_ =profileScope;}

	EProfileThreadType get_activeprofilethreadtype(){ return active_profilethreadtype_;}
	void set_activeprofilethreadtype( EProfileThreadType p){ this->active_profilethreadtype_ =p;}
	
	int get_maxlevel(){ return max_level_;}
	void set_maxlevel( int m){ max_level_ =m;}

	/**
	* 线程函数
	**/
	virtual int svc();

	/**
	* 开始分析
	* @param net	是否通过udp发送
	* @param localfile	是否保存到本地文件
	* @param ip		net=true 目标ip地址
	* @param port	net=true 目标port
	*
	**/
	void start_analysis( bool net =true, bool localfile =false, NS_STL::string ip ="", int port =0);

	/**
	* 停止分析
	**/
	void stop_analysis();

	/**
	* 获取当前线程的收集器,并且开始收集
	* @param threadname	线程名字
	* @return
	**/
	ProfilerThreadCollector* start_threadcollector( const char* threadname);

	/**
	* 停止当前线程收集器的收集
	**/
	void stop_threadcollector();

	bool is_startanalysis( EProfileThreadType t);

private:
	//内存cache
	static ACE_Cached_Allocator<ProfilerThreadCollector, ACE_Null_Mutex>	cache_collector_;

	//等待保存或者发送的collector数据
	NS_STL::list< ProfilerThreadCollector*>	waiting_send_collector_;
	ACE_Thread_Mutex	threadcollector_mutex_;
	
	S_UINT_32		frequency_;

	//当前分析器的过滤条件
	EProfileThreadType	active_profilethreadtype_;
	EProfileScope	active_profilingscope_;
	int				max_level_;

	//分析开始时间
	ACE_UINT64		begin_time_;
	//是否开始分析
	bool			start_analysis_;

	//local file save solution
	bool			save_localfile_;
	ProfilerFileStream	local_filestream_;

	//net save solution
	bool			save_net_;
	ProfilerNetStream	remote_netstream_;
};

#define G_PROFILEANALYSIS ACE_Singleton< ProfilerAnalysis, ACE_Null_Mutex>::instance()

#endif	//ENABLE_PROFILING

#endif // __PROFILERANALYSIS__H__
