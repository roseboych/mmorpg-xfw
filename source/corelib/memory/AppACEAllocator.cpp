/**
* corelib
*
* @category		memory manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/memory/AppACEAllocator.h"

#include "corelib/corelibdef.h"

#include <ace/ACE.h>
#include <ace/OS_NS_errno.h>
#include <ace/OS_NS_string.h>

void* AppACEAllocator::malloc( size_t nbytes)
{
	char *ptr = 0;

	if( nbytes > 0)
	{
		ptr =ACELIB_NEW char[nbytes];
		if( ptr == 0)
		{
			errno = ENOMEM;
			return 0;
		}
	}

	return (void *)ptr;
}

void* AppACEAllocator::calloc( size_t nbytes, char initial_value)
{
	char *ptr = 0;

	ptr =ACELIB_NEW char[nbytes];
	if( ptr == 0)
	{
		errno = ENOMEM;
		return 0;
	}

	ACE_OS::memset( ptr, initial_value, nbytes);

	return (void *) ptr;
}

void* AppACEAllocator::calloc( size_t n_elem, size_t elem_size, char initial_value)
{
	return AppACEAllocator::calloc( n_elem * elem_size, initial_value);
}

void AppACEAllocator::free( void *ptr)
{
	delete [] (char *) ptr;
}

//---------------------------------------------These methods are no-ops.-------------------------------------------

int AppACEAllocator::remove( void)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::bind( const char *, void *, int)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::trybind( const char *, void *&)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::find( const char *, void *&)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::find( const char *)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::unbind( const char *)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::unbind( const char *, void *&)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::sync( ssize_t, int)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::sync( void *, size_t, int)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::protect( ssize_t, int)
{
	ACE_NOTSUP_RETURN( -1);
}

int AppACEAllocator::protect( void *, size_t, int)
{
	ACE_NOTSUP_RETURN( -1);
}

#if defined (ACE_HAS_MALLOC_STATS)

void AppACEAllocator::print_stats( void) const
{
}

#endif /* ACE_HAS_MALLOC_STATS */

void AppACEAllocator::dump( void) const
{
#if defined (ACE_HAS_DUMP)
#endif /* ACE_HAS_DUMP */
}
