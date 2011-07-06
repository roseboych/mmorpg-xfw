/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILERTHREADCOLLECTOR__H__
#define __PROFILERTHREADCOLLECTOR__H__

#include "corelib/corelibdef.h"

#include <prolib/core_type.h>
#include <ace/Basic_Types.h>

#include "corelib/profiler/profiler_define.h"
#include <string>
#include <list>
#include <map>

#ifdef ENABLE_PROFILING

/**
* @class ProfilerThreadCollector
*
* @brief profiler的线程收集器
**/
class ProfilerThreadCollector
{
public:
	ProfilerThreadCollector( int thid, const char* name, int maxlevel =-1, EProfileScope sc =EProfileScope_All);

	/**
	* 注册到tls
	**/
	void regist_2_tls_profileruserdata();

	/**
	* 是否收集指定的event
	* @param sc
	* @param etype
	* @return
	**/
	bool is_validevent( EProfileScope sc, EProfileEventType etype);

	/**
	* 增加一个event
	* @param profileid
	* @param profileid_crc
	* @param etype
	* @param sc
	**/
	void add_event( const char* profileid, unsigned long profileid_crc, EProfileEventType etype, EProfileScope sc);

	/**
	* 检查是否collector超过了收集时间段
	* 缺省设置为20ms作为一个收集段
	* @return
	**/
	bool is_timeout();

	/**
	* 序列化当前collector到内存，用来存储或者发送
	* 一个collector作为一个分析数据单元
	* @param buf
	* @return buf的实际长度
	**/
	int serialize_collector( char* buf);

	/**
	* 获取当前线程tls中存储的collector对象
	* @return
	**/
	static ProfilerThreadCollector* get_profiler_userdata();

public:
	//线程id
	int				thread_id_;
	//线程名字
	NS_STL::string	thread_name_;

	//开始和结束时间，用来标识统计的时间段
	ACE_UINT64		start_time_;
	ACE_UINT64		end_time_;

private:
	//保存统计的profiler名字
	NS_STL::map< unsigned long, NS_STL::string>	profile_table_;
	//保存所有的event，按照执行的先后顺序存储
	NS_STL::list<Profiler_EventData>	event_data_;

	//当前的event深度
	int		current_level_;
	//collector支持的最大深度，可以通过远程控制器来设置
	int		max_level_;

	//当前支持的scope，可以通过远程控制器来设置
	EProfileScope	active_profilescope_;
};

#endif	//ENABLE_PROFILING

#endif	//__PROFILERTHREADCOLLECTOR__H__
