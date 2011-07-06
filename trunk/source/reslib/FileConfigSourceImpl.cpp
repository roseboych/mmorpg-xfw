/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/FileConfigSourceImpl.h"

#include <corelib/util/FileUtil.h>

FileConfigSourceImpl::FileConfigSourceImpl(void)
{
}

FileConfigSourceImpl::~FileConfigSourceImpl(void)
{
}

bool FileConfigSourceImpl::init_source( NS_STL::map< NS_STL::string, NS_STL::string> prop)
{
	//设置配置文件根目录
	basedir_ =prop["dirbase"];
	if( basedir_.size() == 0)
		return false;

	char ec =basedir_[basedir_.size()-1];
	if( ec != '/' && ec != '\\' )
		basedir_ += "/";

	if( !FileUtil::is_direxist( basedir_.c_str()))
		return false;

	return true;
}

void FileConfigSourceImpl::uninit_source()
{

}

NS_STL::string FileConfigSourceImpl::get_txtfilecontent( const char* path, const char* filename)
{
	NS_STL::string p =basedir_;

	if( path != NULL)
	{
		p += path;
		p += "/";
	}

	return FileUtil::get_filecontent( p.c_str(), filename);
}

void FileConfigSourceImpl::get_filelist( const char* path, const char* ext, NS_STL::list< NS_STL::string>& ret)
{
	
}

void FileConfigSourceImpl::get_subdirlist( const char* path, NS_STL::list< NS_STL::string>& ret)
{

}
