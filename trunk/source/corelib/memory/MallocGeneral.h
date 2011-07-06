/**
* corelib
*
* @category		memory manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __MALLOCGENERAL__H__
#define __MALLOCGENERAL__H__

#include "corelib/memory/CoreMemory.h"
#include "corelib/memory/MallocBase.h"

#include <malloc.h>

/**
* @class MallocGeneral
* 
* @brief general memory manage implementation
**/
class MallocGeneral : public MallocBase
{
public:
	typedef MallocBase inherit;

public:
	MallocGeneral();
	virtual ~MallocGeneral();

	void* malloc( size_t size, const char* tag, int _malloc_id = MallocTypeID::MTID_DEFAULT,
		bool usepool = true, const char* file = 0, int line = 0)
	{
		return ::malloc( size);
	}

	void* realloc( void* original, size_t size, const char* tag, 
		int _malloc_id = MallocTypeID::MTID_DEFAULT, bool usepool = true,
		const char* file = 0, int line = 0)
	{
		return ::realloc( original, size);
	}

	void free( void* original){
		::free( original);
	}

	void outofmemory( size_t request_size){

	}

	void dump_malloc(){

	}

	void heap_check(){

	}

	void init(){

	}

	void exit(){

	}
	
	int  allocated_memory(){
		return 0;
	}

	bool is_owner( void* old, size_t* psize){
		return false;
	}

	void push_tagcontext( const char* tag){

	}

	void pop_tagcontext(){

	}

protected:
	void config_general_malloc();
};


#endif	//__MALLOCGENERAL__H__
