/**
* corelib
*
* @category		log module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "logmodule/LoggerWrapper.h"

#ifndef LOG4CXX_STATIC
	#define LOG4CXX_STATIC
#endif

#include "logmodule/config.h"
#include "ace/OS_NS_stdio.h"

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/thread.h>
#include <log4cxx/logmanager.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

#define LOGMSG_LENGTH	512

#ifdef LOGMODULE_WIN32
#else
	extern void ForceSymbolReferences();
#endif

LoggerWrapper::LoggerWrapper()
{
#ifndef LOGMODULE_WIN32
	ForceSymbolReferences();
#endif
}

LoggerWrapper::~LoggerWrapper()
{
}

void LoggerWrapper::InitLogger( const char* file)
{
	PropertyConfigurator::configureAndWatch( file, 3000);
}

void LoggerWrapper::Shutdown()
{
	LogManager::shutdown();
}

void LoggerWrapper::Debug( const char* model, const char* msg, ...)
{
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_DEBUG( ptr, buf);
}

void LoggerWrapper::Info( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_INFO( ptr, buf);
}

void LoggerWrapper::Warn( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_WARN( ptr, buf);
}

void LoggerWrapper::Error( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_ERROR( ptr, buf);
}

void LoggerWrapper::Fatal( const char* model, const char* msg,...)
{
	char buf[LOGMSG_LENGTH] ={0,};

	va_list vaList;

	va_start( vaList, msg);
	::vsnprintf( buf, LOGMSG_LENGTH-1, msg, vaList);
	va_end( vaList );

	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_FATAL( ptr, buf);
}
