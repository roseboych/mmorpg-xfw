/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTSOPTION__H__
#define __CTSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <string>

/**
* @class CTSOption
* 
* @brief 主控服务器配置
**/
class CTSOption
{
public:
	CTSOption();
	~CTSOption();

public:
	//服务器ip+port
	NS_STL::string ip_;
	int port_;

	//总的玩家数
	int totleusers_;
};

#endif	//__CTSOPTION__H__
