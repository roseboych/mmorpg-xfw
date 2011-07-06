/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __TLSCONTEXT__H__
#define __TLSCONTEXT__H__

#include "corelib/corelibdef.h"
#include <prolib/core_type.h>
#include <ace/Thread_Mutex.h>

/**
* @class TLSContext
*
* @brief tls的封装类，为所以的线程提供统一支持
**/
class TLSContext
{
public:
	TLSContext();
	~TLSContext();

	/**
	* 初始化tls的基本支持
	**/
	static bool init_tls_support();
	static void uninit_tls_support();

	/**
	* 初始化具体的线程tls
	**/
	bool init_tlscontext();
	void uninit_tlscontext();

	/**
	* 设置和获取performance的用户定义数据
	**/
	static void* get_profiler_userdata();
	static void set_profiler_userdata( void* p);

	/**
	* 设置和获取应用的用户定义数据
	**/
	static void* get_app_userdata();
	static void set_app_userdata( void* p);

private:
	void*	profiler_userdata_;
	void*	app_userdata_;

	static ACE_thread_key_t	tls_key_;
};

#endif	//__TLSCONTEXT__H__
