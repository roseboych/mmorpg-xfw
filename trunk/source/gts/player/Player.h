/**
* gts application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "../gts_pre.h"

#include <prolib/BasicProtocol.h>
#include <prolib/Pro_all.h>
#include "../GTS_Service.h"

class TeamProxySession;
class CSSLink;

/**
* @class Player
* 
* @brief 玩家信息
**/
class Player
{
public:
	Player();
	~Player();

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

	GTS_Service* get_service(){ return &service_;}

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

	inline bool is_samesession( int userid, S_TIMESTAMP st);
	inline bool is_samesession( uuid_session& u);
	inline bool is_waitproxy();
	inline bool is_selteamsucc();
	inline bool is_freeplayer();
	inline bool is_selchr();
	inline bool is_inteam();
	inline bool is_ingame();
	inline bool is_quitteam();
	inline bool is_logout();

	/**
	* 代理确认
	* @param proxy
	**/
	void proxy_confirm( TeamProxySession* proxy);

	/**
	* 选择角色
	* @param cid
	**/
	void set_curchrid( int cid);

	/**
	* 玩家定时心跳处理
	* @param t
	**/
	void main_tick( ACE_UINT64& t);

	void send_to_css( BasicProtocol* p);

public:
	//当前使用的角色
	int			curchrid_;
	//关联到的css服务器
	CSSLink*	css_svr_;
	//是否在迁移css服务器
	bool		is_switchcss_;
	//是否是副本服务器
	bool		is_instsvr_;

	//是否强制关闭
	bool		have_force_close_;

	//选服用的时间戳
	ACE_UINT64	selteam_tmpstamp_;

public:
	//玩家状态
	PLAYERSTATUS_TYPE_ENUM	status_;
	//玩家session
	uuid_session		uuid_;

	//关联的socket服务
	GTS_Service			service_;
	//玩家全局索引，系统定义
	int					global_index_;

	TeamProxySession*	team_proxy_;
};

inline 
bool Player::is_samesession( int userid, S_TIMESTAMP st)
{
	return uuid_.is_samesession( userid, st);
}

inline 
bool Player::is_samesession( uuid_session& u)
{
	return uuid_.is_samesession( u);
}

inline 
bool Player::is_waitproxy()
{
	return status_ == PLAYERSTATUS_PROXYW;
}

inline 
bool Player::is_inteam()
{
	return status_ == PLAYERSTATUS_INTEAM;
}

inline 
bool Player::is_selteamsucc()
{
	return status_ >= PLAYERSTATUS_INTEAM;
}

inline 
bool Player::is_freeplayer()
{
	return status_ == PLAYERSTATUS_NONE;
}

inline 
bool Player::is_selchr()
{
	return curchrid_ != NO_INITVALUE;
}

inline 
bool Player::is_ingame()
{
	return status_ == PLAYERSTATUS_INGAME;
}

inline 
bool Player::is_quitteam()
{
	return status_ == PLAYERSTATUS_OUTTEAM_FIN;
}

inline 
bool Player::is_logout()
{
	return status_ == PLAYERSTATUS_LOGOUT;
}

#endif	//__PLAYER__H__
