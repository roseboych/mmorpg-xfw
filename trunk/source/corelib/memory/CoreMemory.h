/**
* corelib
*
* @category		memory manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __COREMEMORY__H__
#define __COREMEMORY__H__

//class MallocBase;
#include "corelib/memory/MallocBase.h"

//pre declare malloc initialize function
extern void malloc_init();
extern MallocBase*	g_malloc;

//define all memory categories which be supported by memory manage
namespace MallocTypeID
{
	enum MallocTypeID
	{
		MTID_DEFAULT =0,
		MTID_ACELIB,
		MTID_LOG4XX,
		MTID_STATICRESOURCE,
		MTID_FRAMEWORK,
		MTID_TASKCMD,
		MTID_DATABASE,
		MTID_PROTOCOL,
		MTID_MAX
	};
}

#define ACELIB_NEW		new( "AppACEAllocator", MallocTypeID::MTID_ACELIB, true)
#define STATICRES_NEW	new( "static resource", MallocTypeID::MTID_STATICRESOURCE, true)
#define FRAMEWK_NEW		new( "framework", MallocTypeID::MTID_FRAMEWORK, true)
#define TASKCMD_NEW		new( "task cmd", MallocTypeID::MTID_TASKCMD, true)
#define DATABASE_NEW	new( "database", MallocTypeID::MTID_DATABASE, true)
#define PROTOCOL_NEW	new( "protocol", MallocTypeID::MTID_PROTOCOL, true)

#define app_malloc( size, tag)									ProtectUsage::xx_malloc_internal( size, tag, MallocTypeID::MTID_DEFAULT, true, __FILE__, __LINE__)
#define app_malloc_ext( size, tag, mallocid, usepool)			ProtectUsage::xx_malloc_internal( size, tag, mallocid, usepool, __FILE__, __LINE__)
#define app_realloc( pold, size, tag)							ProtectUsage::xx_realloc_internal( pold, size, tag, MallocTypeID::MTID_DEFAULT, true, __FILE__, __LINE__)
#define app_realloc_ext( pold, size, tag, mallocid, usepool)	ProtectUsage::xx_realloc_internal( pold, size, tag, mallocid, usepool, __FILE__, __LINE__)
#define app_free( ptr)											ProtectUsage::xx_free( ptr)

namespace ProtectUsage
{
	/**
	* @function xx_malloc_internal
	* 
	* @brief wrap malloc 
	**/
	inline void* xx_malloc_internal( size_t size, const char* tag, int mallocid, bool usepool, const char* filename, int line)
	{
		if( g_malloc == 0)
			::malloc_init();

		return g_malloc->malloc( size, tag, mallocid, usepool, filename, line);
	}

	/**
	* @function xx_realloc_internal
	* 
	* @brief wrap malloc 
	**/
	inline void* xx_realloc_internal( void* org, size_t size, const char* tag, int mallocid, bool usepool, const char* file, int line)
	{
		if( g_malloc == 0)
			::malloc_init();

		return g_malloc->realloc( org, size, tag, mallocid, usepool, file, line);
	}

	/**
	* @function xx_free
	* 
	* @brief wrap malloc 
	**/
	inline void xx_free( void* org)
	{
		if( g_malloc == 0)
			::malloc_init();

		g_malloc->free( org);
	}

}

//operator overload 
inline void* operator new( size_t size)
{
	return app_malloc( size, "new");
}

inline void* operator new( size_t size, char* tag)
{
	return app_malloc( size, tag);
}

inline void* operator new( size_t size, char* tag, int mallocid, bool usepool)
{
	return app_malloc_ext( size, tag, mallocid, usepool);
}

inline void operator delete( void* ptr)
{
	app_free( ptr);
}

inline void operator delete( void* ptr, char* tag)
{
	CORELIB_UNUSED( tag);

	app_free( ptr);
}

inline void operator delete( void* ptr, char* tag, int mallocid, bool usepool)
{
	CORELIB_UNUSED( tag);
	CORELIB_UNUSED( mallocid);
	CORELIB_UNUSED( usepool);

	app_free( ptr);
}

inline void* operator new[]( size_t size)
{
	return app_malloc( size, "array new");
}

inline void* operator new[]( size_t size, char* tag)
{
	return app_malloc( size, tag);
}

inline void* operator new[]( size_t size, char* tag, int mallocid, bool usepool)
{
	return app_malloc_ext( size, tag, mallocid, usepool);
}

inline void operator delete[]( void* ptr)
{
	app_free( ptr);
}

inline void operator delete[]( void* ptr, char* tag)
{
	CORELIB_UNUSED( tag);

	app_free( ptr);
}

inline void operator delete[]( void* ptr, char* tag, int mallocid, bool usepool)
{
	CORELIB_UNUSED( tag);
	CORELIB_UNUSED( mallocid);
	CORELIB_UNUSED( usepool);

	return app_free( ptr);
}

#endif	//__COREMEMORY__H__
