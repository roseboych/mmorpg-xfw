/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGSOPTION__H__
#define __RGSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include "reslib/deploy/DBConfig.h"

#include <list>
#include <string>
#include <map>

/**
* @class RGSOption
* 
* @brief RGS服务器配置
**/
class RGSOption
{
	typedef NS_STL::map< NS_STL::string, NS_STL::string>	CONTROLLER_USERS_MAP;
public:
	RGSOption(void);

	/**
	* 验证controller的用户名和密码
	* @param name
	* @param pwd
	* @return
	**/
	bool has_privilege( NS_STL::string name, NS_STL::string pwd);

public:
	NS_STL::string ip_;
	int port_;
	DBConfig	dbinfo_;

	//controller配置信息
	int controller_port_;
	CONTROLLER_USERS_MAP	controller_users_;

	//最大玩家数
	int max_users_;
};

#endif	//__RGSOPTION__H__
