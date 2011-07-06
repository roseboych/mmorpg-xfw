/**
* corelib
*
* @category		util module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __FILEUTIL__H__
#define __FILEUTIL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"

#include <string>

/**
* @class FileUtil
* 
* @brief 文件处理工具
**/
class FileUtil
{
private:
	FileUtil(){}

public:
	/**
	* 获取文件内容
	* @param path
	* @param file
	* @return
	**/
	static NS_STL::string get_filecontent( const char* path, const char* file);

	/**
	* 检查目录是否存在
	**/
	static bool is_direxist( const char* path);
};

#endif	//__FILEUTIL__H__
