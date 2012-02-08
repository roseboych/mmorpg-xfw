/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGSOPTION__H__
#define __LGSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include <string>

/**
* @class LGSOption
* 
* @brief LGS服务器配置
**/
class LGSOption
{
public:
	LGSOption();

public:
	int				sn_;
	NS_STL::string	ip_;
	int				port_;
	//支持的最大玩家数
	int				maxuser_;

	//系统分配的序列号
	int				server_index_;
};

#endif	//__LGSOPTION__H__
