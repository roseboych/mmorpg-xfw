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
	req->posz_ =user->get_positionz();

	CSSMODULE->send_to_cts( p);
	autorelease =false;
}

void MainStoryServiceImpl::cts_instenter_ack( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_AppEnterIns_ack* ack =dynamic_cast<Pro_AppEnterIns_ack*>( p);
	if( ack->result_ != 0)
		user->end_entertranscript();
}
