#ifndef _LOG4CXX_WRAPPER_H
#define _LOG4CXX_WRAPPER_H

namespace log4cxx
{
	/**
	log4c��װ��
	*/
	class Log4cWrapper
	{
	public:
		Log4cWrapper(){
		}

		~Log4cWrapper(){
		}

	public:
		//��ʼ�������ļ�
		static inline void InitLog4c( const String& file);
		static inline void shutdown();

		static inline void Log4cxx_Info( String model, String msg);
		static inline void Log4cxx_Debug( String model, String msg);
		static inline void Log4cxx_Warn( String model, String msg);
		static inline void Log4cxx_Error( String model, String msg);
		static inline void Log4cxx_Fatal( String model, String msg);
	};
}

#endif //_LOG4CXX_WRAPPER_H