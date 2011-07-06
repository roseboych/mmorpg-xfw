/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ADAPTERPLAYER__H__
#define __ADAPTERPLAYER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"
#include "GTSLink.h"

class BaseStoryService;

namespace PRO{ 
	class Pro_AppTeleport_req;
	class Pro_AppTeleport_ack;
	class Pro_ChrRegistToWorld_req;
}

/**
* @class AdapterPlayer
* 
* @brief 玩家信息派发器
**/
class AdapterPlayer
{
public:
	AdapterPlayer();

	void send_to_gts( BasicProtocol* p);

	void reset();

	/**
	* 是否是一个空闲的玩家
	**/
	bool is_freeplayer();

	void player_regist2world( PRO::Pro_ChrRegistToWorld_req* req, bool& autorelease);
	void player_teleport( PRO::Pro_AppTeleport_ack* ack, bool& autorelease);
	void player_teleportout( PRO::Pro_AppTeleport_req* req, bool& autorelease);

public:
	//玩家标识号
	uuid_session uuid_;
	//全局索引,系统初始化时分配
	S_INT_32	global_index_;

	//玩家所在的svr
	BaseStoryService*	insvr_;
	GTSLink	*gts_link_;
};

#endif	//__ADAPTERPLAYER__H__
