/**
* corelib
*
* @category		memory manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __APPACEALLOCATOR__H__
#define __APPACEALLOCATOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <ace/Malloc_Base.h>

class AppACEAllocator : public ACE_Allocator
{
public:
	/// These methods are defined.
	virtual void *malloc (size_t nbytes);
	virtual void *calloc (size_t nbytes, char initial_value = '\0');
	virtual void *calloc (size_t n_elem, size_t elem_size, char initial_value = '\0');
	virtual void free (void *ptr);

	/// These methods are no-ops.
	virtual int remove (void);
	virtual int bind (const char *name, void *pointer, int duplicates = 0);
	virtual int trybind (const char *name, void *&pointer);
	virtual int find (const char *name, void *&pointer);
	virtual int find (const char *name);
	virtual int unbind (const char *name);
	virtual int unbind (const char *name, void *&pointer);
	virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
	virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
	virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
	virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

#if defined (ACE_HAS_MALLOC_STATS)
	virtual void print_stats (void) const;
#endif /* ACE_HAS_MALLOC_STATS */
	virtual void dump (void) const;

private:
};

#endif	//__APPACEALLOCATOR__H__
