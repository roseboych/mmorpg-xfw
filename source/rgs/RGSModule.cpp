/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGSModule.h"

#include <ace/OS_NS_unistd.h>
#include <ace/INET_Addr.h>
#include <ace/Time_Value.h>
#include <ace/Auto_Ptr.h>
#include <ace/Reactor.h>

#include <corelib/net/IOCPMutexAllocator.h>
#include <corelib/log/logmacro.h>
#include <corelib/profiler/ProfilerAnalysis.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/RGSOption.h>
#include <reslib/deploy/DBConfig.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include <algorithm>

//初始化
BasicModule* BasicModule::base_instance_ =RGSMODULE;

RGSModule::RGSModule(void)
:inherit( IOCP_MutexApplocator, 0),
inherit2()
{
	inherit::set_serviceregistfun( boost::bind(&RGSSvr::fin_regist_service, this, _1, _2));
	inherit::set_servicedisconnfun( boost::bind( &RGSSvr::service_disconnected, this, _1));
}

RGSModule::~RGSModule(void)
{
}

int RGSModule::before_module_init( int argc, ACE_TCHAR *argv[])
{
	MODULE_LOG_WARN( MODULE_BOOT, "++++i am rgs hahaha");

	//完成global初始化
	inherit2::init_svr();

	//5 s
	add_moduletimer( 5*1000, boost::bind( &RGSModule::svrteam_update_timer, this, _1));
	//10s
	add_moduletimer( 5*1000, boost::bind( &RGSModule::svrteam_playertick_timer, this, _1));

	return 0;
}

int RGSModule::before_proactor_thread()
{
	RGSOption* opt =GLOBALCONFIG_INS->get_rgsoption();

#ifdef CORELIB_WIN32
#else
	//设置接收的最大句柄数
	proactor_.set_maxiohandle( GLOBALCONFIG_INS->totle_server_machine_);
	proactor_.initialize();

#endif	//CORELIB_WIN32
	if( !accepter_.begin_listen( (u_short)opt->port_))
		return -1;

	if( !controller_accepter_.begin_listen( (u_short)opt->controller_port_))
		return -1;

	return 0;
}

void RGSModule::fini()
{
	inherit2::uninit_svr();
}

void RGSModule::set_dbpoolprop()
{
	RGSOption* opt =GLOBALCONFIG_INS->get_rgsoption();
	DBConfig& db =opt->dbinfo_;
	
	//设置支持数据库
	support_dbpool_ =true;
	
	//设置数据库信息
	dbpool_.set_threadnum( db.dbthreadnum_);
	dbpool_.set_dbloop( db.dbloopstep_);
	dbpool_.set_dbinfo( db.dbname_.c_str(), db.dbuser_.c_str(), db.dbpwd_.c_str());
	dbpool_.set_callfun( boost::bind( &RGSModule::dberror_callback, this));
}

int RGSModule::main_loop()
{
	ACE_Time_Value st( 0, 1000);
	int sleepstep =0;
	while( true)
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
#include <prolib/Pro_boot.h>

void RGSModule::exit_module( void*)
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

void RGSModule::post_exit_cmd()
{
	SystemCommand<void*> *pcmd =
		TASKCMD_NEW SystemCommand<void*>( boost::bind( &RGSModule::exit_module, this, (void*)0));

	regist_syscmd( pcmd);
}

void RGSModule::dberror_callback()
{

}

void RGSModule::svrteam_update_timer( ACE_UINT64 t)
{
	if( lgssvr_nums_ <= 0)
		return;

	PRO::Pro_SvrTeamInfo_NTF* ntf =svr_update_.construct_notify();
	for( int ii =1; ii < lgssvr_nums_; ++ii)
		lgssvr_[ii].send_protocol( ntf->clone());

	lgssvr_[0].send_protocol( ntf);
}

void RGSModule::svrteam_playertick_timer( ACE_UINT64 t)
{
	if( userid_players_.size() == 0)
		return;

	NS_STL::vector<PlayerInfo*> rms;
	NS_STL::vector<PlayerInfo*> rmslnk;
	for( USEDUSER_MAP::iterator iter =userid_players_.begin(); iter != userid_players_.end(); ++iter)
	{
		PlayerInfo* pi =iter->second;
		if( pi->quitteamreconn_timeout( t))
			rms.push_back( pi);
		else if( pi->lnkkeep_timeout( t))
			rmslnk.push_back( pi);
	}

	for( size_t ii =0; ii < rms.size(); ++ii)
		this->free_player( rms[ii]);
	rms.clear();

	for( size_t ii =0; ii < rmslnk.size(); ++ii)
	{
		//玩家强制退出
		player_lnkkeeplogout( rmslnk[ii]);
	}
	rmslnk.clear();
}
