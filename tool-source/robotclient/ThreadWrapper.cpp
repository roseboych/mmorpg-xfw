#include "StdAfx.h"

#include "threadwrapper.h"
#include "logmacro.h"
#include "BasicImpl.h"

ThreadWrapper::ThreadWrapper()
:cases_( 0),
cancel_( false),
hthread( 0),
dwThreadId( 0),
case_num_( 0)
{
}

ThreadWrapper::~ThreadWrapper(void)
{
	/*
	if( !cancel_)
		stop_thread();
	*/
	stop_thread();
}

void ThreadWrapper::set_thread( int startnum, int num)
{
	case_num_ =num;

	if( case_num_ > 0)
	{
		cases_ =new TestCase[case_num_];
		for( int i =0; i < case_num_; ++i)
			cases_[i].set_userid( startnum + i);
	}
}

DWORD  WINAPI ThreadWrapper::Thread(LPVOID  lparam)  
{  
	ThreadWrapper *pThis =reinterpret_cast<ThreadWrapper*>(lparam);

	srand( time( 0));

	MODULE_LOG_INFO((MODULE_TEMP, "start thread: %d", pThis->dwThreadId));

	pThis->main_loop();

	MODULE_LOG_INFO((MODULE_TEMP, "end thread: %d", pThis->dwThreadId));

	return 1;
}

void ThreadWrapper::start_thread()
{
	if( hthread)
		return;

	hthread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Thread, this,0,&dwThreadId);  
}

void ThreadWrapper::stop_thread()
{
	if( cases_)
	{
		for( int i =0; i < case_num_; ++i)
			cases_[i].set_cancel();
	}

	cancel_ =true;
	if( hthread > 0)
	{
		::WaitForSingleObject( hthread, INFINITE);
		CloseHandle( hthread);
	}

	hthread =0;
	dwThreadId =0;

	if( cases_)
		delete[] cases_;
	cases_ =0;
}

void ThreadWrapper::set_cancel()
{ 
	cancel_ =true; 
	if( cases_)
	{
		for( int i =0; i < case_num_; ++i)
			cases_[i].set_cancel();
	}
};

void ThreadWrapper::main_loop()
{
	for( int i =0; i < case_num_; ++i)
		cases_[i].start_case();

	int count =0;
	while(1)  
	{
		if( cancel_) break;

		::Sleep( 1);

		ULONGLONG nnow =BasicImpl::GetMillisecondTime();
		for( int i =0; i < case_num_; ++i)
		{
			if( cancel_) break;
			cases_[i].run_it( nnow);
		}
	}

	for( int i =0; i < case_num_; ++i)
		cases_[i].stop_case();
}
