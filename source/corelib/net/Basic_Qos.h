/**
* corelib
*
* @category		net module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASIC_QOS__H___
#define __BASIC_QOS__H___

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"

/**
* @class Basic_Qos
* 
* @brief 网络状况qos基类,BasicService支持qos的计算
* 
**/
class Basic_Qos
{
public:
	Basic_Qos(void){};
	virtual ~Basic_Qos(void){};

	/**
	* 开始写入
	**/
	virtual void start_write() =0;

	/**
	* 写入字节数统计
	* @param len	写入的长度
	**/
	virtual void write_ready( size_t len) =0;

	/**
	* 读取字节数统计
	* @param len	读取的长度
	**/
	virtual void read_ready( size_t len) =0;

	virtual int qos() =0;

	/**
	* 获取之后一次写或者读的时间msec
	* @return
	**/
	virtual ACE_UINT64 last_time() =0;
};

#endif	//__BASIC_QOS__H___
