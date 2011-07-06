/**
* lgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGSModule.h"

#include <ace/OS_NS_unistd.h>
#include <ace/INET_Addr.h>
#include <ace/Time_Value.h>
#include <ace/Auto_Ptr.h>
#include <ace/Reactor.h>

#include <corelib/net/IOCPMutexAllocator.h>
#include <corelib/log/logmacro.h>
#include <corelib/profiler/ProfilerAnalysis.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/LGSOption.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include <algorithm>

//初始化
BasicModule* BasicModule::base_instance_ =LGSMODULE;

LGSModule::LGSModule(void)
:inherit( IOCP_MutexApplocator, 0),
inherit2()
{
	inherit::set_serviceregistfun( boost::bind(&LGSSvr::fin_regist_service, this, _1, _2));
	inherit::set_servicedisconnfun( boost::bind( &LGSSvr::service_disconnected, this, _1));
}

LGSModule::~LGSModule(void)
{
}

int LGSModule::before_module_init( int argc, ACE_TCHAR *argv[])
{
	MODULE_LOG_WARN( MODULE_BOOT, "++++i am lgs[%d] hahaha", GLOBALCONFIG_INS->get_appsn())

	inherit2::init_svr();

	//注册定时器
	add_moduletimer( 1000*10, boost::bind( &LGSModule::rgssvr_connect_timer, this, _1));
	//500 ns
	add_moduletimer( 500, boost::bind( &LGSModule::player_onlinetick_timer, this, _1));

	return 0;
}

int LGSModule::before_proactor_thread()
{
	LGSOption* opt =GLOBALCONFIG_INS->get_lgsoption( GLOBALCONFIG_INS->get_appsn());

#ifdef CORELIB_WIN32
#else
	//设置接收的最大句柄数
	proactor_.set_maxiohandle( GLOBALCONFIG_INS->totle_server_machine_);
	proactor_.initialize();
#endif	//CORELIB_WIN32

	if( !accepter_.begin_listen( (u_short)opt->port_))
		return -1;

	return 0;
}

void LGSModule::fini()
{
	delall_moduletimer();

	inherit2::uninit_svr();
}

int LGSModule::main_loop()
{
	ACE_Time_Value st( 0, 1000);
	int sleepstep =0;
	while( 1)
	{
		if( sleepstep >= main_loop_)
		{
			ACE_OS::sleep( st);
			sleepstep =0;
		}
		++sleepstep;

		if( exit_module_) break;

		PROFILER_THREAD_TAG( "main thread", EProfileThreadType_Main);

		//定时器支持
		module_timer_.timer_tick();

		//处理消息队列
		BasicCommand *pCmd =pop_one_cmd();
		ACE_Auto_Ptr<BasicCommand> a_pcmd( pCmd);

		if( pCmd == 0) continue;

		pCmd->run();
	}

	return 0;
}

void LGSModule::exit_module( void*)
{
	//关闭acceptor
	accepter_.close_acceptor();

	//关闭所有打开的socket连接
	close_all_service();

	//设置退出标志，结束proactor线程
	inherit::exit_module( 0);

	//清空命令队列
	release_cmds();
}

void LGSModule::post_exit_cmd()
{
	SystemCommand<void*> *pcmd =
		TASKCMD_NEW SystemCommand<void*>( boost::bind( &LGSModule::exit_module, this, (void*)0));

	regist_syscmd( pcmd);
}

void LGSModule::rgssvr_connect_timer( ACE_UINT64 t)
{
	if( rgssvr_.need_connected())
		rgssvr_.connect_to_server();
}

void LGSModule::player_onlinetick_timer( ACE_UINT64 t)
{
	if( used_players_.size() == 0)
		return;

	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	for( size_t ii =0; ii < used_players_.size(); ++ii)
	{
		used_players_[ii]->main_tick( t);
	}
}
