/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ContentServiceThread.h"

#include <ace/OS_NS_unistd.h>
#include <corelib/core/TLSContext.h>
#include "BaseStoryService.h"

#include "CSSModule.h"

BaseStoryService* get_thiscontext()
{
	return (BaseStoryService*)TLSContext::get_app_userdata();
}

ContentServiceThread::ContentServiceThread( S_INT_32 n):inherit(),
initialized_( false),
will_exit_( false),
thread_nums_( n)
{
}

ContentServiceThread::~ContentServiceThread()
{
}

bool ContentServiceThread::start_threads()
{
	if( initialized_)
		return true;

	//创建线程池
	if( this->activate( THR_NEW_LWP|THR_JOINABLE, thread_nums_, 1, THREAD_PRIORITY_HIGHEST) == 0)
	{
		initialized_ =true;
		return true;
	}

	return false;
}

void ContentServiceThread::end_threads()
{
	//设置退出标志
	will_exit_ =true;

	//等待退出
	this->wait();

	initialized_ =false;
}

int ContentServiceThread::svc()
{
	BaseStoryService* svr =0;
	//int loopnum =CSSMODULE->main_loop();

	//初始化内容线程的tls
	TLSContext tls_context;
	tls_context.init_tlscontext();

	ACE_UINT64 now_t =0;
	ACE_Time_Value st( 0, 1000);
	int sleepstep =0;

	//线程循环
	while( !will_exit_)
	{
		if( sleepstep >= 100)
		{
			ACE_OS::sleep( st);
			sleepstep =0;
		}
		++sleepstep;

		//获取服务，处理
		svr =CSSMODULE->pop_storyservice();
		if( svr == 0)
			continue;

		TLSContext::set_app_userdata( (void*)svr);

		ACE_Time_Value now =ACE_OS::gettimeofday();
		now.msec( now_t);

		//服务调用
		svr->do_service( now_t);

		TLSContext::set_app_userdata( (void*)svr);

		CSSMODULE->push_storyservice( svr);
		svr =0;
	}

	if( svr)
	{
		CSSMODULE->push_storyservice( svr);
		svr =0;
	}

	tls_context.uninit_tlscontext();

	return 0;
}
