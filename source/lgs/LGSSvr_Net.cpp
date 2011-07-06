/**
* lgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGSSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "LGSModule.h"

USE_PROTOCOL_NAMESPACE

void LGSSvr::net_user_loginreq( BasicProtocol* p, bool& autorelease)
{
	Player* user =get_player( p->get_uuidglobalindex());
	if( user == 0 || !user->can_dologin())
		return;

	user->login_begin();

	Pro_Login_req* req =dynamic_cast<Pro_Login_req*>( p);
	this->send_to_rgs( req);
	autorelease =false;
}

void LGSSvr::rgs_user_loginack( BasicProtocol* p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	Pro_Login_ack* ack =dynamic_cast< Pro_Login_ack*>( p);
	user->login_end( ack);

	//转发协议
	user->send_protocol( ack);
	autorelease =false;

	//断线保持状态，不需要进入选服流程
	if( user->is_logon() && !user->is_lnkkeep())
		user->notify_svrteaminfo();
}

void LGSSvr::rgs_teaminfo_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>( p);
	svr_update_.update_teaminfo( ntf);
}

void LGSSvr::rgs_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	user->close_service_nonotify();
	this->release_player( user);

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from rgs. user:[%d]", p->uuid_.userid_);
}

void LGSSvr::rgs_teamtimeout_ntf( BasicProtocol* p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	user->selteam_timeout();

	user->send_protocol( p);
	autorelease =false;
}

void LGSSvr::net_team_selreq( BasicProtocol*p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	S_INT_8 ret =0;
	//断线保持不能选服
	if( user->is_lnkkeep())
		ret =5;

	Pro_SvrSelTeam_req* req =dynamic_cast<Pro_SvrSelTeam_req*>(p);
	if( !svr_update_.is_teamexist( req->team_index_))
		ret =1;

	if( ret != 0)
	{
		Pro_SvrSelTeam_ack* ack =PROTOCOL_NEW Pro_SvrSelTeam_ack();
		ack->team_index_ =req->team_index_;
		ack->result_ =ret;

		user->send_protocol( ack);
		return;
	}

	user->selteam_begin();

	//转发请求
	this->send_to_rgs( req);
	autorelease =false;
}

void LGSSvr::rgs_team_selack( BasicProtocol*p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrSelTeam_ack* ack =dynamic_cast<Pro_SvrSelTeam_ack*>(p);

	user->selteam_end( ack->result_ == 0);

	//转发协议
	user->send_protocol( ack);
	autorelease =false;
}

void LGSSvr::rgs_team_confirmack( BasicProtocol*p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	user->selteam_confirm();

	/*
	Pro_SvrSelTeamConfirm_ack* ack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);
	user->send_protocol( ack);
	autorelease =false;
	*/
}

void LGSSvr::net_quitteamreconn_req( BasicProtocol* p, bool& autorelease)
{
	Player* user =get_player( p->get_uuidglobalindex());
	if( user == 0 || !user->can_doteamreconn())
		return;

	//开始登陆重连请求
	user->quitteam_reconn_begin();

	Pro_SvrQuitTeamReconn_req* req =dynamic_cast<Pro_SvrQuitTeamReconn_req*>( p);
	this->send_to_rgs( req);
	autorelease =false;
}

void LGSSvr::rgs_quitteamreconn_ack( BasicProtocol* p, bool& autorelease)
{
	LGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrQuitTeamReconn_ack *ack =dynamic_cast<Pro_SvrQuitTeamReconn_ack*>( p);
	if( ack->result_ == 0)
		user->quitteam_reconn( ack->userid_);

	user->send_protocol( ack);
	autorelease =false;

	if( user->is_logon())
		user->notify_svrteaminfo();
}
