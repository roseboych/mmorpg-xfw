/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTSModule.h"

#include <prolib/Pro_all.h>
#include <ace/OS_NS_unistd.h>
#include <ace/INET_Addr.h>
#include <ace/Time_Value.h>
#include <ace/Auto_Ptr.h>
#include <ace/Reactor.h>

#include <corelib/net/IOCPMutexAllocator.h>
#include <corelib/log/logmacro.h>
#include <corelib/profiler/ProfilerAnalysis.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/CTSOption.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/world/WorldInfoConfig.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include <algorithm>

#include "player/Player.h"

//初始化
BasicModule* BasicModule::base_instance_ =CTSMODULE;

CTSModule::CTSModule(void)
:inherit( IOCP_MutexApplocator, 0),
inherit2()
{
	inherit::set_serviceregistfun( boost::bind(&CTSSvr::fin_regist_service, this, _1, _2));
	inherit::set_servicedisconnfun( boost::bind( &CTSSvr::service_disconnected, this, _1));
}

CTSModule::~CTSModule(void)
{
}

int CTSModule::before_module_init( int argc, ACE_TCHAR *argv[])
{
	if( !WORLDINFO->load_worldconfig())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load map data resource failed.....");
		return -1;
	}

	MODULE_LOG_WARN( MODULE_BOOT, "++++i am cts[region:%d] hahaha", GLOBALCONFIG_INS->get_regionsn());

	inherit2::init_svr();

	//注册 服务器连接定时器
	add_moduletimer( 1000*10, boost::bind( &CTSModule::ctssvr_connect_timer, this, _1));
	add_moduletimer( 500, boost::bind( &CTSModule::ctssvr_teamproxy_timer, this, _1));
	add_moduletimer( 5*1000, boost::bind( &CTSModule::ctssvr_teamupdate_timer, this, _1));
	add_moduletimer( 600, boost::bind( &CTSModule::ctssvr_onlineplayer_tick, this, _1));

	return 0;
}

int CTSModule::before_proactor_thread()
{
	//启动监听端口
	SvrTeamOption* sopt =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());
	CTSOption* opt =sopt->get_ctsoption();

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

void CTSModule::fini()
{
	inherit2::uninit_svr();
}

int CTSModule::main_loop()
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

//------------------------Global_Data-----------------------------------//
void CTSModule::exit_module( void*)
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

void CTSModule::post_exit_cmd()
{
	SystemCommand<void*> *pcmd =
		TASKCMD_NEW SystemCommand<void*>( boost::bind( &CTSModule::exit_module, this, (void*)0));

	regist_syscmd( pcmd);
}

void CTSModule::ctssvr_connect_timer( ACE_UINT64 t)
{
	if( rgssvr_.need_connected())
		rgssvr_.connect_to_server();

	if( dpxsvr_.need_connected())
		dpxsvr_.connect_to_server();
}

void CTSModule::ctssvr_teamupdate_timer( ACE_UINT64 t)
{
	USE_PROTOCOL_NAMESPACE;

	if( rgssvr_.is_connected())
	{
		Pro_SvrTeamInfo_NTF* ntf =PROTOCOL_NEW Pro_SvrTeamInfo_NTF();

		Pro_SvrTeamInfo_NTF::svrteaminfo sinfo;
		sinfo.server_index_ =cts_serverindex_;
		sinfo.is_validate_ =1;
		sinfo.maxuser_ =player_nums_;
		sinfo.curuser_ =(S_INT_32)login_players_.size();
		sinfo.teamid_ =cts_teamid_;

		ntf->teams_.push_back( sinfo);

		rgssvr_.send_protocol( ntf);
	}
}

void CTSModule::ctssvr_teamproxy_timer( ACE_UINT64 t)
{
	if( wait_proxys_.size() == 0)
		return;

	USE_PROTOCOL_NAMESPACE;

	NS_STL::vector<S_INT_32> users;
	for( TEAMPROXY_MAP::iterator iter =wait_proxys_.begin(); iter != wait_proxys_.end(); ++iter)
	{
		TeamProxySession* proxy =iter->second;
		if( proxy->proxy_timeover( t))
		{
			users.push_back( proxy->uuid_.userid_);

			//发送超时通知
			Pro_SvrSelTeamTimeout_NTF* ntf =PROTOCOL_NEW Pro_SvrSelTeamTimeout_NTF();
			PRO_UUID_FILL( ntf, proxy->server_index_, proxy->uuid_)
			ntf->proxy_index_ =proxy->proxy_index_;

			proxy->send_to_gts( ntf->clone());
			this->send_to_rgs( ntf);
		}
	}

	for( size_t ii =0; ii < users.size(); ++ii)
		free_waitproxy( users[ii]);
}

void CTSModule::ctssvr_onlineplayer_tick( ACE_UINT64 t)
{
	if( login_players_.size() == 0)
		return;

	PLAYERALL_MAP::iterator eiter =login_players_.end();
	for( PLAYERALL_MAP::iterator iter =login_players_.begin(); iter != eiter; ++iter)
	{
		Player* p =iter->second;
		p->tick( t);
	}
}
