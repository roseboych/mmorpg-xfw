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
	class Pro_SvrUserLost_NTF;
	class Pro_AppTeleport_req;
	class Pro_AppTeleport_ack;
	class Pro_ChrRegistToWorld_req;
	class Pro_AppEnterIns_req;
	class Pro_AppEnterIns_ack;
	class Pro_AppEnterInsOvertime_ntf;
	class Pro_AppEnterInsConfirm_ntf;
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
	* 玩家断线
	* @param ntf
	* @param autorelease
	**/
	void player_userlost( PRO::Pro_SvrUserLost_NTF* ntf, bool& autorelease);

	void player_regist2world( PRO::Pro_ChrRegistToWorld_req* req, bool& autorelease);
	void player_teleport( PRO::Pro_AppTeleport_ack* ack, bool& autorelease);
	void player_teleportout( PRO::Pro_AppTeleport_req* req, bool& autorelease);

	/**
	* 玩家进入副本占位请求
	* 只在副本css服务器处理
	* @param req
	* @param autorelease
	**/
	void player_instcellproxy( PRO::Pro_AppEnterIns_req* req, bool& autorelease);
	/*
	* 主世界地图css服务器处理进入副本返回
	* @param ack
	* @param autorelase
	*/
	void player_enterinstack( PRO::Pro_AppEnterIns_ack* ack, bool& autorelease);
	/*
	* 主世界和副本地图处理进入副本超时
	* @param ntf
	* @param autorelase
	*/
	void player_enterinstovertime( PRO::Pro_AppEnterInsOvertime_ntf* ntf, bool& autorelease);
	/*
	* 收到gts的进入副本确认通知
	* @param ntf
	* @param autorelease
	*/
	void player_enterinstconfirm( PRO::Pro_AppEnterInsConfirm_ntf* ntf, bool& autorelease);

public:
	void	reset_instcache();
	//是否处于副本注册第一阶段 cts->css(inst map)
	bool	has_instregistcache(){ return inst_svr_cache_ != 0;}

	//进入副本占位缓冲
	uuid_session		inst_uuid_cache_;
	BaseStoryService*	inst_svr_cache_;

public:
	//玩家标识号
	uuid_session		uuid_;
	//全局索引,系统初始化时分配
	int					global_index_;

	//玩家所在的svr
	BaseStoryService*	insvr_;
	GTSLink*			gts_link_;
};

#endif	//__ADAPTERPLAYER__H__
