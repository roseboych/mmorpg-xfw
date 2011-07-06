#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/thread.h>
#include <log4cxx/logmanager.h>

#include <log4cxx/log4cwrapper.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

//-------------implement log4cwrapper-------------------------//
void Log4cWrapper::InitLog4c( const String& file)
{
#ifdef HAVE_XML
	if(StringHelper::endsWith( file , _T("xml")))
	{
		xml::DOMConfigurator::configureAndWatch( file, 3000);
	} 
	else
#endif
	{
		PropertyConfigurator::configureAndWatch( file, 3000);
	}
}

void Log4cWrapper::shutdown()
{
	LogManager::shutdown();
}

void Log4cWrapper::Log4cxx_Debug( String model, String msg)
{
	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_DEBUG( ptr, msg);
}

void Log4cWrapper::Log4cxx_Warn( String model, String msg)
{
	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_WARN( ptr, msg);
}

void Log4cWrapper::Log4cxx_Error( String model, String msg)
{
	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_ERROR( ptr, msg);
}

void Log4cWrapper::Log4cxx_Fatal( String model, String msg)
{
	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_FATAL( ptr, msg);
}

void Log4cWrapper::Log4cxx_Info( String model, String msg)
{
	LoggerPtr ptr = Logger::getLogger( model);
	LOG4CXX_INFO( ptr, msg);
}

//---------------------

void Log4cxx_Init( char* file)
{
	USES_CONVERSION;
	Log4cWrapper::InitLog4c( A2T(file));
}

void Log4cxx_Shutdown()
{
	Log4cWrapper::shutdown();
}

void Log4cxx_Info( char* model, char* msg)
{
	USES_CONVERSION;
	Log4cWrapper::Log4cxx_Info( A2T(model), A2T(msg));
}

void Log4cxx_Warn( char* model, char* msg)
{
	USES_CONVERSION;
	Log4cWrapper::Log4cxx_Warn( A2T(model), A2T(msg));
}

void Log4cxx_Error( char* model, char* msg)
{
	USES_CONVERSION;
	Log4cWrapper::Log4cxx_Error( A2T(model), A2T(msg));
}

void Log4cxx_Debug( char* model, char* msg)
{
	USES_CONVERSION;
	Log4cWrapper::Log4cxx_Debug( A2T(model), A2T(msg));
}

void Log4cxx_Fatal( char* model, char* msg)
{
	USES_CONVERSION;
	Log4cWrapper::Log4cxx_Fatal( A2T(model), A2T(msg));
}
