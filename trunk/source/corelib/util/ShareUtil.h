/**
* corelib
*
* @category		util module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SHAREUTIL__H__
#define __SHAREUTIL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"

#include "ace/Time_Value.h"

#include <list>
#include <vector>

/**
* @class ShareUtil
* 
* @brief 常用工具函数封装
**/
class ShareUtil
{
private:
	ShareUtil(){}

public:
	/**
	* 分割字符串
	* @param src
	* @param splitchr
	* @param ret
	* @return
	**/
	static bool splitstr2int( const char* src, const char* splitchr, NS_STL::vector<int>& ret);

	/**
	* 合并int值和str值为一个string
	* @param v1
	* @param v2
	* @return 
	**/
	static NS_STL::string combineintstr( int v1, const char* v2, int radix= 10);
	static NS_STL::string combinestrint( const char* leftv, int rightv, int radix =10);

	static int atoi( const char* str, int radix= 10);

	/**
	* 拆分字符串
	* @param src
	* @param splitchr
	* @param ret
	* @return
	**/
	static bool splitstr2str( const char* src, const char* splitchr, NS_STL::vector<NS_STL::string>& ret);

};

#endif	//__SHAREUTIL__H__
