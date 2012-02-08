/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPXOPTION__H__
#define __DPXOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include "reslib/deploy/DBConfig.h"

#include <string>

/**
* @class DPXOption
* 
* @brief 数据库代理服务器配置
**/
class DPXOption
{
public:
	DPXOption(void);
	~DPXOption(void);

public:
	//服务器地址和端口
	NS_STL::string	ip_;
	int				port_;

	//玩家数量
	int			maxuser_;

	//数据库配置
	DBConfig	db_;
};

#endif	//__DPXOPTION__H__
