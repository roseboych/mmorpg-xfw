/**
* lgs application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYER__H__
#define __PLAYER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <prolib/Pro_all.h>
#include <prolib/BasicProtocol.h>
#include "LGS_Service.h"

/**
* @class Player
*
* @brief 
*
**/
class Player
{
public:
	Player();

	/**
	* 向客户端发送协议
	* @param
	**/
	void send_protocol( BasicProtocol* p){ 
		if( service_.is_connect_)
			service_.append_write_msg( p);
		else if( p)
			delete p;
	}

#ifdef CORELIB_WIN32
	void proactor( ACE_Proactor *proactor){
		service_.proactor( proactor);
	}
#else
	void reactor( ACE_Reactor *reactor){
		service_.reactor( reactor);
	}
#endif

	LGS_Service* get_service(){ return &service_;}

	void close_service_nonotify(){
		service_.close_service();
	}

public:
	/**
	* 重置玩家
	**/
	void reset();

	/**
	* 为使用准备环境
	**/
	void prepare();

	//登陆验证过程
	void login_begin();
	void login_end( PRO::Pro_Login_ack* ack);

	//选服过程
	void selteam_begin();
	void selteam_end( bool succ);
	void selteam_confirm();
	void selteam_timeout();

	//退服重连
	void quitteam_reconn_begin();
	void quitteam_reconn( int uid);

	/**
	* 玩家定时心跳处理
	* @param t
	**/
	void main_tick( ACE_UINT64& t);

	/**
	* 是否能够处理登陆请求
	**/
	bool can_dologin();

	/**
	* 是否能退服重连
	**/
	bool can_doteamreconn();

	/**
	* 是否同一个session中
	* @param rid
	* @param st
	**/
	inline bool is_samesession( int rid, S_TIMESTAMP st);
	inline bool is_samesession( uuid_session& u);

	/**
	* 发送服务器组信息
	**/
	void notify_svrteaminfo();

	bool is_logon(){
		return status_ >= PLAYERSTATUS_LOGINS;
	}

	bool is_lnkkeep(){
		return status_ == PLAYERSTATUS_LNKKEEP;
	}

public:
	//玩家状态
	PLAYER_STATUS_ENUM	status_;
	//登陆失败次数
	char			login_fail_nums_;
	//玩家userid
	uuid_session	uuid_;
	//时间戳
	ACE_UINT64		tstamp_;
	//处理svrteam和gtslist的定时通知使用
	ACE_UINT64		tstamp2_;
	//是否已经强制关闭
	bool			have_force_close_;

	//关联的socket服务
	LGS_Service	service_;
	//系统索引
	int			server_index_;
};

inline 
bool Player::is_samesession( int rid, S_TIMESTAMP st)
{
	return uuid_.is_samesession( rid, st);
}

inline 
bool Player::is_samesession( uuid_session& u)
{
	return uuid_.is_samesession( u);
}

#endif	//__PLAYER__H__
