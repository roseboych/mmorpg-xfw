/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGSSVR__H__
#define __LGSSVR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>

#include "LGS_Service.h"
#include "RGS_Service.h"
#include "RGSLink.h"
#include "Player.h"
#include "SvrTeamInfo.h"

/**
* @class LGSSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class LGSSvr
{
	typedef NS_STL::list<Player*>	EMPTY_PLAYERS_LIST;
	typedef NS_STL::vector<Player*>	USED_PLAYERS_VECTOR;
public:
	LGSSvr(void);
	virtual ~LGSSvr(void);

public:
	/**
	* 完成服务器链接注册后的回调
	* @param s
	* @param autorelease
	**/
	void fin_regist_service( BasicService* s, bool& autorelease);

	/**
	* 服务断线回调
	* @param s
	**/
	void service_disconnected( BasicService* s);

	virtual void post_exit_cmd() =0;

	virtual ACE_Thread_Mutex& get_mutex() =0;

public:
	/**
	* 初始化服务内容信息
	**/
	void init_svr();
	void uninit_svr();

	inline RGSLink* get_rgslink();
	inline Player* get_player( int ind);

	/**
	* 获取可用的player
	* @return
	**/
	Player* get_emptyplayer();

	/**
	* 释放一个正在使用的玩家
	* @param p
	**/
	void release_player( Player* p);

	/**
	* 转发client协议到rgs
	* @param p
	* @param autorelease
	**/
	inline void transfer_client2rgs( BasicProtocol* p, bool& autorelease);
	inline void send_to_rgs( BasicProtocol* p);

	/**
	* 构造svrteam通知协议
	**/
	BasicProtocol* construct_svrteam_notify();

protected:
	//close socket but don't release
	void close_all_service();

protected:
	//可以连接的rgs
	RGSLink						rgssvr_;

	//可以使用的玩家数
	ACE_Auto_Array_Ptr<Player>	players_;
	int							max_users_;
	//可用的玩家列表
	EMPTY_PLAYERS_LIST			empty_players_;
	//正在使用的玩家列表
	USED_PLAYERS_VECTOR			used_players_;

	//svrteam信息
	SvrTeamInfo					svr_update_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_rgsdisconnect();

	/**
	* 玩家断线
	* @param ps
	**/
	void player_disconnected( LGS_Service* ps);

	//----------------------------协议处理实现--------------------------------//
	//controller
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	/**
	* 玩家登陆请求
	* @param p
	* @param autorelease
	**/
	void net_user_loginreq( BasicProtocol* p, bool& autorelease);
	void rgs_user_loginack( BasicProtocol* p, bool& autorelease);

	/**
	* 服务器组的信息更新。lgs->client
	* @param p
	* @param autorelease
	**/
	void rgs_teaminfo_ntf( BasicProtocol* p, bool& autorelease);
	void rgs_userlost_ntf( BasicProtocol* p, bool& autorelease);

	/**
	* 选组功能
	**/
	void net_team_selreq( BasicProtocol* p, bool& autorelease);
	void rgs_team_selack( BasicProtocol* p, bool& autorelease);
	void rgs_team_confirmack( BasicProtocol* p, bool& autorelease);
	void rgs_teamtimeout_ntf( BasicProtocol* p, bool& autorelease);
	//退服重连
	void net_quitteamreconn_req( BasicProtocol* p, bool& autorelease);
	void rgs_quitteamreconn_ack( BasicProtocol* p, bool& autorelease);
};

inline 
RGSLink* LGSSvr::get_rgslink()
{
	return &rgssvr_;
}

inline 
Player* LGSSvr::get_player( int ind)
{
	if( ind < 0 || ind >= max_users_)
		return 0;

	return &players_[ind];
}

inline 
void LGSSvr::transfer_client2rgs( BasicProtocol* p, bool& autorelease)
{
	rgssvr_.send_protocol( p);
	autorelease =false;
}

inline 
void LGSSvr::send_to_rgs( BasicProtocol* p)
{
	rgssvr_.send_protocol( p);
}

#endif //__RGSSVR__H__
