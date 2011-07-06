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

#include "actions/LoginAction.h"

#include "RGSModule.h"

USE_PROTOCOL_NAMESPACE

void RGSSvr::player_lnkkeeplogout( PlayerInfo* user)
{
	//发送断线通知
	Pro_SvrUserLost_NTF* ntf =PROTOCOL_NEW Pro_SvrUserLost_NTF();
	PROTOCOL_UUID_FILL( ntf, user->server_index_, user->uuid_);
	user->send_to_cts( ntf);

	this->free_player( user);
}

void RGSSvr::svr_teaminfo_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>( p);
	svr_update_.update_teaminfo( ntf);
}

void RGSSvr::lgs_login_req( BasicProtocol* p, bool& autorelease, S_INT_32 server_index)
{
	Pro_Login_req* req =dynamic_cast<Pro_Login_req*>(p);
	LGSLink* l =this->get_lgslink( server_index);
	if( l == 0)
		return;
	
	PlayerInfo* user =get_emptyplayer();
	if( user == 0)
	{
		Pro_Login_ack* ack =PROTOCOL_NEW Pro_Login_ack();
		PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
		ack->result_ =5;
		l->send_protocol( ack);

		return;
	}

	//设置登陆准备状态
	user->login_prepare( req->get_uuidglobalindex(), req->get_uuidinitstmp(), l);

	//准备执行环境
	LoginAction* la =DATABASE_NEW LoginAction( user->uuid_.init_timestamp_);
	la->token_ =(NS_STL::string)req->token_;
	la->player_ =user;
	la->uuid_ =user->uuid_;

	//放入验证等待队列
	MustReturnContext<LoginAction>* rc =TASKCMD_NEW MustReturnContext<LoginAction>( la);
	RGSMODULE->append_returntask( rc);
}

void RGSSvr::lgs_userlost_ntf( BasicProtocol* p, bool& autorelease, S_INT_32 server_index)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	//完成选服后断开没有影响, 去除断线保持状态
	if( user->is_selteamconfirm() && !user->is_lnkkeep_wait())
		return;

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from lgs. user:[%d]", p->uuid_.userid_);

	//转发到cts,如果已经选区的话
	Pro_SvrUserLost_NTF* ntf =dynamic_cast<Pro_SvrUserLost_NTF*>(p);
	user->send_to_cts( ntf);
	autorelease =false;

	this->free_player( user);
}

void RGSSvr::cts_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts. user:[%d]", p->uuid_.userid_);

	//选服完成后不需要通知lgs
	if( !user->is_selteamconfirm())
	{
		Pro_SvrUserLost_NTF* ntf =dynamic_cast<Pro_SvrUserLost_NTF*>(p);
		user->send_to_lgs( ntf);
		autorelease =false;
	}

	//清除玩家信息,如果是退服导致的userlost不注销玩家
	if( !user->is_quitteam())
		this->free_player( user);
}

void RGSSvr::cts_lnkkeep_ntf( BasicProtocol* p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrDisconnKeep_req *req =dynamic_cast<Pro_SvrDisconnKeep_req*>(p);
	user->link_keep( req->token_, req->team_id_, req->gts_id_, req->proxy_index_);
}

void RGSSvr::cts_teamtimeout_ntf( BasicProtocol* p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	user->selteam_timeout();

	user->send_to_lgs( p);
	autorelease =false;
}

void RGSSvr::lgs_selteam_req( BasicProtocol* p, bool& autorelease, S_INT_32 server_index)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrSelTeam_req* req =dynamic_cast<Pro_SvrSelTeam_req*>(p);
	CTSLink* l =this->get_ctslink( req->team_index_);
	if( !l->is_connected())
	{
		//服务器组不可用
		Pro_SvrSelTeam_ack* ack =PROTOCOL_NEW Pro_SvrSelTeam_ack();
		PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
		ack->team_index_ =req->team_index_;
		ack->result_ =1;

		user->send_to_lgs( ack);
		return;
	}

	user->selteam_begin( l);

	//转发到cts
	user->send_to_cts( req);
	autorelease =false;
}

void RGSSvr::cts_selteam_ack( BasicProtocol*p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrSelTeam_ack* ack =dynamic_cast<Pro_SvrSelTeam_ack*>(p);
	user->selteam_end( ack->result_ == 0);

	user->send_to_lgs( ack);
	autorelease =false;
}

void RGSSvr::cts_selteamconfirm_ack( BasicProtocol* p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	Pro_SvrSelTeamConfirm_ack* ack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);
	user->send_to_lgs( ack);
	autorelease =false;

	//修改状态
	if( user->is_lnkkeep_wait())
		user->lnkkeep_confirm();
	else
		user->selteam_confirm();
}

void RGSSvr::cts_quitteam_req( BasicProtocol*p, bool& autorelease)
{
	RGS_GETPLAYER_FROMCACHE( user, p);

	CTSLink* cts =user->cts_link_;
	user->quit_team();

	//发送退出指令
	Pro_SvrQuitTeam_ack* ack =PROTOCOL_NEW Pro_SvrQuitTeam_ack();
	PROTOCOL_UUID_FILL( ack, user->server_index_, user->uuid_)
	ack->result_ =0;
	ack->token_ =user->qt_token_;

	cts->send_protocol( ack);
}

void RGSSvr::lgs_quitteamreconn_req( BasicProtocol*p, bool& autorelease, S_INT_32 server_index)
{
	Pro_SvrQuitTeamReconn_req* req =dynamic_cast<Pro_SvrQuitTeamReconn_req*>(p);
	PlayerInfo* user =find_byuserid( req->userid_);

	S_INT_8 ret =0;
	if( user == 0)
		ret =1;
	else
	{
		if( req->token_ != user->qt_token_)
			ret =2;
		else if( !user->is_quitteam())
			ret =3;
	}

	LGSLink* l =this->get_lgslink( server_index);

	if( ret == 0)
	{
		user->quitteam_reconn( req->get_uuidglobalindex(), req->get_uuidinitstmp(), l);
	}

	Pro_SvrQuitTeamReconn_ack* ack =PROTOCOL_NEW Pro_SvrQuitTeamReconn_ack();
	PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp())
	ack->result_ =ret;
	if( ret == 0)
		ack->userid_ =user->uuid_.userid_;

	l->send_protocol( ack);
}
