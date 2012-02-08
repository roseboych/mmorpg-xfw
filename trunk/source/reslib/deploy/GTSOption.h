/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSOPTION__H__
#define __GTSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <string>

/**
* @class GTSOption
* 
* @brief 网关服务器配置
**/
class GTSOption
{
public:
	GTSOption();
	~GTSOption();

	/**
	* 检查globalindex是否在gts内
	* @param gid
	* @return 
	**/
	bool is_userglobalin( int gid);

public:
	//gts的编号
	int				sn_;
	//服务器地址和端口
	NS_STL::string	ip_;
	int				port_;
	//开始索引号
	int		startuser_;
	//玩家数量
	int		usernum_;

	//系统分配的序列号
	int		server_index_;
};

#endif	//__GTSOPTION__H__
