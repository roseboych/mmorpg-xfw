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
#include "storyserviceimpl/InstanceStoryServiceImpl.h"
#include "CSSModule.h"
#include "character/Player.h"

AdapterPlayer::AdapterPlayer():
insvr_( 0),
gts_link_( 0),
inst_svr_cache_( 0)
{
	inst_uuid_cache_.reset();
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

void AdapterPlayer::reset_instcache()
{
	inst_uuid_cache_.reset();
	inst_svr_cache_ =0;

	if( insvr_ != 0 && insvr_->is_instancestoryservice())
		reset();
}

void AdapterPlayer::player_userlost( PRO::Pro_SvrUserLost_NTF* ntf, bool& autorelease)
{
	bool bclear =false;
	if( this->insvr_)
	{
		//玩家掉线特别处理
		if( ntf->same_session( this->uuid_))
		{
			MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[adapter]. user:[%d] mapid:[%d]", ntf->uuid_.userid_, insvr_->get_storyoption()->get_mapid());

			NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_userlost_ntf, this->insvr_, _1, _2);
			NetCommand* pcmd =TASKCMD_NEW NetCommand( ntf->clone(), fun, true);
			this->insvr_->regist_netcmd( pcmd);

			bclear =true;
		}
	}

	if( this->inst_svr_cache_)
	{
		if( ntf->same_session( this->inst_uuid_cache_))
		{
			MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[adapter]. user:[%d] mapid:[%d]", ntf->uuid_.userid_, inst_svr_cache_->get_storyoption()->get_mapid());

			NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_userlost_ntf, this->inst_svr_cache_, _1, _2);
			NetCommand* pcmd =TASKCMD_NEW NetCommand( ntf, fun, true);
			this->inst_svr_cache_->regist_netcmd( pcmd);
			autorelease =false;

			bclear =true;
		}
	}

	//清除玩家信息
	if( bclear)
	{
		this->reset();
		this->reset_instcache();
	}
}

void AdapterPlayer::player_regist2world( PRO::Pro_ChrRegistToWorld_req* req, bool& autorelease)
{
	CSSOption* opt =CSSMODULE->get_owneropt();
	ACE_ASSERT( opt->test_mapincss( req->mapid_));

	BaseStoryService* svr =CSSMODULE->get_storyservicebymap( req->mapid_);
	ACE_ASSERT( svr != 0);

	reset_instcache();

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
	if( !req->same_session( this->uuid_))
		return;

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
	PRO_UUID_FILL2( ack, req);
	ack->result_ =ret;

	if( ret == 0)
	{
		ack->cssindex_ =opt2->server_index_;
		ack->mapid_ =t2->ownermapid_;

		//目标地图的地图坐标
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

void AdapterPlayer::player_instcellproxy( PRO::Pro_AppEnterIns_req* req, bool& autorelease)
{
	ToTranscriptConfig& tconf =WORLDINFO->get_totranscriptconfig();
	transcript_teleport_info* pteleport =tconf.get_teleportinfobyid( req->telid_);

	S_INT_8 ret =0;
	InstanceStoryServiceImpl* psvr =CSSMODULE->get_inststorysvrbycellid( req->cellid_);
	if( psvr == 0)
		ret =5;	//没有可用的副本
	else if( psvr->get_serverstate() != TRANSCRIPT_SVRST_CANENTER)
		ret =7;	//不允许进入副本

	if( ret != 0)
	{
		PRO::Pro_AppEnterIns_ack* ack =PROTOCOL_NEW PRO::Pro_AppEnterIns_ack();
		PRO_UUID_FILL2( ack, req);
		ack->result_ =ret;
		CSSMODULE->send_to_cts( ack);
		return;
	}

	//缓存注册信息
	inst_uuid_cache_.set_uuid( req->get_uuiduserid(), req->get_uuidinitstmp());
	inst_svr_cache_ =psvr;

	//注册到执行队列
	NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_instenter_req, this->inst_svr_cache_, _1, _2);
	NetCommand* pcmd =TASKCMD_NEW NetCommand( req, fun, true);
	inst_svr_cache_->regist_netcmd( pcmd);
	autorelease =false;
}

void AdapterPlayer::player_enterinstack( PRO::Pro_AppEnterIns_ack* ack, bool& autorelease)
{
	if( !ack->same_session( this->uuid_) || insvr_ == 0)
		return;

	bool bsucc =(ack->result_ == 0);

	NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_instenter_ack, this->insvr_, _1, _2);
	NetCommand* pcmd =TASKCMD_NEW NetCommand( ack, fun, true);
	insvr_->regist_netcmd( pcmd);
	autorelease =false;

	//如果注册成功，对于主世界来说需要注销玩家
	if( bsucc)
		this->reset();
}

void AdapterPlayer::player_enterinstconfirm( PRO::Pro_AppEnterInsConfirm_ntf* ntf, bool& autorelease)
{
	if( inst_svr_cache_ == 0 || !ntf->same_session( this->inst_uuid_cache_))
		return;

	insvr_ =inst_svr_cache_;
	uuid_ =inst_uuid_cache_;
	this->gts_link_ =CSSMODULE->get_gtslinkbyuserglobal( global_index_);

	inst_svr_cache_ =0;
	inst_uuid_cache_.reset();

	NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::gts_instenterconfirm_ntf, this->insvr_, _1, _2);
	NetCommand* pcmd =TASKCMD_NEW NetCommand( ntf, fun, true);
	insvr_->regist_netcmd( pcmd);
	autorelease =false;
}

void AdapterPlayer::player_enterinstovertime( PRO::Pro_AppEnterInsOvertime_ntf* ntf, bool& autorelease)
{
	//发送给副本服务器的
	if( ntf->cellid_ != NO_INITVALUE)
	{
		BaseStoryService* svr =0;
		if( has_instregistcache())
		{
			if( !ntf->same_session( inst_uuid_cache_))
				return;

			svr =this->inst_svr_cache_;
		}
		else if( insvr_ != 0 && insvr_->is_instancestoryservice())
		{
			if( !ntf->same_session( this->uuid_))
				return;

			svr =insvr_;
		}

		if( svr != 0)
		{
			NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_enterinstovertime_ntf, svr, _1, _2);
			NetCommand* pcmd =TASKCMD_NEW NetCommand( ntf, fun, true);
			autorelease =false;

			svr->regist_netcmd( pcmd);
		}
	}
	else if( insvr_ != 0)
	{
		if( !ntf->same_session( this->uuid_))
			return;

		//发给主世界地图服务器的
		NETCMD_FUN_MAP fun =boost::bind( &BaseStoryService::cts_enterinstovertime_ntf, this->insvr_, _1, _2);
		NetCommand* pcmd =TASKCMD_NEW NetCommand( ntf, fun, true);
		autorelease =false;

		insvr_->regist_netcmd( pcmd);
	}
}
