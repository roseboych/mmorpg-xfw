/**
* dpx application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "../dpx_pre.h"

#include <prolib/BasicProtocol.h>

#include "../CSSLink.h"
#include "PlayerCache.h"

/**
* @class Player
* 
* @brief 玩家信息
**/
class Player : public PlayerCache
{
public:
	Player();

	/**
	* 玩家注册
	* @param id
	**/
	void regist( uuid_session& id);

	/**
	* 重置玩家
	**/
	void reset();

	/**
	* 是否同一个session
	* @param u
	* @return
	**/
	inline bool is_samesession( uuid_session& u);

	/**
	* 发送协议到css服务器
	* @param p
	**/
	void send_to_css( BasicProtocol* p);

public:
	/**
	* 构造角色列表协议
	**/
	BasicProtocol* construct_chrlistpro();

	/**
	* 设置当前角色
	* @param cid
	**/
	void set_curchrid( int cid);

public:
	//当前使用的角色
	int			chrid_;
	//所在的csslink
	CSSLink*	css_svr_;

public:
	//玩家的唯一标识
	uuid_session	uuid_;
	//玩家全局索引，系统定义
	int				global_index_;
};

inline 
bool Player::is_samesession( uuid_session& u)
{
	return this->uuid_.is_samesession( u);
}

#endif	//__PLAYER__H__
