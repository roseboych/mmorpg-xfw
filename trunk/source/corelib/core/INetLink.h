/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __INETLINK__H__
#define __INETLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/BasicProtocol.h>

/**
* @class INetLink
* 
* @brief 服务器连接的接口类
**/
class INetLink
{
public:

	/**
	* 发送协议
	* @param p
	**/
	virtual void send_protocol( BasicProtocol* p) =0;

	/**
	* 获取link的索引
	**/
	virtual int get_serverindex() =0;
};

#endif	//__INETLINK__H__
