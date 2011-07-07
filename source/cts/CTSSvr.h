/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTSSVR__H__
#define __CTSSVR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>

#include "CTS_Service.h"
#include "RGS_Service.h"
#include "RGSLink.h"
#include "GTSLink.h"
#include "DPXLink.h"
#include "CSSLink.h"
#include "player/Player.h"
#include "TeamProxySession.h"
#include "InstCellMgr.h"

class SvrTeamOption;

/**
* @class CTSSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class CTSSvr
{
	//userid来索引的列表
	typedef NS_STL::map< S_INT_32, Player*>	PLAYERALL_MAP;
	typedef NS_STL::list<Player*> PLAYER_LIST;
	typedef NS_STL::map< S_INT_32, TeamProxySession*> TEAMPROXY_MAP;
	typedef NS_STL::list<TeamProxySession*>	EMPTY_TEAMPROXY_LIST;
public:
	CTSSvr(void);
	virtual ~CTSSvr(void);

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
	inline GTSLink* get_gtslink( int ind);
	inline GTSLink* get_gtslinkbysindex( int user_index_);
	inline Player* get_player( int ind);
	inline Player* get_playerbyid( S_INT_32 userid);

	/**
	* 发送到rgs
	* @param p	使用人分配，调用函数负责释放
	**/
	void send_to_rgs( BasicProtocol* p);
	void send_to_dpx( BasicProtocol* p);
	void broadcast_to_gts( BasicProtocol* p);
	void broadcast_to_css( BasicProtocol* p);

	/**
	* 获取个空闲的玩家代理
	**/
	TeamProxySession* get_playerproxy( S_INT_32 userid, S_TIMESTAMP st, S_INT_32 sindex);
	TeamProxySession* is_playerproxy( S_INT_32 userid);
	inline TeamProxySession* get_playerproxy( S_INT_32 ind);
	void free_waitproxy( S_INT_32 userid);
	void free_player( Player* user);

protected:
	//close socket but don't release
	void close_all_service();

protected:
	//可以连接的rgs
	RGSLink	rgssvr_;

	//可以连接的dpx
	DPXLink	dpxsvr_;

	//可以被连接的css
	ACE_Auto_Array_Ptr<CSSLink>	csssvr_;
	S_INT_32					csssvr_nums_;

	//可以被连接的gts
	ACE_Auto_Array_Ptr<GTSLink>	gtssvr_;
	S_INT_32					gtssvr_nums_;

	//支持的玩家总数
	ACE_Auto_Array_Ptr<Player>	players_;
	S_INT_32					player_nums_;
	PLAYERALL_MAP				login_players_;

	//副本状态维护
	InstCellMgr					instcell_mgr_;

	//选服代理
	ACE_Auto_Array_Ptr<TeamProxySession>	team_proxys_;
	EMPTY_TEAMPROXY_LIST					empty_proxys_;
	TEAMPROXY_MAP							wait_proxys_;

	//本服务器的server_index_
	S_INT_32		cts_serverindex_;
	S_INT_32		cts_teamid_;

	SvrTeamOption*	owner_svrteam_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_rgsdisconnect();
	void svr_dpxdisconnect();
	void svr_gtsdisconnect( GTSLink* l);
	void svr_cssdisconnect( CSSLink* l);

	//----------------------------协议处理实现--------------------------------//
	//controller
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	void rgs_userlost_ntf( BasicProtocol* p, bool& autorelease);
	void gts_userlost_ntf( BasicProtocol* p, bool& autorelease);
	//断线保持
	void gts_lnkkeep_ntf( BasicProtocol* p, bool& autorelease);
	//副本服务器状态更新
	void css_instupdate_ntf( BasicProtocol* p, bool& autorelease);

	//选服处理
	void rgs_team_selreq( BasicProtocol* p, bool& autorelease);
	void gts_team_selproxyack( BasicProtocol* p, bool& autorelease);
	void gts_team_confirmack( BasicProtocol* p, bool& autorelease);
	//退服处理
	void gts_quitteam_req( BasicProtocol* p, bool& autorelease);
	void rgs_quitteam_ack( BasicProtocol* p, bool& autorelease);
	//角色处理
	void gts_chrlist_req( BasicProtocol* p, bool& autorelease);
	void dpx_chrlist_ack( BasicProtocol* p, bool& autorelease);
	void gts_chradd_req( BasicProtocol* p, bool& autorelease);
	void dpx_chradd_ack( BasicProtocol* p, bool& autorelease);
	void gts_chrsel_req( BasicProtocol* p, bool& autorelease);
	void dpx_chrsel_ack( BasicProtocol* p, bool& autorelease);
	void gts_chrdel_req( BasicProtocol* p, bool& autorelease);
	void dpx_chrdel_ack( BasicProtocol* p, bool& autorelease);
	//地图转跳
	void css_teleport_ack( BasicProtocol* p, bool& autorelease);
	//进入副本
	void css_enterinst_req( BasicProtocol* p, bool& autorelease);
	void css_enterinst_ack( BasicProtocol* p, bool& autorelease);
	void css_enterinstconfirm_ntf( BasicProtocol* p, bool& autorelease);
	//退出副本
	void css_quitinst_req( BasicProtocol* p, bool& autorelease);
};

inline 
CSSLink* CTSSvr::get_csslink( int ind)
{
	if( ind < 0 || ind >= csssvr_nums_)
		return 0;
	return &(csssvr_[ind]);
}

inline 
GTSLink* CTSSvr::get_gtslink( int ind)
{
	if( ind < 0 || ind >= gtssvr_nums_)
		return 0;
	return &(gtssvr_[ind]);
}

inline Player* CTSSvr::get_player( int ind)
{
	if( ind < 0 || ind >= player_nums_)
		return 0;
	return &(players_[ind]);
}

inline 
Player* CTSSvr::get_playerbyid( S_INT_32 userid)
{
	PLAYERALL_MAP::iterator fiter =login_players_.find( userid);
	if( fiter == login_players_.end())
		return 0;

	return fiter->second;
}

inline 
TeamProxySession* CTSSvr::get_playerproxy( S_INT_32 ind)
{
	if( ind < 0 || ind >= player_nums_)
		return 0;
	return &(team_proxys_[ind]);
}

inline
GTSLink* CTSSvr::get_gtslinkbysindex( int user_index_)
{
	for( int ii =0; ii < gtssvr_nums_; ++ii)
	{
		if( gtssvr_[ii].is_useringts( user_index_))
			return &(gtssvr_[ii]);
	}

	return 0;
}

#endif //__CTSSVR__H__
