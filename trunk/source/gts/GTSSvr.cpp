/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTSSvr.h"

#include <prolib/Pro_all.h>

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/CTSOption.h>

#include <algorithm>
#include <memory>

#include "GTSModule.h"

ACE_Cached_Allocator<Player, ACE_Null_Mutex> GTSSvr::cache_players_(GTS_MAX_CLIENT_NUM);

GTSSvr::GTSSvr(void)
{
}

GTSSvr::~GTSSvr(void)
{
}

void GTSSvr::init_svr()
{
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());

	//css
	csssvr_nums_ =st->get_cssnums();
	csssvr_.reset( FRAMEWK_NEW CSSLink[csssvr_nums_]);

	NS_STL::list< CSSOption*> css;
	st->get_cssoptions( css);
	for( NS_STL::list< CSSOption*>::iterator iter =css.begin(); iter != css.end(); ++iter)
	{
		CSSOption* opt =(*iter);
		CSSLink* l =&csssvr_[opt->server_index_];
		l->set_option( opt);
	}

	//cts
	ctssvr_.set_option( st->get_ctsoption());

	//players
	used_players_.clear();

	GTSOption* dp =st->get_gtsoption( GLOBALCONFIG_INS->get_appsn());
	gts_index_ =dp->server_index_;
	start_player_ =dp->startuser_;
	player_nums_ =dp->usernum_;

	team_proxys_.reset( FRAMEWK_NEW TeamProxySession[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		//proxy信息
		team_proxys_[ii].proxy_index_ =start_player_ + ii;
		team_proxys_[ii].reset();
	}
}

void GTSSvr::uninit_svr()
{
	used_players_.clear();
}

void GTSSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	GTSAPP_SERVICE_ENUM stype =( GTSAPP_SERVICE_ENUM)s->get_serviceimpl();
	
	if( stype == GTSAPP_SERVICE_CSS)
	{
		CSS_Service* ps =dynamic_cast<CSS_Service*>(s);

		CSSLink* l =get_csslink( ps->server_index_);
		if( l && l->is_waitregistack())
		{
			l->set_status( SVRREGIST_ST_SUCC);
			l->set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to css:[%d] success.....", l->opt_->sn_);
		}

		autorelease =false;
	}
	else if( stype == GTSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>( s);

		CTSLink* l =get_ctslink();
		if( l && l->is_waitregistack())
		{
			l->set_status( SVRREGIST_ST_SUCC);
			l->set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to cts success.....");
		}

		autorelease =false;
	}
}

void GTSSvr::service_disconnected( BasicService* s)
{
	GTSAPP_SERVICE_ENUM stype =( GTSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == GTSAPP_SERVICE_CSS)
	{
		CSS_Service* ps =dynamic_cast<CSS_Service*>(s);

		CSSLink* l =get_csslink( ps->server_index_);
		if( l)
		{
			MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] disconnected with me....!", l->opt_->sn_);

			if( l->is_connected())
				svr_cssdisconnect( l);

			l->reset();
		}
	}
	else if( stype == GTSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>( s);

		CTSLink* l =get_ctslink();

		MODULE_LOG_WARN( MODULE_BOOT, "++++cts disconnected with me....!");

		if( l->is_connected())
			svr_ctsdisconnect( l);

		l->reset();
	}
	
}

void GTSSvr::svr_cssdisconnect( CSSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to css. now, clear system resource......");
}

void GTSSvr::svr_ctsdisconnect( CTSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to cts. now, clear system resource......");
}

void GTSSvr::close_all_service()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

Player* GTSSvr::get_emptyplayer()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	Player* player =static_cast<Player*>(cache_players_.malloc());
	if( player == 0)
		return 0;

	try
	{
		new(player)Player();
	}
	catch( ...)
	{
		cache_players_.free( player);
		return 0;
	}
	
#ifdef CORELIB_WIN32
	player->proactor( GTSMODULE->proactor());
#else
	player->reactor( GTSMODULE->reactor());
#endif

	player->prepare();

	//加入使用玩家队列
	used_players_.push_back( player);

	return player;
}

void GTSSvr::release_player( Player* p)
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	//复位信息
	if( p->team_proxy_)
		p->team_proxy_->reset();

	USED_PLAYERS_VECTOR::iterator fiter =NS_STL::find( used_players_.begin(), used_players_.end(), p);
	if( fiter != used_players_.end())
		used_players_.erase( fiter);

	p->~Player();
	cache_players_.free( (void*)p);
}

TeamProxySession* GTSSvr::team_proxy( S_INT_32 proxyind, S_INT_32 userid, S_TIMESTAMP st)
{
	TeamProxySession* ret =get_playerproxybyind( proxyind);
	if( ret == 0 || !ret->is_freeproxy())
		return 0;

	ret->proxy( userid, st);

	return ret;
}

void GTSSvr::player_linkkeep( Player* p)
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

	//复位信息
	p->team_proxy_->lnkkeep_reset();

	//从使用队列中移出
	USED_PLAYERS_VECTOR::iterator fiter =NS_STL::find( used_players_.begin(), used_players_.end(), p);
	if( fiter != used_players_.end())
		used_players_.erase( fiter);

	p->~Player();
	cache_players_.free( (void*)p);
}

void GTSSvr::player_disconnected( GTS_Service* ps)
{
	//处理断线
	if( ps->player_ == 0)
		return;

	//防止多次disconnect通知
	{
		ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());

		USED_PLAYERS_VECTOR::iterator fiter =NS_STL::find( used_players_.begin(), used_players_.end(), ps->player_);
		if( fiter == used_players_.end())
			return;
	}

	//选服成功
	if( ps->player_->is_ingame())
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from client. user:[%d], and will keep living in 5 minutes.", ps->player_->uuid_.userid_);

		//断线保持注册
		PRO::Pro_SvrDisconnKeep_req *req =PROTOCOL_NEW PRO::Pro_SvrDisconnKeep_req();
		PRO_UUID_FILL( req, ps->player_->global_index_, ps->player_->uuid_);
		req->token_ =ps->player_->team_proxy_->token_;
		//保存的gts索引
		req->gts_id_ =GLOBALCONFIG_INS->get_appsn();
		req->proxy_index_ =ps->player_->team_proxy_->proxy_index_;

		this->send_to_cts( req);

		//选线成功之后断线，提供断线保持
		player_linkkeep( ps->player_);
	}
	else
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from client. user:[%d]", ps->player_->uuid_.userid_);

		//其他情况走正常的断线流程
		if( ps->player_->is_selteamsucc())
		{
			PRO::Pro_SvrUserLost_NTF* ntf =PROTOCOL_NEW PRO::Pro_SvrUserLost_NTF();
			PRO_UUID_FILL( ntf, ps->player_->global_index_, ps->player_->uuid_)

			this->send_to_cts( ntf);
		}

		//释放玩家资源
		release_player( ps->player_);
	}
}
