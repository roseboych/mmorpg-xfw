/**
* corelib
*
* @category		profiler
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PROFILER_DEFINE__H__
#define __PROFILER_DEFINE__H__

//是否支持profiler性能分析
#ifndef ENABLE_PROFILING
	#define ENABLE_PROFILING
#endif

#include <ace/config.h>
#include <ace/Basic_Types.h>
#include <prolib/core_type.h>

//profiler中字符串的最大长度
#define	PROFILER_MAX_NAME_LENGTH_	128
//一个ThreadCollector能够构造的最大数据包
#define PROFILER_UDPPACKAGE_MAX_	1024*64	//64k

enum EProfileScope
{
	//一次循环总的花费时间，在thread循环处设置使用
	EProfileScope_Summary		 = 0xFFFFFFFF,
	//对网络协议处理的scope
	EProfileScope_NetCommand	 = 0x00000001,
	EProfileScope_HighLevel      = 0x00000002,
	EProfileScope_LowLevel       = 0x00000004,
	EProfileScope_All            = 0xFFFFFFFF,
};

enum EProfileEventType
{
	//event开始
	EProfileEventType_EventBegin,
	//event结束
	EProfileEventType_EventEnd,
};

//定义了profiler收集器的线程类型
enum EProfileThreadType
{
	//主线程
	EProfileThreadType_Main		= 0x00000001,
	//数据库线程
	EProfileThreadType_Database	= 0x00000002,
	//内容服务线程
	EProfileThreadType_Content	= 0x00000004,
	//所有线程
	EProfileThreadType_All		= 0xFFFFFFFF,
};

#ifdef ENABLE_PROFILING

/**
* @class Profiler_EventData
*
* @brief 一条event的信息，用来记录性能
**/
struct Profiler_EventData
{
public:
	explicit Profiler_EventData( unsigned long profileId, EProfileEventType type, ACE_UINT64 time):
		profile_id_( profileId), time_( time), type_( type){}

public:
	//profiler名字生成的crc值作为id
	unsigned long		profile_id_;
	//时间戳
	ACE_UINT64			time_;
	//类型
	EProfileEventType	type_;
};

//-------------------------------------------------profile macro define----------------------------------------------//

#define PROFILER_THREAD_TAG( str, threadtype) \
	ProfilerThreadTags __profiler_thread_tag( str, threadtype);

#define PROFILER_ADDEVENT( profileid, profilescope)	\
	static S_ULONG_32 __profile_crc =CRCUtil::create_crcnocase( profileid);	\
	ProfilerTags __profiler_tags( profileid, __profile_crc, profilescope);

#else	//not define ENABLE_PROFILING

	#define	PROFILER_THREAD_TAG( str, threadtype)
	#define PROFILER_ADDEVENT( profileid, profilescope)


#endif	//ENABLE_PROFILING

#endif	//__PROFILER_DEFINE__H__
