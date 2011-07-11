/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTSSvr.h"

#include <prolib/Pro_all.h>
#include "GTSModule.h"

USE_PROTOCOL_NAMESPACE

void GTSSvr::cts_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrUserLost_NTF* ntf =dynamic_cast<Pro_SvrUserLost_NTF*>(p);
	TeamProxySession* proxy =this->get_playerproxybyind( ntf->proxy_index_);
	if( proxy == 0 || !p->same_session( proxy->uuid_))
		return;

	if( proxy->player_)
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[no proxy]. user:[%d]", p->uuid_.userid_);

		proxy->player_->close_service_nonotify();
		this->release_player( proxy->player_);
	}
	else
	{
		if( proxy->is_lnkkeep())
		{
			MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[proxy]. user:[%d], and living keep finish.", p->uuid_.userid_);
		}
		else
		{
			MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[proxy]. user:[%d]", p->uuid_.userid_);
		}

		proxy->reset();
	}
}

void GTSSvr::cts_teamtimeout_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrSelTeamTimeout_NTF* ntf =dynamic_cast<Pro_SvrSelTeamTimeout_NTF*>(p);
	TeamProxySession* proxy =this->get_playerproxybyind( ntf->proxy_index_);
	if( proxy == 0 || !p->same_session( proxy->uuid_))
		return;

	if( proxy->player_)
	{
		proxy->player_->close_service_nonotify();
		this->release_player( proxy->player_);
	}
	else
	{
		proxy->reset();
	}
}

void GTSSvr::net_userlogout_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//设置注销标志
	user->status_ =PLAYERSTATUS_LOGOUT;
	user->selteam_tmpstamp_ =ACE_OS::gettimeofday().get_msec();
}

void GTSSvr::cts_teamproxy_req( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrSelTeamProxy_req* req =dynamic_cast<Pro_SvrSelTeamProxy_req*>(p);
	
	TeamProxySession* proxy =team_proxy( req->proxy_index_, req->get_uuiduserid(), req->get_uuidinitstmp());
	if( proxy == 0)
	{
		Pro_SvrSelTeamProxy_ack* ack =PROTOCOL_NEW Pro_SvrSelTeamProxy_ack();
		PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
		ack->result_ =1;

		this->send_to_cts( ack);
		return;
	}

	Pro_SvrSelTeamProxy_ack* ack =PROTOCOL_NEW Pro_SvrSelTeamProxy_ack();
	PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
	ack->proxy_index_ =proxy->proxy_index_;
	ack->result_ =0;
	ack->userid_ =proxy->uuid_.userid_;
	ack->token_ =proxy->token_;
	ack->gts_index_ =gts_index_;

	this->send_to_cts( ack);
}

void GTSSvr::net_proxyconfirm_req( BasicProtocol*p, bool& autorelease, void* pdata)
{
	Player* user =(Player*)pdata;
	if( user == 0)
		return;

	Pro_SvrSelTeamConfirm_req* req =dynamic_cast<Pro_SvrSelTeamConfirm_req*>(p);
	TeamProxySession* proxy =this->get_playerproxybyind( req->proxy_index_);

	Pro_SvrSelTeamConfirm_ack* ack =PROTOCOL_NEW Pro_SvrSelTeamConfirm_ack();
	ack->proxy_index_ =req->proxy_index_;
	if( proxy == 0)
	{
		ack->result_ =1;
		user->send_protocol( ack);
		return;
	}
	else
	{
		if( !proxy->is_waitconfirm() && !proxy->is_lnkkeep())
		{
			ack->result_ =1;
			user->send_protocol( ack);
			return;
		}

		if( !proxy->confirm_token( req->token_))
		{
			ack->result_ =2;
			user->send_protocol( ack);
			return;
		}
	}

	if( proxy->is_lnkkeep())
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "user:[%d] have actived the lost living in 5 minutes.", proxy->uuid_.userid_);
	}

	//确认
	user->proxy_confirm( proxy);

	PRO_UUID_FILL( ack, user->global_index_, user->uuid_)
	ack->result_ =0;
	ack->userid_ =proxy->uuid_.userid_;
	ack->init_stmp_ =proxy->uuid_.init_timestamp_;

	//直接回复给玩家
	user->send_protocol( ack->clone());

	//转发给cts完成注册
	this->send_to_cts( ack);

	//发送给css请求数据
	Pro_SvrLnkkeepLoad_req *req2 =PROTOCOL_NEW Pro_SvrLnkkeepLoad_req();
	PRO_UUID_FILL( req2, user->global_index_, user->uuid_);
	user->send_to_css( req2);
}

void GTSSvr::net_quitteam_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//设置退服标志
	user->status_ =PLAYERSTATUS_OUTTEAM;
	this->send_to_cts( p);
	autorelease =false;
}

void GTSSvr::cts_quitteam_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrQuitTeam_ack *ack =dynamic_cast<Pro_SvrQuitTeam_ack*>( p);
	if( ack->result_ != 0)
		user->status_ =PLAYERSTATUS_INGAME;
	else
	{
		user->status_ =PLAYERSTATUS_OUTTEAM_FIN;
		user->selteam_tmpstamp_ =ACE_OS::gettimeofday().get_msec();
	}

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_chrlist_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//状态过滤
	if( !user->is_inteam())
		return;

	this->send_to_cts( p);
	autorelease =false;
}

void GTSSvr::cts_chrlist_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_chrsel_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//状态过滤
	if( !user->is_inteam())
		return;

	this->send_to_cts( p);
	autorelease =false;
}

void GTSSvr::cts_chrsel_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrSel_ack* ack =dynamic_cast<Pro_ChrSel_ack*>(p);
	if( ack->result_ == 0)
		user->set_curchrid( ack->chrid_);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_chradd_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//状态过滤
	if( !user->is_inteam())
		return;

	this->send_to_cts( p);
	autorelease =false;
}

void GTSSvr::cts_chradd_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_chrdel_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	//状态过滤
	if( !user->is_inteam())
		return;

	this->send_to_cts( p);
	autorelease =false;
}

void GTSSvr::cts_chrdel_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrDel_ack* ack =dynamic_cast<Pro_ChrDel_ack*>(p);
	if( ack->result_ == 0 && user->curchrid_ == ack->chrid_)
		user->set_curchrid( NO_INITVALUE);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_teleport_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	S_INT_8 ret =0;
	if( !user->is_ingame())
		ret =2;
	else if( user->is_instsvr_)
		ret =6;
	else if( user->is_switchcss_)
		ret =7;

	if( ret != 0)
	{
		Pro_AppTeleport_ack* ack =PROTOCOL_NEW Pro_AppTeleport_ack();
		ack->result_ =ret;
		user->send_protocol( ack);

		return;
	}

	user->is_switchcss_ =true;

	user->send_to_css( p);
	autorelease =false;
}

void GTSSvr::css_teleport_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_AppTeleport_ack* ack =dynamic_cast<Pro_AppTeleport_ack*>(p);
	if( ack->result_ == 0)
	{
		user->status_ =PLAYERSTATUS_INGAME;
		user->css_svr_ =this->get_csslink( ack->cssindex_);
	}

	user->is_switchcss_ =false;

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::css_chrreg2world_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrRegistToWorld_req* req =dynamic_cast<Pro_ChrRegistToWorld_req*>( p);

	//建立和css的关联
	user->css_svr_ =this->get_csslink( req->cssindex_);
	//该协议表示进入主世界地图
	user->is_instsvr_ =false;
}

void GTSSvr::css_chrload_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::css_chrfin_ntf( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_instenter_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	S_INT_8	ret =0;
	if( !user->is_ingame())
		ret =2;
	else if( user->is_instsvr_)
		ret =6;	//不能从副本进入副本
	else if( user->is_switchcss_)
		ret =8;

	if( ret != 0)
	{
		Pro_AppEnterIns_ack* ack =PROTOCOL_NEW Pro_AppEnterIns_ack();
		ack->result_ =ret;
		user->send_protocol( ack);

		return;
	}

	user->is_switchcss_ =true;

	user->send_to_css( p);
	autorelease =false;
}

void GTSSvr::css_instenter_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_AppEnterIns_ack* ack =dynamic_cast<Pro_AppEnterIns_ack*>( p);

	if( ack->result_ == 0)
	{
		//指向副本所在的css服务器
		user->css_svr_ =this->get_csslink( ack->cssindex_);
		user->is_instsvr_ =true;

		//发送进入副本确认
		Pro_AppEnterInsConfirm_ntf* ntf =PROTOCOL_NEW Pro_AppEnterInsConfirm_ntf();
		PRO_UUID_FILL2( ntf, ack);
		user->send_to_css( ntf);
	}

	user->is_switchcss_ =false;

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::net_instquit_req( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	if( !user->is_ingame() || !user->is_instsvr_ || user->is_switchcss_)
	{
		Pro_AppQuitInst_ack* ack =PROTOCOL_NEW Pro_AppQuitInst_ack();
		PRO_UUID_FILL2( ack, p);
		ack->result_ =1;
		user->send_protocol( ack);
		return;
	}

	user->is_switchcss_ =true;

	user->send_to_css( p);
	autorelease =false;
}

void GTSSvr::css_instquit_ack( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	Pro_AppQuitInst_ack* ack =dynamic_cast<Pro_AppQuitInst_ack*>( p);
	if( ack->result_ == 0)
	{
		user->css_svr_ =0;
		user->is_instsvr_ =false;
	}

	user->is_switchcss_ =false;

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::transfer_to_net( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_protocol( p);
	autorelease =false;
}

void GTSSvr::transfer_to_css( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	user->send_to_css( p);
	autorelease =false;
}

void GTSSvr::transfer_to_css_ingame( BasicProtocol* p, bool& autorelease)
{
	GTS_GETPLAYER_FROMCACHE( user, p);

	if( !user->is_ingame())
		return;

	user->send_to_css( p);
	autorelease =false;
}
