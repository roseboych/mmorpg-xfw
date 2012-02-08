/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DBCONFIG__H__
#define __DBCONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include <string>

/**
* @struct DBConfig
* 
* @brief 数据库配置描述
**/
struct DBConfig
{
	//用户名口令
	NS_STL::string dbuser_;
	NS_STL::string dbpwd_;
	//数据库名字
	NS_STL::string dbname_;

	//系统配置
	int		dbthreadnum_;
	int		dbloopstep_;
};

#endif	//__DBCONFIG__H__
