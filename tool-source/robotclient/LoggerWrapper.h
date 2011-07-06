#ifndef _LOGGERWRAPPER_HEADER_H
#define _LOGGERWRAPPER_HEADER_H

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

class CLoggerWrapper
{
public:
	CLoggerWrapper();
	~CLoggerWrapper(void);

public:
	static void InitLogger( const char* file);
	static void Shutdown();
	static void Debug( const char* model, const char* msg,...);
	static void Info( const char* model, const char* msg,...);
	static void Warn( const char* model, const char* msg,...);
	static void Error( const char* model,const  char* msg,...);
	static void Fatal( const char* model, const char* msg,...);
};

#endif //_LOGGERWRAPPER_HEADER_H