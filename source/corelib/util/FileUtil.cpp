/**
* corelib
*
* @category		util module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/util/FileUtil.h"

#include <boost/filesystem.hpp>
#include <istream>

NS_STL::string FileUtil::get_filecontent( const char* path, const char* file)
{
	if( !boost::filesystem::is_directory( path))
		return "";

	boost::filesystem::path p( path);
	p /= file;

	if( !boost::filesystem::exists( p))
		return "";

	NS_STL::string ret ="";

	NS_STL::ifstream ff;
	ff.open( p.string().c_str());

	char buf[1024*100];
	while( !ff.eof())
	{
		ff.read( &buf[0], 1024*100 -1);
		int cnt =ff.gcount();
		buf[cnt] ='\0';

		ret += buf;
	}

	return ret;
}

bool FileUtil::is_direxist( const char* path)
{
	if( path == 0)
		return false;

	if( !boost::filesystem::is_directory( path))
		return false;

	return true;
}