/**
* corelib
*
* @category		memory manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __MALLOCBASE__H__
#define __MALLOCBASE__H__

#define CORELIB_UNUSED(pv)	((void)&pv)

/**
* @class MallocBase
* 
* @brief define memory manage interface
**/
class MallocBase
{
public:
	MallocBase(){}
	//No destrutor here to keep VPT after child destruct.

	virtual void* malloc( size_t size, const char* tag, int _malloc_id = 0,
		bool usepool = true, const char* file = 0, int line = 0) =0;

	virtual void* realloc( void* original, size_t size, const char* tag, 
		int _malloc_id = 0, bool usepool = true,
		const char* file = 0, int line = 0) =0;

	virtual void free( void* original) =0;

	virtual void outofmemory( size_t request_size ) =0;
	virtual void dump_malloc() =0;
	virtual void heap_check() =0;
	virtual void init() =0;
	virtual void exit() =0;
	virtual int  allocated_memory() { return 0;}
	virtual bool is_owner( void* old, size_t* psize ) =0;

	//Tag context can be used for memory statistics collection
	virtual void push_tagcontext( const char* tag ){
		CORELIB_UNUSED(tag);
	}

	virtual void pop_tagcontext(){}
};

#endif	//__MALLOCBASE__H__
