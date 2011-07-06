/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "AdapterPlayer.h"

#include <prolib/Pro_all.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/world/Teleport.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include "BaseStoryService.h"
#include "CSSModule.h"
#include "character/Player.h"

AdapterPlayer::AdapterPlayer():
insvr_( 0),
gts_link_( 0)
{
}

void AdapterPlayer::send_to_gts( BasicProtocol* p)
{
	if( gts_link_)
		gts_link_->send_protocol( p);
	else
		delete p;
}

void AdapterPlayer::reset()
{
	uuid_.reset();

	insvr_ =0;
	gts_link_ =0;
}

bool AdapterPlayer::is_freeplayer()
{
	return (insvr_ == 0);
}

void AdapterPlayer::player_regist2world( PRO::Pro_ChrRegistToWorld_req* req, bool& autorelease)
{
	CSSOption* opt =CSSMODULE->get_owneropt();
	ACE_ASSERT( opt->test_mapincss( req->mapid_));

	BaseStoryService* svr =CSSMODULE->get_storyservicebymap( req->mapid_);
	ACE_ASSERT( svr != 0);

	//保存值
	this->insvr_ =svr;
	this->gts_link_ =CSSMODULE->get_gtslinkbyuserglobal( global_index_);
	this->uuid_.set_uuid( req->get_uuiduserid(), req->get_uuidinitstmp());

	//注册到执行队列
	NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_chrreg2world_req, this->insvr_, _1, _2);
	NetCommand* pcmd =TASKCMD_NEW NetCommand( req, fun, true);
	insvr_->regist_netcmd( pcmd);
	autorelease =false;
}

void AdapterPlayer::player_teleportout( PRO::Pro_AppTeleport_req* req, bool& autorelease)
{
	ACE_ASSERT( this->insvr_ != 0);
	//验证转跳点
	S_INT_8 ret =0;

	TeleportPair* tp =WORLDINFO->get_teleportconfig().get_teleportpair( req->telid_);
	CSSOption* opt2 =0;
	teleport_info* t2 =0;
	if( tp == 0)
		ret =1;
	else
	{
		teleport_info* ti =tp->get_from();
		if( ti->ownermapid_ != insvr_->get_storyoption()->get_mapid())
			ret =4;
		t2 =tp->get_tobyid( req->teltoid_);
		if( t2 == 0)
			ret =1;
		else if( t2->ownermapid_ == insvr_->get_storyoption()->get_mapid())
			ret =3;
	}

	//检查目标地图不在服务器组支持中
	if( ret == 0)
	{
		CSSOption* opt =CSSMODULE->get_owneropt();
		opt2 =opt->svrteamopt_->get_cssoptbymapid( t2->ownermapid_);
		if( opt2 == 0)
			ret =5;
	}

	//检查是否和进入副本冲突
	Player* user =insvr_->get_player( req->uuid_);
	if( user == 0 || user->is_entertranscript_)
		ret =6;

	//发送返回
	PRO::Pro_AppTeleport_ack* ack =PROTOCOL_NEW PRO::Pro_AppTeleport_ack();
	PLAYERC_UUID_FILL( ack, req->get_uuidglobalindex(), req->get_uuiduserid(), req->get_uuidinitstmp());
	ack->result_ =ret;

	if( ret == 0)
	{
		ack->cssindex_ =opt2->server_index_;
		ack->mapid_ =t2->ownermapid_;

		ack->locationx_ =t2->pos_.x_;
		ack->locationy_ =t2->pos_.y_;
		ack->locationz_ =t2->pos_.z_;
		ack->facing_ =t2->face_;
	}

	if( ret == 0)
	{
		CSSMODULE->send_to_cts( ack);

		//通知清除资源
		NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::gts_teleport_req, this->insvr_, _1, _2);
		NetCommand* pcmd =TASKCMD_NEW NetCommand( req, fun, true);
		autorelease =false;

		insvr_->regist_netcmd( pcmd);

		//退出当前地图
		this->reset();
	}
	else
		this->send_to_gts( ack);
}

void AdapterPlayer::player_teleport( PRO::Pro_AppTeleport_ack* ack, bool& autorelease)
{
	CSSOption* opt =CSSMODULE->get_owneropt();
	ACE_ASSERT( opt->test_mapincss( ack->mapid_));

	BaseStoryService* svr =CSSMODULE->get_storyservicebymap( ack->mapid_);
	ACE_ASSERT( svr != 0);

	//保存值
	this->insvr_ =svr;
	gts_link_ =CSSMODULE->get_gtslinkbyuserglobal( global_index_);
	this->uuid_.set_uuid( ack->get_uuiduserid(), ack->get_uuidinitstmp());

	//注册到执行队列
	NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_teleport_ack, this->insvr_, _1, _2);
	NetCommand* pcmd =TASKCMD_NEW NetCommand( ack, fun, true);
	insvr_->regist_netcmd( pcmd);
	autorelease =false;
}
