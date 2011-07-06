#include "loggerwrapper.h"
#include <stdarg.h>
#include <stdio.h>

#define OPEN_LOG4C_DEF

extern void Log4cxx_Init( char* file);
extern void Log4cxx_Shutdown();
extern void Log4cxx_Info( char* model, char* msg);
extern void Log4cxx_Warn( char* model, char* msg);
extern void Log4cxx_Error( char* model, char* msg);
extern void Log4cxx_Debug( char* model, char* msg);
extern void Log4cxx_Fatal( char* model, char* msg);

#define LOGMSG_LENGTH	512

CLoggerWrapper::CLoggerWrapper()
{
}

CLoggerWrapper::~CLoggerWrapper(void)
{
}

void CLoggerWrapper::InitLogger( const char* file)
{
#ifdef OPEN_LOG4C_DEF
	Log4cxx_Init( const_cast<char*>(file));
#endif
}

void CLoggerWrapper::Shutdown()
{
#ifdef OPEN_LOG4C_DEF
	Log4cxx_Shutdown();
#endif
}

void CLoggerWrapper::Debug( const char* model, const char* msg,...)
{
#ifdef OPEN_LOG4C_DEF
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;
	va_start( vaList, msg);

	::_vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);

	va_end( vaList );

	Log4cxx_Debug( const_cast<char*>(model), buf);
#endif
}

void CLoggerWrapper::Info( const char* model, const char* msg,...)
{
#ifdef OPEN_LOG4C_DEF
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;
	va_start( vaList, msg);

	::_vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);

	va_end( vaList );

	Log4cxx_Info( const_cast<char*>(model), buf);
#endif
}

void CLoggerWrapper::Warn( const char* model, const char* msg,...)
{
#ifdef OPEN_LOG4C_DEF
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;
	va_start( vaList, msg);

	::_vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);

	va_end( vaList );

	Log4cxx_Warn( const_cast<char*>(model), buf);
#endif
}

void CLoggerWrapper::Error( const char* model, const char* msg,...)
{
#ifdef OPEN_LOG4C_DEF
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;
	va_start( vaList, msg);

	::_vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);

	va_end( vaList );

	Log4cxx_Error( const_cast<char*>(model), buf);
#endif
}

void CLoggerWrapper::Fatal( const char* model, const char* msg,...)
{
#ifdef OPEN_LOG4C_DEF
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;
	va_start( vaList, msg);

	::_vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);

	va_end( vaList );

	Log4cxx_Fatal( const_cast<char*>(model), buf);
#endif
}
