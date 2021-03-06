/**
* css application
*
* @category		Main Story Service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "MainStoryServiceImpl.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/world/ToTranscriptConfig.h>

#include "../CSSModule.h"
#include "EnterInstCondHelper.h"

USE_PROTOCOL_NAMESPACE;

void MainStoryServiceImpl::gts_instenter_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_AppEnterIns_req* req =dynamic_cast<Pro_AppEnterIns_req*>( p);

	transcript_teleport_info* pteleport =0;
	S_INT_8 ret =0;

	//已经存在一个请求
	if( user->is_entertranscript_)
		ret =3;
	else
	{
		ToTranscriptConfig& tconf =WORLDINFO->get_totranscriptconfig();
		pteleport =tconf.get_teleportinfobyid( req->telid_);
		//转跳点无效
		if( pteleport == 0)
			ret =1;
	}

	if( ret == 0)
	{
		//转跳点必须在当前地图内
		if( get_storyoption()->get_mapid() != pteleport->mainmap_id_)
			ret =4;
	}

	if( ret == 0)
	{
		//不满足进入副本条件
		StoryMapOption* instopt =WORLDINFO->get_instancemapres( pteleport->instmap_id_);
		ACE_ASSERT( instopt != 0);
		if( !ENTERINSTHelper->can_joininstance( this, instopt, user))
			ret =9;
	}

	if( ret != 0)
	{
		Pro_AppEnterIns_ack* ack =PROTOCOL_NEW Pro_AppEnterIns_ack();
		PRO_UUID_FILL2( ack, req);
		ack->result_ =ret;

		user->send_to_gts( ack);
		return;
	}

	//设置进入副本状态变量
	user->begin_entertranscript();

	//进入cts进行注册,需要记录最后的玩家位置
	req->posx_ =user->get_positionx();
	req->posy_ =user->get_positiony();
	scene_manage_->local_to_world( req->posx_, req->posy_);

	req->posz_ =user->get_positionz();

	CSSMODULE->send_to_cts( p);
	autorelease =false;
}

void MainStoryServiceImpl::cts_instenter_req( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}

void MainStoryServiceImpl::cts_instenter_ack( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_AppEnterIns_ack* ack =dynamic_cast<Pro_AppEnterIns_ack*>( p);
	bool bsucc =( ack->result_ == 0);

	user->end_entertranscript();

	if( bsucc)
	{
		//如果成功
		//保存最新位置
		Pro_DBPosRotSave_ntf* ntf =PROTOCOL_NEW Pro_DBPosRotSave_ntf();
		PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);
		
		ntf->lastposx_ =user->get_positionx();
		ntf->lastposy_ =user->get_positiony();
		scene_manage_->local_to_world( ntf->lastposx_, ntf->lastposy_);

		ntf->lastposz_ =user->get_positionz();
		ntf->lastfacing_ =user->get_facing();

		CSSMODULE->send_to_dpx( ntf);

		//发送退出协议
		Pro_Logout_ntf* ntf2 =PROTOCOL_NEW Pro_Logout_ntf();
		ntf2->chrid_ =user->get_id();
		scene_manage_->broadcast_inview( user, ntf2, false);

		scene_manage_->unitobj_leavemap( user);
	}

	user->send_to_gts( ack);
	autorelease =false;

	//玩家注销
	if( bsucc)
	{
		this->player_unregist( p->uuid_);
	}
}

void MainStoryServiceImpl::cts_enterinstovertime_ntf( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	user->end_entertranscript();
}

void MainStoryServiceImpl::gts_instenterconfirm_ntf( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}

void MainStoryServiceImpl::gts_instquit_req( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}
