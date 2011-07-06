/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGSSVR__H__
#define __RGSSVR__H__

#include "rgs_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>

#include "PlayerInfo.h"
#include "LGSLink.h"
#include "CTSLink.h"
#include "RGS_Service.h"
#include "SvrTeamInfo.h"
#include "servercontroller/ControllerLink.h"

/**
* @class RGSSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class RGSSvr
{
	typedef NS_STL::map<S_INT_32, PlayerInfo*>	USEDUSER_MAP;
	typedef NS_STL::list< PlayerInfo*> EMPTYUSER_LIST;
public:
	RGSSvr(void);
	virtual ~RGSSvr(void);

	//---------------------------server管理----------------------------//
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

	inline LGSLink* get_lgslink( S_INT_32 ind);
	inline CTSLink* get_ctslink( S_INT_32 ind);

	inline ControllerLink* get_controllerlink();

	/**
	* 向所有的lgs广播
	* @param p
	**/
	void broadcast_to_lgs( BasicProtocol* p);
	void broadcast_to_cts( BasicProtocol* p);

	/**
	* 根据userid查找登陆的玩家
	* @param userid
	* @return PlayerInfo*
	**/
	PlayerInfo* find_byuserid( S_INT_32 userid);

	/**
	* 注册一个登陆玩家
	* @param p
	**/
	bool unregist_logonuser( PlayerInfo *p);
	bool regist_player( PlayerInfo* p);
	void free_player( PlayerInfo* p);

	/**
	* 玩家强制退出，并且发送通知
	* @param player
	**/
	void player_lnkkeeplogout( PlayerInfo* user);

	/**
	* 获取一个可用的玩家
	* @return
	**/
	PlayerInfo* get_emptyplayer();
	bool free_emptyplayer( PlayerInfo* p);
	void fore_playerlogout( PlayerInfo* p);

protected:
	//close socket but don't release
	void close_all_service();

protected:
	//存在的所有用户信息
	ACE_Auto_Array_Ptr<PlayerInfo>	players_;
	S_INT_32	player_nums_;
	
	//根据userid索引的用户信息
	USEDUSER_MAP	userid_players_;
	//可用的玩家列表
	EMPTYUSER_LIST	noused_players_;
	//占位玩家列表
	EMPTYUSER_LIST	proxy_players_;

	//所有的lgs信息
	ACE_Auto_Array_Ptr<LGSLink>	lgssvr_;
	S_INT_32	lgssvr_nums_;
	//所有的cts信息
	ACE_Auto_Array_Ptr<CTSLink>	ctssvr_;
	S_INT_32	ctssvr_nums_;

	//管理工具连接对象
	ControllerLink	controller_svr_;

	//svrteam信息
	SvrTeamInfo	svr_update_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_lgsdisconnect( LGSLink* l);
	void svr_ctsdisconnect( CTSLink* l);

	void svr_controller_disconnect();

	//----------------------------协议处理实现--------------------------------//
	//controller
	void controller_login_req( BasicProtocol* p, bool& autorelease);
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	//cts server
	/**
	* 服务器组的信息更新。cts->rgs
	* @param p
	* @param autorelease
	**/
	void svr_teaminfo_ntf( BasicProtocol* p, bool& autorelease);
	void cts_userlost_ntf( BasicProtocol* p, bool& autorelease);
	//断线保持
	void cts_lnkkeep_ntf( BasicProtocol* p, bool& autorelease);
	//sel team server ack
	void cts_selteamconfirm_ack( BasicProtocol* p, bool& autorelease);

	//lgs server
	void lgs_login_req( BasicProtocol* p, bool& autorelease, S_INT_32 server_index);
	void lgs_userlost_ntf( BasicProtocol* p, bool& autorelease, S_INT_32 server_index);
	//sel team server
	void lgs_selteam_req( BasicProtocol* p, bool& autorelease, S_INT_32 server_index);
	void cts_selteam_ack( BasicProtocol*p, bool& autorelease);
	void cts_teamtimeout_ntf( BasicProtocol* p, bool& autorelease);
	//退服请求
	void cts_quitteam_req( BasicProtocol*p, bool& autorelease);
	void lgs_quitteamreconn_req( BasicProtocol*p, bool& autorelease, S_INT_32 server_index);
};

inline 
LGSLink* RGSSvr::get_lgslink( S_INT_32 ind)
{
	if( ind < 0 || ind >= lgssvr_nums_)
		return 0;
	return &lgssvr_[ind];
}

inline 
CTSLink* RGSSvr::get_ctslink( S_INT_32 ind)
{
	if( ind < 0 || ind >= ctssvr_nums_)
		return 0;
	return &ctssvr_[ind];
}

inline
ControllerLink* RGSSvr::get_controllerlink()
{
	return &controller_svr_;
}

#endif //__RGSSVR__H__
