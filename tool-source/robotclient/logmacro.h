#ifndef __LOGMACRO__H__
#define __LOGMACRO__H__

#include "LoggerWrapper.h"

#define MODULE_LOG_INIT(X)	CLoggerWrapper::InitLogger X;

#define MODULE_LOG_SHUTDOWN	CLoggerWrapper::Shutdown();

#define MODULE_LOG_DEBUG(X)	CLoggerWrapper::Debug X;

#define MODULE_LOG_INFO(X)	CLoggerWrapper::Info X;

#define MODULE_LOG_WARN(X)	CLoggerWrapper::Warn X;

#define MODULE_LOG_ERROR(X)	CLoggerWrapper::Error X;

#define MODULE_LOG_FATAL(X)	CLoggerWrapper::Fatal X;

#define MODULE_BOOT		"boot"
#define MODULE_DATABASE	"database"
#define MODULE_TEMP		"temp"
#define MODULE_SYSTEM	"system"

//log format
//#proid (userid) $time$ other information

#endif //__LOGMACRO__H__