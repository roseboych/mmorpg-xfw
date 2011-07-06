#ifndef __ACE__CONFIG__H
#define __ACE__CONFIG__H

//使用32的时间值
#ifdef _USE_32BIT_TIME_T
	#undef _USE_32BIT_TIME_T
#endif	//_USE_32BIT_TIME_T

#include "ace/config-win32.h"

#endif	//__ACE__CONFIG__H
