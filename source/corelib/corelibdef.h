/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CORELIBDEF__H__
#define __CORELIBDEF__H__

#include "ace/config.h"

#include <prolib/StaticString.h>

#ifdef CORELIB_WIN32
	#undef CORELIB_WIN32
#endif

#ifdef ACE_WIN32
	#define CORELIB_WIN32
#else
#endif

#define NAMESPACE_MEMORY_BEGIN		{
#define NAMESPACE_MEMORY_END		}

#define NAMESPACE_APPSCRIPT_BEGIN	namespace app { namespace script {
#define NAMESPACE_APPSCRIPT_END		} } 

#define NAMESPACE_APPUTIL_BEGIN		namespace app { namespace util {
#define NAMESPACE_APPUTIL_END		} }

#define CODE_BLOCK_BEGIN()	{
#define CODE_BLOCK_END()	}

#define CLASS_COPY_NOT_ALLOWED(ClassName)				\
	private:                                            \
		ClassName( const ClassName &);                  \
		const ClassName & operator=(const ClassName &)

#define OTL_STL // Turn on STL features
#define OTL_ANSI_CPP // Turn on ANSI C++ typecasts
#include "corelib/db/otlv4.h"

#include "corelib/memory/CoreMemory.h"

#endif	//__CORELIBDEF__H__
