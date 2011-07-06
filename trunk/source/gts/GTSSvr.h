/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSSVR__H__
#define __GTSSVR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "gts_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>
#include <ace/Malloc_T.h>

#include "GTS_Service.h"
#include "CTS_Service.h"
#include "CSS_Service.h"
#include "CSSLink.h"
#include "CTSLink.h"
#include "player/Player.h"
#include "TeamProxySession.h"

#define GTS_MAX_CLIENT_NUM	6000

/**
* @class GTSSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class GTSSvr
{
	typedef NS_STL::list<Player*>	EMPTY_PLAYERS_LIST;
	typedef NS_STL::vector<Player*>	USED_PLAYERS_VECTOR;

public:
	GTSSvr(void);
	virtual ~GTSSvr(void);

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

	inline CSSLink* get_csslink( int ind);
	inline CTSLink* get_ctslink();
	inline Player* get_player( int ind);

	/**
	* 获取可用的player
	* @return
	**/
	Player* get_emptyplayer();

	/**
	* 玩家选服占位
	**/
	inline TeamProxySession* get_playerproxybyind( S_INT_32 proxyindex);
	TeamProxySession* team_proxy( S_INT_32 proxyind, S_INT_32 userid, S_TIMESTAMP st);

	/**
	* 释放一个正在使用的玩家
	* @param p
	**/
	void release_player( Player* p);

	/**
	* 断线玩家的连接保持处理
	* @param p
	**/
	void player_linkkeep( Player* p);

	inline void send_to_cts( BasicProtocol* p);

protected:
	//close socket but don't release
	void close_all_service();

protected:
	//可以连接的linesvr
	ACE_Auto_Array_Ptr<CSSLink> csssvr_;
	S_INT_32	csssvr_nums_;

	//可以连接的cts
	CTSLink	ctssvr_;

	//支持的玩家总数
	ACE_Auto_Array_Ptr<Player>	players_;
	static ACE_Cached_Allocator<Player, ACE_Null_Mutex>	cache_players_;
	S_INT_32	start_player_;
	S_INT_32	player_nums_;
	//正在使用的玩家列表
	USED_PLAYERS_VECTOR	used_players_;
	//选服占位
	ACE_Auto_Array_Ptr<TeamProxySession> team_proxys_;

	//本gts的server_index_
	S_INT_32	gts_index_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_cssdisconnect( CSSLink* l);
	void svr_ctsdisconnect( CTSLink* l);

	/**
	* 玩家断线
	* @param ps
	**/
	void player_disconnected( GTS_Service* ps);

	//----------------------------协议处理实现--------------------------------//
	//转发协议到client
	void transfer_to_net( BasicProtocol* p, bool& autorelease);
	void transfer_to_css( BasicProtocol* p, bool& autorelease);
	void transfer_to_css_ingame( BasicProtocol* p, bool& autorelease);

	//controller
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	void cts_userlost_ntf( BasicProtocol* p, bool& autorelease);

	//玩家退出请求
	void net_userlogout_req( BasicProtocol* p, bool& autorelease);
	//选服处理
	void cts_teamproxy_req( BasicProtocol* p, bool& autorelease);
	void net_proxyconfirm_req( BasicProtocol* p, bool& autorelease, void*);
	void cts_teamtimeout_ntf( BasicProtocol* p, bool& autorelease);
	//退服处理
	void net_quitteam_req( BasicProtocol* p, bool& autorelease);
	void cts_quitteam_ack( BasicProtocol* p, bool& autorelease);
	//角色功能
	void net_chrlist_req( BasicProtocol* p, bool& autorelease);
	void cts_chrlist_ack( BasicProtocol* p, bool& autorelease);
	void net_chradd_req( BasicProtocol* p, bool& autorelease);
	void cts_chradd_ack( BasicProtocol* p, bool& autorelease);
	void net_chrdel_req( BasicProtocol* p, bool& autorelease);
	void cts_chrdel_ack( BasicProtocol* p, bool& autorelease);
	//选角色进入游戏
	void net_chrsel_req( BasicProtocol* p, bool& autorelease);
	void cts_chrsel_ack( BasicProtocol* p, bool& autorelease);
	void css_chrreg2world_req( BasicProtocol* p, bool& autorelease);
	void css_chrload_ack( BasicProtocol* p, bool& autorelease);
	void css_chrfin_ntf( BasicProtocol* p, bool& autorelease);
	//地图切换
	void net_teleport_req( BasicProtocol* p, bool& autorelease);
	void css_teleport_ack( BasicProtocol* p, bool& autorelease);
	//副本切换
	void net_instenter_req( BasicProtocol* p, bool& autorelease);
	void css_instenter_ack( BasicProtocol* p, bool& autorelease);
	//移动请求
	//宠物召唤
	//换装
	//聊天
	//技能
};

inline 
CSSLink* GTSSvr::get_csslink( int ind)
{
	if( ind < 0 || ind >= csssvr_nums_)
		return 0;
	return &(csssvr_[ind]);
}

inline 
CTSLink* GTSSvr::get_ctslink()
{
	return &ctssvr_;
}

inline 
Player* GTSSvr::get_player( int ind)
{
	int lind =ind - start_player_;
	if( lind < 0 || lind >= player_nums_)
		return 0;

	return team_proxys_[lind].player_;
}

inline
TeamProxySession* GTSSvr::get_playerproxybyind( S_INT_32 proxyindex)
{
	int lind =proxyindex - start_player_;
	if( lind < 0 || lind >= player_nums_)
		return 0;

	return &(team_proxys_[lind]);
}

inline 
void GTSSvr::send_to_cts( BasicProtocol* p)
{
	ctssvr_.send_protocol( p);
}

#endif //__GTSSVR__H__
