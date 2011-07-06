/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "BaseStoryService.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "CSSModule.h"
#include "character/MyItemInfo.h"
#include "character/MyPetInfo.h"

USE_PROTOCOL_NAMESPACE;

void BaseStoryService::cts_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts[maincontentsvr]. user:[%d]", p->uuid_.userid_);

	//向周围玩家广播退出协议
	Pro_Logout_ntf* ntf =PROTOCOL_NEW Pro_Logout_ntf();
	ntf->chrid_ =user->get_id();
	scene_manage_->broadcast_inview( user, ntf, false);

	//TODO:
	player_unregist( p->uuid_);
}

void BaseStoryService::gts_lnkkeepload_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	user->lnkkeep_reload();
}

void BaseStoryService::dpx_chrload_ack( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_ChrLoad_ack* ack =dynamic_cast<Pro_ChrLoad_ack*>(p);
	//属性加载
	user->props_load( ack);

	user->send_to_gts( p);
	autorelease =false;
}

void BaseStoryService::dpx_chrfin_ntf( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_ChrFin_NTF* fin =dynamic_cast<Pro_ChrFin_NTF*>(p);
	//转化为地图坐标
	scene_manage_->world_to_local( fin->baseinfo_.posx_, fin->baseinfo_.posy_);
	//完成属性加载
	user->regist_fin( fin);

	//初始化玩家进入地图
	scene_manage_->unitobj_entermap( user);

	//计算玩家属性
	user->calcuate_playerproperty();

	user->send_to_gts( p);
	autorelease =false;
}

void BaseStoryService::cts_chrreg2world_req( BasicProtocol* p, bool& autorelease)
{
	//注销玩家
	Player* pold =get_player( p->uuid_);
	ACE_ASSERT( pold != 0);
	this->player_unregist( pold);

	//注册玩家
	Pro_ChrRegistToWorld_req* req =dynamic_cast<Pro_ChrRegistToWorld_req*>(p);
	Player* user =this->player_regist( p->uuid_, req->chrid_);

	//转发请求给gts
	user->send_to_gts( req);
	autorelease =false;

	//发送获取玩家信息请求
	Pro_ChrLoad_req* lntf =PROTOCOL_NEW Pro_ChrLoad_req();
	PRO_UUID_FILL( lntf, user->global_index_, user->uuid_);
	lntf->chrid_ =user->chrid_;
	lntf->cssindex_ =CSSMODULE->get_owneropt()->server_index_;

	CSSMODULE->send_to_dpx( lntf);
}

void BaseStoryService::gts_teleport_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	//发送退出协议
	Pro_Logout_ntf* ntf =PROTOCOL_NEW Pro_Logout_ntf();
	ntf->chrid_ =user->get_id();
	scene_manage_->broadcast_inview( user, ntf, false);

	scene_manage_->unitobj_leavemap( user);

	//玩家注销
	this->player_unregist( p->uuid_);
}

void BaseStoryService::cts_teleport_ack( BasicProtocol* p, bool& autorelease)
{
	//注销玩家
	Player* pold =get_player( p->uuid_);
	ACE_ASSERT( pold != 0);
	this->player_unregist( pold);

	//注册玩家
	Pro_AppTeleport_ack* ack =dynamic_cast<Pro_AppTeleport_ack*>(p);
	Player* user =this->player_regist( p->uuid_, ack->chrid_);

	//保存最新位置
	Pro_DBPosRotSave_ntf* ntf =PROTOCOL_NEW Pro_DBPosRotSave_ntf();
	PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);
	ntf->lastposx_ =ack->locationx_;
	ntf->lastposy_ =ack->locationy_;
	ntf->lastposz_ =ack->locationz_;
	ntf->lastfacing_ =ack->facing_;

	CSSMODULE->send_to_dpx( ntf);

	//发送获取玩家信息请求
	Pro_ChrLoad_req* lntf =PROTOCOL_NEW Pro_ChrLoad_req();
	PRO_UUID_FILL( lntf, user->global_index_, user->uuid_);
	lntf->chrid_ =user->chrid_;
	lntf->cssindex_ =ack->cssindex_;

	CSSMODULE->send_to_dpx( lntf);
}

void BaseStoryService::gts_playermoveto_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_PlayerMove_req* req =dynamic_cast<Pro_PlayerMove_req*>(p);

	coordinate_pointer pos;
	pos.x_ =req->locationx_;
	pos.y_ =req->locationy_;
	pos.z_ =req->locationz_;

	scene_manage_->unitobj_moveto( user, pos, req->facing_);
}

void BaseStoryService::gts_equipitem_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);


}

void BaseStoryService::gts_petsummon_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_PetSummon_req* req =dynamic_cast<Pro_PetSummon_req*>(p);

	Pro_PetSummon_ack* ack =PROTOCOL_NEW Pro_PetSummon_ack();
	PRO_UUID_FILL( ack, user->global_index_, user->uuid_);
	ack->petid_ =req->petid_;

	MyPetInfo* pet =0;
	if( req->petid_ == NO_INITVALUE)
	{
		pet =user->pet_back();
		if( pet == 0)
			ack->result_ =2;
		else
			ack->result_ =0;
	}
	else
	{
		pet =user->pet_followme( req->petid_);
		if( pet == 0)
			ack->result_ =1;
		else
			ack->result_ =0;
	}

	user->send_to_gts( ack);

	//可视区域广播
	if( pet)
	{
		Pro_PetSummon_ntf* ntf =PROTOCOL_NEW Pro_PetSummon_ntf();
		ntf->chrid_ =user->chrid_;
		ntf->petcode_ =pet->get_petcode();
		ntf->petname_ =pet->get_petname();

		scene_manage_->broadcast_inview( user, ntf, false);
	}
}

void BaseStoryService::gts_teamchat_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	/*
	Pro_AppMapChat_req* req =dynamic_cast<Pro_AppMapChat_req*>(p);

	Pro_AppMapChat_ntf* ntf =PROTOCOL_NEW Pro_AppMapChat_ntf();
	ntf->chrid_ =user->chrid_;
	ntf->nick_ =user->baseinfo_.name_;
	ntf->msg_ =req->msg_;

	scene_manage_->broadcast_inview( user, ntf, true);
	*/
}

void BaseStoryService::gts_p2pchat_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

}

void BaseStoryService::gts_skillstudy_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_SkillStudy_req* req =dynamic_cast<Pro_SkillStudy_req*>( p);
	user->skill_study( req->skillid_);
}

void BaseStoryService::gts_skillused_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_SkillUsed_req* req =dynamic_cast<Pro_SkillUsed_req*>( p);
	user->skill_used( req->skillid_, req->target_unit_, (CHRSTATE_TYPE_ENUM)req->curstate_);
}
