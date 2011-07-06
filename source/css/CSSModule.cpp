/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSModule.h"

#include <ace/OS_NS_unistd.h>
#include <ace/INET_Addr.h>
#include <ace/Time_Value.h>
#include <ace/Auto_Ptr.h>
#include <ace/Reactor.h>

#include <corelib/net/IOCPMutexAllocator.h>
#include <corelib/log/logmacro.h>
#include <corelib/profiler/ProfilerAnalysis.h>

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/items/ItemsConfig.h>
#include <reslib/skills/SkillTreeConfig.h>
#include <reslib/buffers/BufferConfig.h>
#include <reslib/gamesetting/XGameSetting.h>

#include "character/states/PlayerBehaviorLoader.h"

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include <algorithm>

//初始化
BasicModule* BasicModule::base_instance_ =CSSMODULE;

CSSModule::CSSModule(void)
:inherit( IOCP_MutexApplocator, 0),
inherit2()
{
	inherit::set_serviceregistfun( boost::bind( &CSSSvr::fin_regist_service, this, _1, _2));
	inherit::set_servicedisconnfun( boost::bind( &CSSSvr::service_disconnected, this, _1));
}

CSSModule::~CSSModule(void)
{
}

int CSSModule::before_module_init( int argc, ACE_TCHAR *argv[])
{
	MODULE_LOG_WARN( MODULE_BOOT, "++++i am css[region:%d sn:%d] hahaha", GLOBALCONFIG_INS->get_regionsn(), GLOBALCONFIG_INS->get_appsn());

	//初始化资源信息
	if( !GAMESETTING_CFG->load_gamesetting())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load gamesetting failed.....");
		return -1;
	}

	if( !PlayerBehaviorLoader::load_behaviorconfig())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load character behavior config failed.....");
		return -1;
	}

	if( !ITEMRESMGR_INS->load_itemconfig())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load item resource failed.....");
		return -1;
	}

	if( !SKILLTREE_CFG->load_skilltree())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load skill tree resource failed.....");
		return -1;
	}

	if( !BUFFER_CFG->load_bufferconfig())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load buffer config resource failed.....");
		return -1;
	}

	if( !WORLDINFO->load_worldconfig())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load map data resource failed.....");
		return -1;
	}

	inherit2::init_svr();

	//注册 服务器连接定时器
	add_moduletimer( 1000*10, boost::bind( &CSSModule::csssvr_connect_timer, this, _1));

	return 0;
}

int CSSModule::before_proactor_thread()
{
	SvrTeamOption* sopt =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());
	CSSOption* opt =sopt->get_cssoption( GLOBALCONFIG_INS->get_appsn());

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

void CSSModule::fini()
{
	inherit2::uninit_svr();
}

int CSSModule::main_loop()
{
	ACE_Time_Value st( 0, 1000);

	while( 1)
	{
		ACE_OS::sleep( st);

		if( exit_module_) break;

		PROFILER_THREAD_TAG( "main thread", EProfileThreadType_Main);

		//定时器支持
		module_timer_.timer_tick();

		//处理消息队列
		int runnum =0;
		while( 1)
		{
			BasicCommand *pCmd =pop_one_cmd();
			if( pCmd == 0) break;
			++runnum;

			ACE_Auto_Ptr<BasicCommand> a_pcmd( pCmd);
			pCmd->run();

			if( runnum > 1000)
				break;
		}
	}

	return 0;
}

//------------------------Global_Data-----------------------------------//
void CSSModule::exit_module( void*)
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

void CSSModule::post_exit_cmd()
{
	SystemCommand<void*> *pcmd =
		TASKCMD_NEW SystemCommand<void*>( boost::bind( &CSSModule::exit_module, this, (void*)0));

	regist_syscmd( pcmd);
}

void CSSModule::csssvr_connect_timer( ACE_UINT64 t)
{
	//cts
	if( ctssvr_.need_connected())
		ctssvr_.connect_to_server();

	//dpx
	if( dpxsvr_.need_connected())
		dpxsvr_.connect_to_server();

	//css
	csssvr_.css2link_tick( t);
}
