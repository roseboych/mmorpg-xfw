/**
* corelib
*
* @category		global config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ICONFIGCONTENTSOURCE__H__
#define __ICONFIGCONTENTSOURCE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"

#include <string>
#include <list>
#include <map>

/**
* @class IConfigContentSource
* 
* @brief 配置模块数据源接口
**/
class IConfigContentSource
{
public:
	/**
	* 初始化数据源
	* @param prop
	**/
	virtual bool init_source( NS_STL::map< NS_STL::string, NS_STL::string> prop) =0;
	virtual void uninit_source() =0;
	
	/**
	* 根据指定的路径和文件名获取文本文件内容
	*
	* @param path
	* @param filename
	* @return
	**/
	virtual NS_STL::string get_txtfilecontent( const char* path, const char* filename) =0;

	/**
	* 从指定的路径和指定的扩展名获取文件名称列表
	* @param path
	* @param ext
	* @param ret
	**/
	virtual void get_filelist( const char* path, const char* ext, NS_STL::list< NS_STL::string>& ret) =0;

	/**
	* 从指定的路径获取子目录名称列表
	* @param path
	* @param ret
	**/
	virtual void get_subdirlist( const char* path, NS_STL::list< NS_STL::string>& ret) =0;

};

#endif	//__ICONFIGCONTENTSOURCE__H__
