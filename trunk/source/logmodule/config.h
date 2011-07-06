#ifndef __CONFIG__H__
#define __CONFIG__H__

#include "ace/config.h"

#ifdef LOGMODULE_WIN32
#undef LOGMODULE_WIN32
#endif

#ifdef ACE_WIN32
#define LOGMODULE_WIN32
#else
#endif

#endif	//__CONFIG__H__
