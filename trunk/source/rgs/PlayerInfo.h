/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYERINFO__H__
#define __PLAYERINFO__H__

#include <prolib/core_type.h>

#include "LGSLink.h"
#include "CTSLink.h"

/**
* @class PlayerInfo
* 
* @brief global中用到的玩家信息
* 
**/
class PlayerInfo
{
public:
	PlayerInfo(void);
	virtual ~PlayerInfo(void);

	//清除资源准备复用
	void clear();

	/**
	* 发送协议
	* @param p
	**/
	void send_to_lgs( BasicProtocol *p){
		if( lgs_link_)
			lgs_link_->send_protocol( p);
		else if( p)
			delete p;
	}
	void send_to_cts( BasicProtocol *p){
		if( cts_link_)
			cts_link_->send_protocol( p);
		else if( p)
			delete p;
	}

public:
	/**
	* 使用初始化
	**/
	void prepare();

	/**
	* 登陆请求准备
	**/
	void login_prepare( S_INT_32 serverindex, S_TIMESTAMP session, LGSLink* l);
	void login_succ();

	void login_succ_of_lnkkeep( S_INT_32 serverindex, LGSLink* l);

	/**
	* 选服请求
	**/
	void selteam_begin( CTSLink* l);
	void selteam_end( bool succ);
	void selteam_confirm();

	void selteam_timeout();
	void quit_team();
	void quitteam_reconn( S_INT_32 serverindex, S_TIMESTAMP session, LGSLink* l);
	
	void link_keep( S_TIMESTAMP& token, S_INT_32 teamid, S_INT_32 gtsid, S_INT_32 proxyindex);
	void lnkkeep_confirm();

	inline bool is_selteamconfirm();
	inline bool is_quitteam();
	inline bool is_lnkkeep();
	inline bool is_lnkkeep_wait();

	/**
	* 退服重连超时
	* @param tnow
	* @return
	**/
	bool quitteamreconn_timeout( ACE_UINT64& tnow);

	/**
	* 断线重连超时
	* @param tnow
	**/
	bool lnkkeep_timeout( ACE_UINT64& tnow);

	/**
	* 验证是否同一个session
	* @param uuid
	**/
	inline bool is_samesession( uuid_session& uuid);

public:
	//玩家状态
	PLAYERSTATUS_TYPE_ENUM status_;
	//玩家退服/断线重连的令牌
	S_TIMESTAMP	qt_token_;
	//断线重连保存的gts索引
	S_INT_32	lnkkeep_teamid_;
	S_INT_32	lnkkeep_gtsid_;
	S_INT_32	lnkkeep_proxyindex_;

	//计时用时间戳
	ACE_UINT64	tstamp_;

	//玩家标识
	uuid_session uuid_;
	//lgs使用的临时serverindex
	S_INT_32	server_index_;

	//对应的lgs连接
	LGSLink	*lgs_link_;
	//对应的cts连接
	CTSLink	*cts_link_;
};

inline 
bool PlayerInfo::is_samesession( uuid_session& uuid)
{
	return this->uuid_.is_samesession( uuid);
}

inline 
bool PlayerInfo::is_selteamconfirm()
{
	return status_ >= PLAYERSTATUS_FIN;
}

inline 
bool PlayerInfo::is_quitteam()
{
	return status_ == PLAYERSTATUS_QUITTEAM;
}

inline 
bool PlayerInfo::is_lnkkeep()
{
	return status_ == PLAYERSTATUS_LNKKEEP;
}

inline 
bool PlayerInfo::is_lnkkeep_wait()
{
	return status_ == PLAYERSTATUS_LNKKEEP_SELTEAMING;
}

#endif //__PLAYERINFO__H__
