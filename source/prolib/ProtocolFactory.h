/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PROTOCOLFACTORY__H__
#define __PROTOCOLFACTORY__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/BasicProtocol.h"

/**
* @class ProtocolFactory
* 
* @brief 通讯协议类工厂、所有定义的通讯协议必须在该类中注册。
* 改类不能保存自己的中间状态
**/
class ProtocolFactory
{
public:
	ProtocolFactory();

public:
	/**
	* 协议类工厂
	* 
	* @param pdata		协议数据块开始地址
	* @param len		数据块长度
	* @param ext		扩展信息,保留
	* @param internals	true:构造的是来自服务器组内部的协议 false:构造的是来自客户端的协议
	**/
	virtual BasicProtocol* data_protocol( char *pdata, S_INT_32 len, 
		void* ext =0, bool internals =true);
};

#endif //__PROTOCOLFACTORY__H__
