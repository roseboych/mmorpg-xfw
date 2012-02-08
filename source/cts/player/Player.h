/**
* cts application
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

#include "../cts_pre.h"
#include "../GTSLink.h"
#include "../CSSLink.h"
#include "EnterInstCellTicker.h"

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

	/**
	* 重置玩家
	**/
	void reset();

	void tick( const ACE_UINT64& t);

	inline bool is_emptyplayer();

	/**
	* 选服确认
	**/
	void selteamconfirm( TeamProxySession* proxy, S_TIMESTAMP st);

	/**
	* 角色选择
	* @param chrid
	**/
	void chrsel( int chrid, float posx, float posy, float posz);

	inline bool is_lnkkeep();

	/**
	* 进入副本超时
	**/
	void	instcell_proxy_overtime();
	void	instcell_confirm_overtime();
	bool	is_enterinst(){
		return enter_instcell_ctrl_.is_enterinstcell();
	}
	void	instcell_quit();

public:
	/**
	* 是否同一个session
	**/
	inline bool is_samesession( int chrid, S_TIMESTAMP st);
	inline bool is_samesession( uuid_session& u);

	inline void send_to_gts( BasicProtocol* p);
	inline void send_to_css( BasicProtocol* p);

public:
	//当前角色
	int			chrid_;
	//最后位置
	float		lastposx_, lastposy_, lastposz_;
	//是否处于断线保持
	bool		is_lnkkeep_;

	//进入副本状态
	EnterInstCellTicker		enter_instcell_ctrl_;

	//所在的css信息
	CSSLink*	css_link_;

public:
	//userid
	uuid_session		uuid_;
	//玩家全局索引，系统定义
	int					global_index_;

	//玩家所在的gts服务器
	GTSLink*			gts_link_;
	//选服占位
	TeamProxySession*	proxy_session_;
};

inline 
bool Player::is_emptyplayer()
{
	return uuid_.userid_ == NO_INITVALUE;
}

inline 
bool Player::is_samesession( int uid, S_TIMESTAMP st)
{
	return uuid_.is_samesession( uid, st);
}

inline 
bool Player::is_samesession( uuid_session& u)
{
	return uuid_.is_samesession( u);
}

inline 
bool Player::is_lnkkeep()
{
	return this->is_lnkkeep_;
}

inline 
void Player::send_to_gts( BasicProtocol* p)
{
	if( gts_link_)
		gts_link_->send_protocol( p);
	else if( p)
		delete p;		
}

inline 
void Player::send_to_css( BasicProtocol* p)
{
	if( css_link_)
		css_link_->send_protocol( p);
	else if( p)
		delete p;
}

#endif	//__PLAYER__H__
