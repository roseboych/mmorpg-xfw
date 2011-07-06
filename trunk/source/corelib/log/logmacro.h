/**
* corelib
*
* @category		log module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LOGMACRO__H__
#define __LOGMACRO__H__

#include <logmodule/LoggerWrapper.h>

#include <prolib/core_type.h>

#include <map>

#define MODULE_LOG_INIT(x)	LoggerWrapper::InitLogger(x);
#define MODULE_LOG_SHUTDOWN	LoggerWrapper::Shutdown();
#define MODULE_LOG_DEBUG( cat, fmt, ...) LoggerWrapper::Debug( cat, fmt, __VA_ARGS__);

#define MODULE_LOG_INFO( cat, fmt, ...)	LoggerWrapper::Info( cat, fmt, __VA_ARGS__);

#define MODULE_LOG_WARN( cat, fmt, ...)	LoggerWrapper::Warn( cat, fmt, __VA_ARGS__);

#define MODULE_LOG_ERROR( cat, fmt, ...) LoggerWrapper::Error( cat, fmt, __VA_ARGS__);

#define MODULE_LOG_FATAL( cat, fmt, ...) LoggerWrapper::Fatal( cat, fmt, __VA_ARGS__);

//log category
//boot type
#define MODULE_BOOT			"boot"
//database type
#define MODULE_DATABASE		"database"
//系统错误
#define MODULE_SYSTEM		"system"
//运行时错误
#define MODULE_RUNTIME		"runtime"
//临时测试日志
#define MODULE_TEMP			"temp"
//性能测试
#define MODULE_PERFORMANCE	"perfm"

#endif //__LOGMACRO__H__
