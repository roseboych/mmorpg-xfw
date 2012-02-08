/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTSSvr.h"

#include <prolib/Pro_all.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/RGSOption.h>
#include <reslib/deploy/DPXOption.h>

#include <algorithm>
#include <memory>

CTSSvr::CTSSvr(void):
csssvr_nums_(0),
gtssvr_nums_(0),
player_nums_(0),
owner_svrteam_(0)
{
}

CTSSvr::~CTSSvr(void)
{
}

void CTSSvr::init_svr()
{
	//rgs
	rgssvr_.set_option( GLOBALCONFIG_INS->get_rgsoption());

	owner_svrteam_ =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());

	cts_serverindex_ =owner_svrteam_->server_index_;
	cts_teamid_ =owner_svrteam_->sn_;

	//dpx
	dpxsvr_.set_option( &(owner_svrteam_->dpxopt_));

	//css
	csssvr_nums_ =owner_svrteam_->get_cssnums();
	csssvr_.reset( FRAMEWK_NEW CSSLink[csssvr_nums_]);

	NS_STL::list< CSSOption*> css;
	owner_svrteam_->get_cssoptions( css);
	for( NS_STL::list< CSSOption*>::iterator iter =css.begin(); iter != css.end(); ++iter)
	{
		CSSOption* opt =(*iter);
		CSSLink* l =&csssvr_[opt->server_index_];
		l->set_option( opt);
	}

	//gts
	gtssvr_nums_ =owner_svrteam_->get_gtsnums();
	gtssvr_.reset( FRAMEWK_NEW GTSLink[gtssvr_nums_]);

	NS_STL::list< GTSOption*> gts;
	owner_svrteam_->get_gtsoptions( gts);
	for( NS_STL::list< GTSOption*>::iterator iter =gts.begin(); iter != gts.end(); ++iter)
	{
		GTSOption* opt =(*iter);
		GTSLink* l =&gtssvr_[opt->server_index_];
		l->set_option( opt);
	}

	//players
	player_nums_ =owner_svrteam_->maxuser_;
	players_.reset( FRAMEWK_NEW Player[player_nums_]);
	team_proxys_.reset( FRAMEWK_NEW TeamProxySession[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		players_[ii].global_index_ =ii;
		players_[ii].gts_link_ =get_gtslinkbysindex( players_[ii].global_index_);
		players_[ii].reset();

		team_proxys_[ii].reset();
		team_proxys_[ii].proxy_index_ =ii;
		team_proxys_[ii].gts_link_ =players_[ii].gts_link_;
		empty_proxys_.push_back( &team_proxys_[ii]);
	}

	login_players_.clear();

	//ins cell
	instcell_mgr_.initialize( owner_svrteam_);
}

void CTSSvr::uninit_svr()
{
	login_players_.clear();
	empty_proxys_.clear();
	wait_proxys_.clear();
}

void CTSSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	CTSAPP_SERVICE_ENUM stype =( CTSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == CTSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =dynamic_cast<RGS_Service*>(s);
		if( rgssvr_.is_waitregistack())
		{
			rgssvr_.set_status( SVRREGIST_ST_SUCC);
			rgssvr_.set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to rgs success.....");
		}

		autorelease =false;
	}
	else if( stype == CTSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>(s);
		if( ps->type_ == NETSVR_CSS)
		{
			CSSLink* l =get_csslink( ps->server_index_);
			if( l == 0)
				return;

			l->set_linkservice( ps);
			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] regist to me....!", l->opt_->sn_);
		}
		else if( ps->type_ == NETSVR_GTS)
		{
			GTSLink* l =get_gtslink( ps->server_index_);
			if( l == 0)
				return;

			l->set_linkservice( ps);
			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++gts:[%d] regist to me....!", l->opt_->sn_);
		}
		else if( ps->type_ == NETSVR_DPX)
		{
			dpxsvr_.set_linkservice( ps);
			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++dpx regist to me....!");
		}
		else
			return;

		PRO::Pro_SvrRegist_ack* ack =PROTOCOL_NEW PRO::Pro_SvrRegist_ack();
		ack->result_ =0;
		s->append_write_msg( ack);
	}
}

void CTSSvr::service_disconnected( BasicService* s)
{
	CTSAPP_SERVICE_ENUM stype =( CTSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == CTSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =dynamic_cast<RGS_Service*>(s);
		if( rgssvr_.is_connected())
		{
			svr_rgsdisconnect();

			MODULE_LOG_WARN( MODULE_BOOT, "++++rgs disconnected with me....!");
		}
		rgssvr_.reset();
	}
	else if( stype == CTSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>(s);

		if( ps->type_ == NETSVR_DPX)
		{
			if( dpxsvr_.is_connected())
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++dpx disconnected with me....!");
				svr_dpxdisconnect();
			}
			dpxsvr_.reset();
		}
		else if( ps->type_ == NETSVR_CSS)
		{
			CSSLink* l =get_csslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] disconnected with me....!", l->opt_->sn_);

				if( l->is_connected())
					svr_cssdisconnect( l);

				l->reset();
			}
		}
		else if( ps->type_ == NETSVR_GTS)
		{
			GTSLink* l =get_gtslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++gts:[%d] disconnected with me....!", l->opt_->sn_);

				if( l->is_connected())
					svr_gtsdisconnect( l);

				l->reset();
			}
		}
	}
}

void CTSSvr::svr_rgsdisconnect()
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to rgs. now, clear system resource......");
}

void CTSSvr::svr_gtsdisconnect( GTSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to gts. now, clear system resource......");

}

void CTSSvr::svr_dpxdisconnect()
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to dpx. now, clear system resource......");

}

void CTSSvr::svr_cssdisconnect( CSSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to css. now, clear system resource......");
}

void CTSSvr::close_all_service()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

void CTSSvr::send_to_rgs( BasicProtocol* p)
{
	rgssvr_.send_protocol( p);
}

void CTSSvr::send_to_dpx( BasicProtocol* p)
{
	dpxsvr_.send_protocol( p);
}

void CTSSvr::broadcast_to_gts( BasicProtocol* p)
{
	if( p == 0)
		return;

	for( int ii =1; ii < gtssvr_nums_; ++ii)
	{
		if( !gtssvr_[ii].is_connected())
			continue;

		gtssvr_[ii].send_protocol( p->clone());
	}

	if( gtssvr_[0].is_connected())
		gtssvr_[0].send_protocol( p);
	else
		delete p;
}

void CTSSvr::broadcast_to_css( BasicProtocol* p)
{
	if( p == 0)
		return;

	for( int ii =1; ii < csssvr_nums_; ++ii)
	{
		if( !csssvr_[ii].is_connected())
			continue;
		csssvr_[ii].send_protocol( p->clone());
	}

	if( csssvr_[0].is_connected())
		csssvr_[0].send_protocol( p);
	else
		delete p;
}

TeamProxySession* CTSSvr::get_playerproxy( int userid, S_TIMESTAMP st, int sindex)
{
	if( empty_proxys_.size() == 0)
		return 0;

	TeamProxySession* ret =empty_proxys_.front();
	empty_proxys_.pop_front();

	ret->proxy( userid, st, sindex);

	wait_proxys_[userid] =ret;

	return ret;
}

TeamProxySession* CTSSvr::is_playerproxy( int userid)
{
	TEAMPROXY_MAP::iterator fiter =wait_proxys_.find( userid);
	if( fiter == wait_proxys_.end())
		return 0;

	return fiter->second;
}

void CTSSvr::free_waitproxy( int userid)
{
	TEAMPROXY_MAP::iterator fiter =wait_proxys_.find( userid);
	if( fiter == wait_proxys_.end())
		return;

	TeamProxySession* proxy =fiter->second;
	wait_proxys_.erase( fiter);

	proxy->reset();

	empty_proxys_.push_back( proxy);
}

void CTSSvr::free_player( Player* user)
{
	login_players_.erase( user->uuid_.userid_);

	TeamProxySession* proxy =user->proxy_session_;
	if( proxy)
	{
		proxy->reset();
		empty_proxys_.push_back( proxy);
	}

	user->reset();
}
