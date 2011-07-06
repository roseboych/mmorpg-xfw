/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/corelibdef.h"

//#include "corelib/memory/MallocBase.h"
#include "corelib/memory/MallocGeneral.h"
#include "corelib/memory/AppACEAllocator.h"

//global member for memory manage
MallocBase*	g_malloc =0;

void malloc_init()
{
	static MallocGeneral s_malloc;

	g_malloc =&s_malloc;
	g_malloc->init();

	//use self defined ace allocator to replace default allocator of the ACE library
	static AppACEAllocator	s_app_ace_allocator;
	ACE_Allocator::instance( &s_app_ace_allocator);
}

#if defined(CORELIB_WIN32)
//force link to new operator
namespace PrivateUsage
{
	void _force_link_to_new_operator()
	{
		int* p1 =new int;
		::operator delete( p1);
		
		int* p2 =new("onlyforlink") int;
		::operator delete( p2, "onlyforlink");
		
		int* p3 =new("onlyforlink", MallocTypeID::MTID_DEFAULT, true) int;
		::operator delete( p3, "onlyforlink", MallocTypeID::MTID_DEFAULT, true);

		int* p4 =new int[1];
		::operator delete[] (p4);

		int* p5 =new("onlyforlink") int[1];
		::operator delete[] ( p5, "onlyforlink");
		
		int* p6 =new("onlyforlink", MallocTypeID::MTID_DEFAULT, true) int[1];
		::operator delete[] ( p6, "onlyforlink", MallocTypeID::MTID_DEFAULT, true);
	}
}

#endif
