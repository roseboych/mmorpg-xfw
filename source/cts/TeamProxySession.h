/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __TEAMPROXYSESSION__H__
#define __TEAMPROXYSESSION__H__

#include "cts_pre.h"

#include <prolib/BasicProtocol.h>
#include "GTSLink.h"

class Player;

/**
* @class TeamProxySession
* 
* @brief 选组代理占位session
**/
class TeamProxySession
{
public:
	TeamProxySession();

	/**
	* 重置状态
	**/
	void reset();

	void proxy( S_INT_32 uid, S_TIMESTAMP st, S_INT_32 si);

	void proxy_ack( S_TIMESTAMP st);

	void selteam_confirm( Player* p);

	inline bool is_samesession( S_INT_32 uid, S_TIMESTAMP st);
	inline bool is_proxysame( S_INT_32 uid, S_TIMESTAMP st);

	inline void send_to_gts( BasicProtocol* p);

	bool proxy_timeover( ACE_UINT64& now);

public:
	//保存玩家的session记录
	uuid_session	uuid_;
	S_INT_32	server_index_;

	//生成的token
	S_TIMESTAMP	proxy_timestamp_;

	//检测用时间戳
	ACE_UINT64 check_stmp_;

	//关联的玩家
	Player*	player_;

	//proxy 编号
	S_INT_32	proxy_index_;
	//玩家所在的gts服务器
	GTSLink*	gts_link_;
};

inline 
bool TeamProxySession::is_samesession( S_INT_32 uid, S_TIMESTAMP st)
{
	return uuid_.is_samesession( uid, st);
}

inline 
bool TeamProxySession::is_proxysame( S_INT_32 uid, S_TIMESTAMP st)
{
	return uid == uuid_.userid_ && st == proxy_timestamp_;
}

inline
void TeamProxySession::send_to_gts( BasicProtocol* p)
{
	if( gts_link_)
		gts_link_->send_protocol( p);
	else if( p)
		delete p;
}

#endif	//__TEAMPROXYSESSION__H__
