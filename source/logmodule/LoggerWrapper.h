/**
* corelib
*
* @category		log module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LOGGERWRAPPER__HEADER__H__
#define __LOGGERWRAPPER__HEADER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

/**
* @class LoggerWrapper
*
* @brief log4c的日志封装类
*
*/
class LoggerWrapper
{
private:
	LoggerWrapper();
	~LoggerWrapper();

public:
	static void InitLogger( const char* file);
	static void Shutdown();
	static void Debug( const char* model, const char* msg,...);
	static void Info( const char* model, const char* msg,...);
	static void Warn( const char* model, const char* msg,...);
	static void Error( const char* model,const  char* msg,...);
	static void Fatal( const char* model, const char* msg,...);

private:

};

#endif //__LOGGERWRAPPER__HEADER__H__
