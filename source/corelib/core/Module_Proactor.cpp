/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/core/Module_Proactor.h"

#include "ace/OS_NS_unistd.h"

#ifdef CORELIB_WIN32
	#include "ace/win32_proactor.h"
#else
	#include "ace/Dev_Poll_Reactor.h"
#endif

#include "corelib/log/logmacro.h"

Module_Proactor::Module_Proactor(void)
	:thread_nums_( 1)
#ifdef CORELIB_WIN32
#else
	,max_iohandle_(64)
#endif
{
}

Module_Proactor::~Module_Proactor(void)
{
	//TODO:框架会释放内存异常。以后处理
#ifdef CORELIB_WIN32
	proactor_.release();
#else
	reactor_.release();
#endif
}

int Module_Proactor::svc (void)
{
	MODULE_LOG_DEBUG( MODULE_TEMP, "start iocp thread pool ...");

#ifdef CORELIB_WIN32
	proactor_->proactor_run_event_loop();

#else
	reactor_->owner( ACE_OS::thr_self());
	reactor_->run_reactor_event_loop();

#endif

	MODULE_LOG_DEBUG( MODULE_TEMP, "end iocp thread pool ...");
	return 0;
}

int Module_Proactor::initialize()
{
#ifdef CORELIB_WIN32
	ACE_WIN32_Proactor *proactor_impl = FRAMEWK_NEW ACE_WIN32_Proactor( thread_nums_, false);
	if( proactor_impl == 0)
		return -1;

	proactor_.reset( FRAMEWK_NEW ACE_Proactor( proactor_impl, 1));

#else
	ACE_Dev_Poll_Reactor *reactor_impl = FRAMEWK_NEW ACE_Dev_Poll_Reactor();
	if( reactor_impl == 0)
		return -1;

	reactor_.reset( FRAMEWK_NEW ACE_Reactor( reactor_impl, 1));

#endif

	return 0;
}

void Module_Proactor::start()
{
#ifdef CORELIB_WIN32
	if( thread_nums_ <= 0)
		thread_nums_ =1;
#else
	thread_nums_ =1;
#endif

	activate( THR_NEW_LWP|THR_JOINABLE, thread_nums_, 1);
}

void Module_Proactor::stop()
{
#ifdef CORELIB_WIN32
	proactor_->proactor_end_event_loop();

#else
	reactor_->end_reactor_event_loop();

#endif

	this->wait();
}
