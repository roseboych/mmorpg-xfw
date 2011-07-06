/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

USE_PROTOCOL_NAMESPACE;

void CSSSvr::gts_netadapter( BasicProtocol* p, bool& autorelease)
{
	//其他协议的处理
	AdapterPlayer* user =get_adapterplayer( p->get_uuidglobalindex());
	if( user == 0)
		return;

	BaseStoryService* svr =0;
	{
		ACE_Guard<ACE_Thread_Mutex> mon( player_mutex_);

		if( user->insvr_ == 0 || !p->same_session( user->uuid_))
			return;
		svr =user->insvr_;

		if( p->iid_ == AOI_TELEPORT_REQ)
		{
			Pro_AppTeleport_req *req =dynamic_cast<Pro_AppTeleport_req*>(p);
			user->player_teleportout( req, autorelease);
			return;
		}
	}

	if( svr == 0)
		return;

	//派发到指定的内容处理
	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == AOI_ENTERINS_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_instenter_req, svr, _1, _2);
	}
	else if( p->iid_ == AOI_ENTERINSCONFIRM_NTF)
	{
		fun =boost::bind( &BaseStoryService::gts_instenterconfirm_ntf, svr, _1, _2);
	}
	else if( p->iid_ == SVR_LNKEEPLOAD_NTF)
	{
		fun =boost::bind( &BaseStoryService::gts_lnkkeepload_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_EQUIPITEM_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_equipitem_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_PETSUMMON_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_petsummon_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_TEAMCHAT_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_teamchat_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_P2PCHAT_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_p2pchat_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_SKILLSTUDY_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_skillstudy_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_SKILLUSED_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_skillused_req, svr, _1, _2);
	}
	else if( p->iid_ == APP_PLAYERMOVE_REQ)
	{
		fun =boost::bind( &BaseStoryService::gts_playermoveto_req, svr, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
		svr->regist_netcmd( pcmd);
		autorelease =false;
	}
}

void CSSSvr::dpx_netadapter( BasicProtocol* p, bool& autorelease)
{
	AdapterPlayer* user =get_adapterplayer( p->get_uuidglobalindex());
	if( user == 0)
		return;

	BaseStoryService* svr =0;
	{
		ACE_Guard<ACE_Thread_Mutex> mon( player_mutex_);
		if( user->insvr_ == 0 || !p->same_session( user->uuid_))
			return;
		svr =user->insvr_;
	}

	if( svr == 0)
		return;

	//派发到指定的内容处理
	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == GAME_CHRLOAD_ACK)
	{
		fun =boost::bind( &BaseStoryService::dpx_chrload_ack, svr, _1, _2);
	}
	else if( p->iid_ == GAME_CHRFIN_NTF)
	{
		fun =boost::bind( &BaseStoryService::dpx_chrfin_ntf, svr, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
		svr->regist_netcmd( pcmd);
		autorelease =false;
	}
}

void CSSSvr::cts_netadapter( BasicProtocol* p, bool& autorelease)
{
	AdapterPlayer* user =get_adapterplayer( p->get_uuidglobalindex());
	if( user == 0)
		return;

	BaseStoryService* svr =0;
	{
		ACE_Guard<ACE_Thread_Mutex> mon( player_mutex_);

		if( p->iid_ == GAME_CHRREGTOWORLD_REQ)
		{
			Pro_ChrRegistToWorld_req* req =dynamic_cast<Pro_ChrRegistToWorld_req*>(p);
			user->player_regist2world( req, autorelease);
			return;
		}
		else if( p->iid_ == AOI_TELEPORT_ACK)
		{
			Pro_AppTeleport_ack *ack =dynamic_cast<Pro_AppTeleport_ack*>(p);
			user->player_teleport( ack, autorelease);
			return;
		}
		else if( p->iid_ == AOI_ENTERINS_REQ)
		{
			Pro_AppEnterIns_req* req =dynamic_cast<Pro_AppEnterIns_req*>( p);
			user->player_instcellproxy( req, autorelease);
			return;
		}
		else if( p->iid_ == AOI_ENTERINS_ACK)
		{
			Pro_AppEnterIns_ack* ack =dynamic_cast<Pro_AppEnterIns_ack*>( p);
			user->player_enterinstack( ack, autorelease);
			return;
		}
		else if( p->iid_ == AOI_ENTERINSOT_NTF)
		{
			Pro_AppEnterInsOvertime_ntf* ntf =dynamic_cast<Pro_AppEnterInsOvertime_ntf*>( p);
			user->player_enterinstovertime( ntf, autorelease);
			return;
		}
		else if( p->iid_ == SVR_USERLOST_NTF)
		{
			//玩家掉线特别处理
			if( !p->same_session( user->uuid_))
				return;

			MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[adapter]. user:[%d]", p->uuid_.userid_);

			if( user->insvr_)
			{
				NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_userlost_ntf, user->insvr_, _1, _2);
				NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
				user->insvr_->regist_netcmd( pcmd);
				autorelease =false;
			}

			//清除玩家信息
			user->reset();

			return;
		}
		else if( !p->same_session( user->uuid_))
			return;

		svr =user->insvr_;
	}

	if( svr == 0)
		return;

	//派发到指定的内容处理
	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == 100)
	{
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
		svr->regist_netcmd( pcmd);
		autorelease =false;
	}
}

void CSSSvr::css_netadapter( BasicProtocol* p, bool& autorelease)
{
	AdapterPlayer* user =get_adapterplayer( p->get_uuidglobalindex());
	if( user == 0)
		return;

	BaseStoryService* svr =0;
	{
		ACE_Guard<ACE_Thread_Mutex> mon( player_mutex_);
		if( user->insvr_ == 0 || !p->same_session( user->uuid_))
			return;
		svr =user->insvr_;
	}

	//派发到指定的内容处理
	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == 100)
	{

	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
		svr->regist_netcmd( pcmd);
		autorelease =false;
	}
}
