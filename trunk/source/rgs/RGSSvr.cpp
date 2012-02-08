/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGSSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/db/MustReturnContext.h>
#include <corelib/db/NotReturnContext.h>

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/RGSOption.h>

#include <algorithm>
#include <memory>

RGSSvr::RGSSvr(void)
:player_nums_( 0),
lgssvr_nums_( 0),
ctssvr_nums_( 0)
{
}

RGSSvr::~RGSSvr(void)
{
}

//---------------------------server管理----------------------------//
void RGSSvr::init_svr()
{
	RGSOption* opt =GLOBALCONFIG_INS->get_rgsoption();

	player_nums_ =opt->max_users_;
	players_.reset( FRAMEWK_NEW PlayerInfo[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		noused_players_.push_back( &players_[ii]);
		players_[ii].clear();
	}

	lgssvr_nums_ =GLOBALCONFIG_INS->get_lgsnums();
	lgssvr_.reset( FRAMEWK_NEW LGSLink[lgssvr_nums_]);

	ctssvr_nums_ =GLOBALCONFIG_INS->get_svrteamnums();
	ctssvr_.reset( FRAMEWK_NEW CTSLink[ctssvr_nums_]);

	//svrteam update initialize
	svr_update_.init_svrteaminfo();
}

void RGSSvr::uninit_svr()
{
	userid_players_.clear();
	noused_players_.clear();
	proxy_players_.clear();
}

void RGSSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	RGSAPP_SERVICE_ENUM stype =( RGSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == RGSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =dynamic_cast<RGS_Service*>( s);
		if( ps->type_ == NETSVR_LGS)
		{
			LGSLink* l =get_lgslink( ps->server_index_);
			if( l == 0)
				return;

			l->set_linkservice( ps);

			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++lgs:[%d] regist to me....!", l->sn_);
		}
		else if( ps->type_ == NETSVR_CTS)
		{
			CTSLink* l =get_ctslink( ps->server_index_);
			if( l == 0)
				return;

			l->set_linkservice( ps);

			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++cts:[%d] regist to me....!", l->sn_);
		}
		else
			return;

		//发送回复
		PRO::Pro_SvrRegist_ack* ack =PROTOCOL_NEW PRO::Pro_SvrRegist_ack();
		ack->result_ =0;
		s->append_write_msg( ack);
	}
}

void RGSSvr::service_disconnected( BasicService* s)
{
	RGSAPP_SERVICE_ENUM stype =( RGSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == RGSAPP_SERVICE_RGS)
	{
		RGS_Service* ps =(RGS_Service*)s;

		//清楚关联注册信息
		if( ps->type_ == NETSVR_LGS)
		{
			LGSLink* l =get_lgslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++lgs:[%d] disconnected with me....!", l->sn_);

				if( l->is_connected())
					svr_lgsdisconnect( l);

				l->reset();
			}
		}
		else if( ps->type_ == NETSVR_CTS)
		{
			CTSLink* l =get_ctslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++cts:[%d] disconnected with me....!", l->sn_);

				if( l->is_connected())
					svr_ctsdisconnect( l);

				l->reset();
			}
		}
	}
}

void RGSSvr::svr_lgsdisconnect( LGSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to lgs. now, clear system resource......");
}

void RGSSvr::svr_ctsdisconnect( CTSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to cts. now, clear system resource......");
}

void RGSSvr::svr_controller_disconnect()
{
	controller_svr_.reset();
}

void RGSSvr::broadcast_to_lgs( BasicProtocol *p)
{
	for( int ss =1; ss < lgssvr_nums_; ++ss)
	{
		if( lgssvr_[ss].is_connected())
			lgssvr_[ss].send_protocol( p->clone());
	}

	if( lgssvr_[0].is_connected())
		lgssvr_[0].send_protocol( p);
	else
		delete p;
}

void RGSSvr::broadcast_to_cts( BasicProtocol* p)
{
	for( int ss =1; ss < ctssvr_nums_; ++ss)
	{
		if( ctssvr_[ss].is_connected())
			ctssvr_[ss].send_protocol( p->clone());
	}

	if( ctssvr_[0].is_connected())
		ctssvr_[0].send_protocol( p);
	else
		delete p;
}

void RGSSvr::close_all_service()
{
	//ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

PlayerInfo* RGSSvr::find_byuserid( int userid)
{
	USEDUSER_MAP::iterator fiter =userid_players_.find( userid);
	if( fiter == userid_players_.end())
		return 0;

	return fiter->second;
}

bool RGSSvr::unregist_logonuser( PlayerInfo *p)
{
	if( userid_players_.find( p->uuid_.userid_) == userid_players_.end())
		return false;

	userid_players_.erase( p->uuid_.userid_);
	p->clear();

	noused_players_.push_back( p);

	return true;
}

bool RGSSvr::regist_player( PlayerInfo* p)
{
	EMPTYUSER_LIST::iterator fiter =NS_STL::find( proxy_players_.begin(), proxy_players_.end(), p);
	if( fiter == proxy_players_.end())
		return false;

	proxy_players_.erase( fiter);

	userid_players_[p->uuid_.userid_] =p;

	p->login_succ();

	return true;
}

void RGSSvr::free_player( PlayerInfo* p)
{
	userid_players_.erase( p->uuid_.userid_);

	EMPTYUSER_LIST::iterator fiter =NS_STL::find( proxy_players_.begin(), proxy_players_.end(), p);
	if( fiter != proxy_players_.end())
		proxy_players_.erase( fiter);

	fiter =NS_STL::find( noused_players_.begin(), noused_players_.end(), p);
	if( fiter == noused_players_.end())
		noused_players_.push_back( p);

	p->clear();
}

PlayerInfo* RGSSvr::get_emptyplayer()
{
	if( noused_players_.size() == 0)
		return 0;

	PlayerInfo* ret =noused_players_.front();
	noused_players_.pop_front();

	ret->prepare();

	proxy_players_.push_back( ret);

	return ret;
}

bool RGSSvr::free_emptyplayer( PlayerInfo* p)
{
	EMPTYUSER_LIST::iterator fiter =NS_STL::find( proxy_players_.begin(), proxy_players_.end(), p);
	if( fiter == proxy_players_.end())
		return false;

	proxy_players_.erase( fiter);

	noused_players_.push_back( p);

	p->clear();

	return true;
}

void RGSSvr::fore_playerlogout( PlayerInfo* p)
{
	if( p == 0)
		return;

	MODULE_LOG_DEBUG( MODULE_TEMP, "rgs force user:[%d] logout", p->uuid_.userid_);

	USE_PROTOCOL_NAMESPACE;

	//转发到cts,如果已经选区的话
	Pro_SvrUserLost_NTF* ntf =PROTOCOL_NEW Pro_SvrUserLost_NTF();

	//是否没有完成选服
	if( !p->is_selteamconfirm())
	{
		PROTOCOL_UUID_FILL( ntf, p->server_index_, p->uuid_);
		p->send_to_lgs( ntf->clone());
	}

	PROTOCOL_UUID_FILL( ntf, NO_INITVALUE, p->uuid_);
	p->send_to_cts( ntf);

	this->free_player( p);
}
