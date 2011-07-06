/**
* dpx application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPXSVR__H__
#define __DPXSVR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "dpx_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>

#include "DPX_Service.h"
#include "CTS_Service.h"
#include "CSSLink.h"
#include "CTSLink.h"
#include "player/Player.h"

/**
* @class DPXSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class DPXSvr
{
public:
	DPXSvr(void);
	virtual ~DPXSvr(void);

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

	void send_to_cts( BasicProtocol* p);
	void send_to_css( BasicProtocol* p, int ind);

protected:
	//close socket but don't release
	void close_all_service();

protected:
	//可以连接的cts
	CTSLink	ctssvr_;

	//可以被连接的css
	ACE_Auto_Array_Ptr<CSSLink>	csssvr_;
	S_INT_32	csssvr_nums_;

	//dpx支持的最大玩家列表
	ACE_Auto_Array_Ptr<Player>	players_;
	S_INT_32	player_nums_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_cssdisconnect( CSSLink* l);
	void svr_ctsdisconnect( CTSLink* l);

	//----------------------------协议处理实现--------------------------------//
	//controller
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	//玩家注销
	void cts_userlost_ntf( BasicProtocol* p, bool& autorelease);
	//玩家注册
	void cts_userregist_ntf( BasicProtocol* p, bool& autorelease);
	//角色管理
	void cts_chrlist_req( BasicProtocol* p, bool& autorelease);
	void cts_chradd_req( BasicProtocol* p, bool& autorelease);
	void cts_chrsel_req( BasicProtocol* p, bool& autorelease);
	void cts_chrdel_req( BasicProtocol* p, bool& autorelease);
	//css内容加载
	void css_chrload_req( BasicProtocol* p, bool& autorelease);
	//玩家数据保存
	void css_posrotsave_ntf( BasicProtocol* p, bool& autorelease);
	void css_playerbaseinfo_sav( BasicProtocol* p, bool& autorelease);
	//buffer数据变更
	void css_buffchange_ntf( BasicProtocol* p, bool& autorelease);
};

inline 
CSSLink* DPXSvr::get_csslink( int ind)
{
	if( ind < 0 || ind >= csssvr_nums_)
		return 0;
	return &(csssvr_[ind]);
}

inline 
CTSLink* DPXSvr::get_ctslink()
{
	return &ctssvr_;
}

inline 
Player* DPXSvr::get_player( int ind)
{
	if( ind < 0 || ind >= player_nums_)
		return 0;

	return &(players_[ind]);
}

#endif //__DPXSVR__H__
