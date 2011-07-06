/**
* corelib
*
* @category		util module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/util/ShareUtil.h"

#include <ace/OS_NS_string.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_sys_time.h>

bool ShareUtil::splitstr2str( const char* src, const char* splitchr, NS_STL::vector<NS_STL::string>& ret)
{
	if( src == 0 || ACE_OS::strlen( src) == 0)
		return false;
	if( splitchr == 0 || ACE_OS::strlen( splitchr) == 0)
		return false;

	char* token =0;
	char* s =(char*)src;

	token =ACE_OS::strtok( s, splitchr);
	while( token != NULL)
	{
		ret.push_back( token);

		token =ACE_OS::strtok( NULL, splitchr);
	}

	return true;
}

bool ShareUtil::splitstr2int( const char* src, const char* splitchr, NS_STL::vector<int>& ret)
{
	if( src == 0 || ACE_OS::strlen( src) == 0)
		return false;
	if( splitchr == 0 || ACE_OS::strlen( splitchr) == 0)
		return false;

	char* token =0;
	char* s =(char*)src;

	token =ACE_OS::strtok( s, splitchr);
	while( token != NULL)
	{
		int v =ACE_OS::atoi( token);
		ret.push_back( v);

		token =ACE_OS::strtok( NULL, splitchr);
	}

	return true;
}

NS_STL::string ShareUtil::combineintstr( int v1, const char* v2, int radix)
{
	char buf[16];
	NS_STL::string ret =ACE_OS::itoa( v1, buf, radix);
	ret += v2;

	return ret;
}

NS_STL::string ShareUtil::combinestrint( const char* leftv, int rightv, int radix)
{
	NS_STL::string ret( leftv);

	char buf[16];
	ret += ACE_OS::itoa( rightv, buf, radix);

	return ret;
}

int ShareUtil::atoi( const char* str, int radix)
{
	char *endptr =0;
	return (int)ACE_OS::strtol( str, &endptr, radix);
}
