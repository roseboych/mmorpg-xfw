/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTSSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/GTSOption.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/world/StoryMapOption.h>

#include "CTSModule.h"
#include "InstCellState.h"

USE_PROTOCOL_NAMESPACE

void CTSSvr::rgs_userlost_ntf( BasicProtocol*p, bool& autorelease)
{
	Pro_SvrUserLost_NTF* ntf =dynamic_cast<Pro_SvrUserLost_NTF*>(p);

	TeamProxySession* proxy =this->is_playerproxy( p->get_uuiduserid());
	if( proxy)
	{
		//正在占位代理
		if( !proxy->is_samesession( p->get_uuiduserid(), p->get_uuidinitstmp()))
			return;

		MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from rgs[proxy]. user:[%d]", p->uuid_.userid_);

		ntf->proxy_index_ =proxy->proxy_index_;
		proxy->send_to_gts( p);
		autorelease =false;

		this->free_waitproxy( p->get_uuiduserid());
	}
	else
	{
		Player* user =get_playerbyid( p->get_uuiduserid());
		if( user == 0 || user->proxy_session_ == 0)
			return;
		if( !p->same_session( user->proxy_session_->uuid_))
			return;

		MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from rgs[no proxy]. user:[%d]", p->uuid_.userid_);

		//广播到css/gts/dpx
		//gts保持rgs的session
		ntf->proxy_index_ =user->proxy_session_->proxy_index_;
		user->send_to_gts( p->clone());

		PRO_UUID_FILL( p, user->global_index_, user->uuid_)
		user->send_to_css( p->clone());
		this->send_to_dpx( p);

		autorelease =false;

		//释放玩家
		this->free_player( user);
	}
}

void CTSSvr::gts_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from gts. user:[%d]", p->uuid_.userid_);

	user->send_to_css( p->clone());
	this->send_to_dpx( p->clone());

	if( user->proxy_session_)
	{
		PRO_UUID_FILL( p, user->proxy_session_->server_index_, user->proxy_session_->uuid_);
		this->send_to_rgs( p);
		autorelease =false;
	}

	this->free_player( user);
}

void CTSSvr::gts_lnkkeep_ntf( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrDisconnKeep_req *req =dynamic_cast<Pro_SvrDisconnKeep_req*>(p);
	PRO_UUID_FILL( p, user->proxy_session_->server_index_, user->proxy_session_->uuid_);
	req->team_id_ =this->cts_teamid_;

	//设置标志
	user->is_lnkkeep_ =true;

	this->send_to_rgs( req);
	autorelease =false;
}

void CTSSvr::css_instupdate_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrInstUpdate_ntf* ntf =dynamic_cast<Pro_SvrInstUpdate_ntf*>(p);

	InstCellState* pstate =instcell_mgr_.get_instcellbycellid( ntf->cellid_);
	if( pstate)
	{
		if( ntf->state_ == 1)
			pstate->set_state( TRANSCRIPT_SVRST_DISABLE);
		else if( ntf->state_ == 2)
			pstate->set_state( TRANSCRIPT_SVRST_CANENTER);
		else if( ntf->state_ == 3)
			pstate->set_state( TRANSCRIPT_SVRST_REJECTENTER);
	}
}

void CTSSvr::rgs_team_selreq( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrSelTeam_req* req =dynamic_cast<Pro_SvrSelTeam_req*>(p);

	S_INT_8 ret =0;
	Player* user =get_playerbyid( p->get_uuiduserid());
	if( user != 0)
		ret =4;

	TeamProxySession* proxy =0;
	if( ret == 0)
	{
		proxy =get_playerproxy( req->get_uuiduserid(), req->get_uuidinitstmp(), req->get_uuidglobalindex());
		if( proxy == 0)
			ret =2;
	}

	if( ret != 0)
	{
		Pro_SvrSelTeam_ack* ack =PROTOCOL_NEW Pro_SvrSelTeam_ack();
		PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
		ack->team_index_ =req->team_index_;
		ack->result_ =ret;
		
		this->send_to_rgs( ack);
		return;
	}

	//发送占位请求
	Pro_SvrSelTeamProxy_req* req2 =PROTOCOL_NEW Pro_SvrSelTeamProxy_req();
	PLAYERC_UUID_FILL( req2, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
	req2->proxy_index_ =proxy->proxy_index_;

	//查找对应的gts
	proxy->send_to_gts( req2);
}

void CTSSvr::gts_team_selproxyack( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrSelTeamProxy_ack* ack =dynamic_cast<Pro_SvrSelTeamProxy_ack*>(p);
	TeamProxySession* proxy =get_playerproxy( ack->proxy_index_);
	if( proxy == 0 || !proxy->is_samesession( p->get_uuiduserid(), p->get_uuidinitstmp()))
		return;

	Pro_SvrSelTeam_ack* ack2 =PROTOCOL_NEW Pro_SvrSelTeam_ack();
	PRO_UUID_FILL( ack2, proxy->server_index_, proxy->uuid_)

	if( ack->result_ == 0)
	{
		//success
		ack2->result_ =0;
		GTSOption* opt =this->get_gtslink( ack->gts_index_)->opt_;
		ack2->gts_ip_ =opt->ip_;
		ack2->gts_port_ =opt->port_;
		ack2->proxy_index_ =proxy->proxy_index_;
		ack2->token_ =ack->token_;

		proxy->proxy_ack( ack->token_);
	}
	else
	{
		//failure
		ack2->result_ =3;

		//释放占位
		this->free_waitproxy( ack->userid_);
	}

	this->send_to_rgs( ack2);
}

void CTSSvr::gts_team_confirmack( BasicProtocol* p, bool& autorelease)
{
	Player* user =get_player( p->get_uuidglobalindex());
	if( user == 0)
		return;

	Pro_SvrSelTeamConfirm_ack* ack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);
	TeamProxySession* proxy =get_playerproxy( ack->proxy_index_);

	if( user->is_lnkkeep())
	{
		//如果是断线保持
		if( proxy == 0 || !proxy->is_samesession( ack->userid_, ack->init_stmp_))
			return;

		user->is_lnkkeep_ =false;

		//填充原有的凭据
		PRO_UUID_FILL( ack, proxy->server_index_, proxy->uuid_);

		this->send_to_rgs( ack);
		autorelease =false;
	}
	else if( user->is_emptyplayer())
	{
		//新玩家注册
		if( proxy == 0 || !proxy->is_samesession( ack->userid_, ack->init_stmp_))
			return;

		//注册到登陆玩家列表，完成选服注册
		user->selteamconfirm( proxy, p->get_uuidinitstmp());
		login_players_[user->uuid_.userid_] =user;
		wait_proxys_.erase( proxy->uuid_.userid_);

		//填充原有的凭据
		PRO_UUID_FILL( ack, proxy->server_index_, proxy->uuid_);

		this->send_to_rgs( ack);
		autorelease =false;

		//注册到dpx
		Pro_UserRegist_ntf* rn =PROTOCOL_NEW Pro_UserRegist_ntf();
		PRO_UUID_FILL( rn, user->global_index_, user->uuid_);

		this->send_to_dpx( rn);
	}
}

void CTSSvr::gts_quitteam_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	//转发到rgs
	if( user->proxy_session_)
	{
		PRO_UUID_FILL( p, user->proxy_session_->server_index_, user->proxy_session_->uuid_);
		this->send_to_rgs( p);
		autorelease =false;
	}
}

void CTSSvr::rgs_quitteam_ack( BasicProtocol* p, bool& autorelease)
{
	Player* user =get_playerbyid( p->get_uuiduserid());
	if( user == 0 || user->proxy_session_ == 0)
		return;

	if( !p->same_session( user->proxy_session_->uuid_))
		return;

	PRO_UUID_FILL( p, user->global_index_, user->uuid_)
	user->send_to_gts( p);
	autorelease =false;
}

void CTSSvr::gts_chrlist_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	this->send_to_dpx( p);
	autorelease =false;
}

void CTSSvr::dpx_chrlist_ack( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	user->send_to_gts( p);
	autorelease =false;
}

void CTSSvr::gts_chradd_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrCreate_req* req =dynamic_cast<Pro_ChrCreate_req*>( p);

	//设置出生点坐标
	world_bornpos* bornpos =WORLDINFO->get_bornpos( req->race_);
	ACE_ASSERT( bornpos != 0);
	req->posx_ =bornpos->posx_;
	req->posy_ =bornpos->posy_;
	req->posz_ =bornpos->posz_;
	req->facing_ =bornpos->face_;

	this->send_to_dpx( p);
	autorelease =false;
}

void CTSSvr::dpx_chradd_ack( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	user->send_to_gts( p);
	autorelease =false;
}

void CTSSvr::gts_chrsel_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	this->send_to_dpx( p);
	autorelease =false;
}

void CTSSvr::dpx_chrsel_ack( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrSel_ack* ack =dynamic_cast<Pro_ChrSel_ack*>(p);
	if( ack->result_ == 0)
	{
		//完成关联设置
		user->chrsel( ack->chrid_, ack->lastposx_, ack->lastposy_, ack->lastposz_);

		StoryMapOption* opt =WORLDINFO->get_mainstorybyxy( ack->lastposx_, ack->lastposy_);
		ACE_ASSERT( opt != 0);

		user->css_link_ =this->get_csslink( opt->owner_css_->server_index_);

		//转换成地图坐标
		opt->world_to_local( ack->lastposx_, ack->lastposy_);
		ack->lastmap_ =opt->get_mapid();

		//发送注册请求
		Pro_ChrRegistToWorld_req* loadreq =PROTOCOL_NEW Pro_ChrRegistToWorld_req();
		loadreq->uuid_ =p->uuid_;
		loadreq->chrid_ =ack->chrid_;
		loadreq->mapid_ =ack->lastmap_;
		loadreq->cssindex_ =opt->owner_css_->server_index_;

		user->send_to_css( loadreq);
	}

	user->send_to_gts( p);
	autorelease =false;
}

void CTSSvr::gts_chrdel_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	this->send_to_dpx( p);
	autorelease =false;
}

void CTSSvr::dpx_chrdel_ack( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrDel_ack* ack =dynamic_cast<Pro_ChrDel_ack*>(p);
	if( ack->result_ == 0 && user->chrid_ == ack->chrid_)
		user->chrsel( NO_INITVALUE, NO_INITVALUE, NO_INITVALUE, NO_INITVALUE);

	user->send_to_gts( p);
	autorelease =false;
}

void CTSSvr::css_teleport_ack( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_AppTeleport_ack *ack =dynamic_cast<Pro_AppTeleport_ack*>(p);

	//重新建立关联
	user->css_link_ =this->get_csslink( ack->cssindex_);

	//发送回复
	ack->chrid_ =user->chrid_;

	user->send_to_css( ack);
	autorelease =false;
}

void CTSSvr::css_enterinst_req( BasicProtocol* p, bool& autorelease)
{
	CTS_GETPLAYER_FROMCACHE( user, p);

	Pro_AppEnterIns_req* req =dynamic_cast<Pro_AppEnterIns_req*>( p);

	S_INT_8 ret =0;
	InstCellState* pcell =0;
	if( user->is_enterinst())
		ret =3;	//已有一个进入请求
	else
	{
		ToTranscriptConfig& tconf =WORLDINFO->get_totranscriptconfig();
		transcript_teleport_info* pteleport =tconf.get_teleportinfobyid( req->telid_);
		ACE_ASSERT( pteleport != 0);
		pcell =instcell_mgr_.get_canenterinstcellbymapid( pteleport->instmap_id_);
		//没有可用的副本
		if( pcell == 0)
			ret =5;
	}

	if( ret != 0)
	{
		Pro_AppEnterIns_ack* ack =PROTOCOL_NEW Pro_AppEnterIns_ack();
		PRO_UUID_FILL2( ack, req);
		ack->result_ =ret;

		user->send_to_css( ack);
		return;
	}

	//记录临时状态
	user->enter_instcell_ctrl_.wait_cellproxy( req, pcell->get_csslink());

	//转发请求到副本服务器
	pcell->get_csslink()->send_protocol( p);
	autorelease =false;
}
