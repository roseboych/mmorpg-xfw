/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __TEAMPROXYSESSION__H__
#define __TEAMPROXYSESSION__H__

#include "gts_pre.h"

class Player;
class CSSLink;

enum PROXY_STATUS_ENUM{
	//none
	PROXYSTATUS_NONE =0,
	//占位成功等待验证
	PROXYSTATUS_CONFIRM,
	//验证通过
	PROXYSTATUS_FIN,
	//连接保持
	PROXYSTATUS_LNKKEEP,
};

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
	void lnkkeep_reset();

	/**
	* 代理
	* @param uid
	**/
	void proxy( int uid, S_TIMESTAMP st);

	void proxy_confirm( Player* p);

	inline bool is_freeproxy();
	inline bool is_waitconfirm();
	inline bool is_lnkkeep();

	inline bool confirm_token( S_TIMESTAMP token);

public:
	PROXY_STATUS_ENUM	status_;
	//保存玩家的session记录
	uuid_session		uuid_;

	//断线保持时临时存储的css指向
	CSSLink*			lnkkeep_csslink_;
	int					lnkkeep_chrid_;

	S_TIMESTAMP token_;

	Player*		player_;

	//系统使用
	int			proxy_index_;
};

inline 
bool TeamProxySession::is_freeproxy()
{
	return status_ == PROXYSTATUS_NONE;
}

inline 
bool TeamProxySession::is_waitconfirm()
{
	return status_ == PROXYSTATUS_CONFIRM;
}

inline 
bool TeamProxySession::is_lnkkeep()
{
	return status_ == PROXYSTATUS_LNKKEEP;
}

inline 
bool TeamProxySession::confirm_token( S_TIMESTAMP token)
{
	return this->token_ == token;
}


#endif	//__TEAMPROXYSESSION__H__
