/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include <ace/OS_NS_stdio.h>

#include <corelib/log/logmacro.h>

#if defined (ACE_WIN32)

//exception
LONG WINAPI exception_filter( struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	MODULE_LOG_ERROR( MODULE_SYSTEM, "exp code:%08X", ExceptionInfo->ExceptionRecord->ExceptionAddress);

	switch(ExceptionInfo->ExceptionRecord->ExceptionInformation[0])
	{
	case 0:
		MODULE_LOG_ERROR( MODULE_SYSTEM, "Read Error:%08X", ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
		break;
	case 1:
		MODULE_LOG_ERROR( MODULE_SYSTEM, "Write Error:%08X", ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif	//ACE_WIN32

#include "CSSModule.h"

#include <ace/OS_NS_time.h>
int ACE_TMAIN(int argc, ACE_TCHAR **argv) 
{
	//TODO:test
	time_t tnow =ACE_OS::time();
#if defined (ACE_WIN32)
	//挂接异常记录filter
	::SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)exception_filter);
#endif

	MODULE_LOG_INIT("css.properties")

	MODULE_LOG_WARN( MODULE_BOOT, "========css server starting-------------------------------------------");

	if( CSSMODULE->init( argc, argv) != 0)
		return 1;

	MODULE_LOG_WARN( MODULE_BOOT, "========css server start end------------------------------------------");

	MODULE_LOG_WARN( MODULE_BOOT, "========css server begin main loop------------------------------------");

	//进入主循环
	CSSMODULE->main_loop();

	//释放资源
	CSSMODULE->fini();

	MODULE_LOG_WARN( MODULE_BOOT, "========css server end main loop-------------------------------------");

	MODULE_LOG_SHUTDOWN

#if defined (ACE_WIN32)
	::SetUnhandledExceptionFilter( NULL);
#endif
	return 0;
}
