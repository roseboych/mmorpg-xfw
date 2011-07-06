/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __FILECONFIGSOURCEIMPL__H__
#define __FILECONFIGSOURCEIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <corelib/mconf/IConfigContentSource.h>

/**
* @class FileConfigSourceImpl
* 
* @brief 配置模块数据源本地文件实现
**/
class FileConfigSourceImpl : public IConfigContentSource
{
public:
	FileConfigSourceImpl();
	~FileConfigSourceImpl();

public:
	//-----------------------implement IConfigContentSource------------------------
	/**
	* @see void IConfigContentSource::init_source( NS_STL::map< NS_STL::string, NS_STL::string> prop)
	* FileConfigSourceImpl支持的属性
	* dirbase:	配置文件根目录
	**/
	virtual bool init_source( NS_STL::map< NS_STL::string, NS_STL::string> prop);

	/**
	* @see void IConfigContentSource::uninit_source()
	**/
	virtual void uninit_source();

	/**
	* @see NS_STL::string IConfigContentSource::get_txtfilecontent( const char* path, const char* filename)
	**/
	virtual NS_STL::string get_txtfilecontent( const char* path, const char* filename);

	/**
	* @see void IConfigContentSource::get_filelist( const char* path, const char* ext, NS_STL::list< NS_STL::string>& ret)
	**/
	virtual void get_filelist( const char* path, const char* ext, NS_STL::list< NS_STL::string>& ret);

	/**
	* @see void IConfigContentSource::get_subdirlist( const char* path, NS_STL::list< NS_STL::string>& ret)
	**/
	virtual void get_subdirlist( const char* path, NS_STL::list< NS_STL::string>& ret);

protected:


private:
	//配置信息根目录
	NS_STL::string basedir_;
};

#endif	//__FILECONFIGSOURCEIMPL__H__
