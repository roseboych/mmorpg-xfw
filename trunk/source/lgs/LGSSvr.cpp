/**
* lgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGSSvr.h"

#include <prolib/Pro_all.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/RGSOption.h>
#include <reslib/deploy/LGSOption.h>

#include "LGSModule.h"

#include <algorithm>
#include <memory>

LGSSvr::LGSSvr(void)
{
}

LGSSvr::~LGSSvr(void)
{
}

void LGSSvr::init_svr()
{
	//rgs
	RGSOption* opt =GLOBALCONFIG_INS->get_rgsoption();
	rgssvr_.set_option( opt);

	//player
	empty_players_.clear();
	used_players_.clear();

	LGSOption* olgs =GLOBALCONFIG_INS->get_lgsoption( GLOBALCONFIG_INS->get_appsn());
	max_users_ =olgs->maxuser_;
	players_.reset( FRAMEWK_NEW Player[ max_users_]);
	for( int ii =0; ii < max_users_; ++ii)
	{
		players_[ii].reset();
		players_[ii].server_index_ =ii;

#ifdef CORELIB_WIN32
		players_[ii].proactor( LGSMODULE->proactor());
#else
		players_[ii].reactor( LGSMODULE->reactor());
#endif

		empty_players_.push_back( &(players_[ii]));
	}

	//svrteam update initialize
	svr_update_.init_svrteaminfo();
}

void LGSSvr::uninit_svr()
{
	used_players_.clear();
	empty_players_.clear();
}

void LGSSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	LGSAPP_SERVICE_ENUM stype =( LGSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == LGSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =dynamic_cast<RGS_Service*>( s);

		if( rgssvr_.is_waitregistack())
		{
			rgssvr_.set_status( SVRREGIST_ST_SUCC);
			rgssvr_.set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to rgs success......");
		}

		autorelease =false;
	}
}

void LGSSvr::service_disconnected( BasicService* s)
{
	LGSAPP_SERVICE_ENUM stype =( LGSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == LGSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =dynamic_cast<RGS_Service*>(s);
		if( rgssvr_.is_connected())
		{
			MODULE_LOG_WARN( MODULE_BOOT, "++++me disconnect to rgs......");

			svr_rgsdisconnect();
		}
		rgssvr_.reset();
	}
}

void LGSSvr::close_all_service()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

void LGSSvr::svr_rgsdisconnect()
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to rgs. now, clear system resource......");
}

Player* LGSSvr::get_emptyplayer()
{
	if( empty_players_.size() == 0)
		return 0;

	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	Player* player =empty_players_.front();
	empty_players_.pop_front();

	used_players_.push_back( player);

	player->prepare();

	return player;
}

void LGSSvr::release_player( Player* p)
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	EMPTY_PLAYERS_LIST::iterator f =NS_STL::find( empty_players_.begin(), empty_players_.end(), p);
	if( f == empty_players_.end())
		empty_players_.push_back( p);

	p->reset();

	USED_PLAYERS_VECTOR::iterator fiter =NS_STL::find( used_players_.begin(), used_players_.end(), p);
	if( fiter != used_players_.end())
		used_players_.erase( fiter);
}

void LGSSvr::player_disconnected( LGS_Service* ps)
{
	//处理断线
	if( ps->player_ == 0)
		return;

	//防止因为异步导致的多次discnnect通知
	{
		ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

		USED_PLAYERS_VECTOR::iterator fiter =NS_STL::find( used_players_.begin(), used_players_.end(), ps->player_);
		if( fiter == used_players_.end())
			return;
	}

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from client. user:[%d]", ps->player_->uuid_.userid_);

	//向rgs发送断线通知
	PRO::Pro_SvrUserLost_NTF* ntf =PROTOCOL_NEW PRO::Pro_SvrUserLost_NTF();
	SESSION_UUID_FILL( ntf, ps->player_->server_index_, ps->player_->uuid_)

	this->send_to_rgs( ntf);

	//释放玩家资源
	release_player( ps->player_);
}

BasicProtocol* LGSSvr::construct_svrteam_notify()
{
	PRO::Pro_SvrTeamInfo_NTF* ntf =svr_update_.construct_notify();
	return ntf;
}