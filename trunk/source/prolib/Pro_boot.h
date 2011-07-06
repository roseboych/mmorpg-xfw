/**
* protocol module
*
* 定义了服务器组启动类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PRO_BOOT__H__
#define __PRO_BOOT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/boost.h"
#include "prolib/BasicProtocol.h"

#include <list>

#pragma warning(disable:4121)

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_SvrRegist_req
* 
* @brief 服务器注册请求()
**/
class Pro_SvrRegist_req : public AppProtocol<Pro_SvrRegist_req>
{
	typedef AppProtocol<Pro_SvrRegist_req> inherit;

public:
	Pro_SvrRegist_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, svr_type_)
		LOAD_INT32_PRO( pdata, totlen, regionsn_)
		LOAD_INT32_PRO( pdata, totlen, appsn_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, svr_type_)
		SAVE_INT32_PRO( pdata, buflen, len, regionsn_)
		SAVE_INT32_PRO( pdata, buflen, len, appsn_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrRegist_req, proo)
		CLONE_VAR_PRO( proo, svr_type_)
		CLONE_VAR_PRO( proo, regionsn_)
		CLONE_VAR_PRO( proo, appsn_)
	END_CLONE_PRO()

public:
	//服务器类型 NETSVR_TYPE_ENUM定义
	S_INT_8	svr_type_;
	//服务器组id rgs, lgs不需要设置
	S_INT_32 regionsn_;
	//服务器sn
	S_INT_32 appsn_;
};

/**
* @class Pro_SvrRegist_req
* 
* @brief 服务器注册请求回复
**/
class Pro_SvrRegist_ack : public AppProtocol<Pro_SvrRegist_ack>
{
	typedef AppProtocol<Pro_SvrRegist_ack> inherit;

public:
	Pro_SvrRegist_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrRegist_ack, proo)
		CLONE_VAR_PRO( proo, result_)
	END_CLONE_PRO()

public:
	//0:成功 1:失败
	S_INT_8	result_;
};

/**
* @class Pro_SvrOnlineTick_NTF
* 
* @brief 服务器心跳
**/
class Pro_SvrOnlineTick_NTF : public AppProtocol<Pro_SvrOnlineTick_NTF>
{
	typedef AppProtocol<Pro_SvrOnlineTick_NTF> inherit;

public:
	Pro_SvrOnlineTick_NTF();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrOnlineTick_NTF, proo)
	END_CLONE_PRO()

};

/**
* @class Pro_SvrTeamInfo_NTF
* 
* @brief RGS发送的各个服务器组的更新信息 rgs->lgs lgs->client
**/
class Pro_SvrTeamInfo_NTF : public AppProtocol<Pro_SvrTeamInfo_NTF>
{
	typedef AppProtocol<Pro_SvrTeamInfo_NTF> inherit;

public:
	struct svrteaminfo
	{
		//server_index 索引，选择服务器组时使用
		S_INT_32	server_index_;
		//服务器组id
		S_INT_32	teamid_;
		//最大玩家数
		S_INT_32	maxuser_;
		//当前玩家数
		S_INT_32	curuser_;
		//是否有效 0:false 1:true
		S_INT_8		is_validate_;
	};

	typedef NS_STL::list< svrteaminfo> SVRTEAMINF_LIST;

public:
	Pro_SvrTeamInfo_NTF();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_LIST_PRO_BEGIN( pdata, totlen, svrteaminfo)
			LOAD_INT32_PRO_LS( pdata, totlen, server_index_)
			LOAD_INT32_PRO_LS( pdata, totlen, teamid_)
			LOAD_INT32_PRO_LS( pdata, totlen, maxuser_)
			LOAD_INT32_PRO_LS( pdata, totlen, curuser_)
			LOAD_INT8_PRO_LS( pdata, totlen, is_validate_)
		LOAD_LIST_PRO_END( teams_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, teams_, svrteaminfo)
			SAVE_INT32_PRO_LS( pdata, buflen, len, server_index_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, teamid_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, maxuser_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, curuser_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, is_validate_)
		SAVE_LIST_PRO_END()
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrTeamInfo_NTF, proo)
		CLONE_LIST_PRO( proo, teams_)
	END_CLONE_PRO()

public:
	SVRTEAMINF_LIST	teams_;
};

/**
* @class Pro_SvrSelTeamTimeout_NTF
* 
* @brief 选服务器组超时通知
**/
class Pro_SvrSelTeamTimeout_NTF : public AppProtocol<Pro_SvrSelTeamTimeout_NTF>
{
	typedef AppProtocol<Pro_SvrSelTeamTimeout_NTF> inherit;

public:
	Pro_SvrSelTeamTimeout_NTF();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeamTimeout_NTF, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
	END_CLONE_PRO()

public:
	//超时的服务器组
	S_INT_32	proxy_index_;
};

/**
* @class Pro_SvrSelTeam_req
* 
* @brief 选服务器组请求
**/
class Pro_SvrSelTeam_req : public AppProtocol<Pro_SvrSelTeam_req>
{
	typedef AppProtocol<Pro_SvrSelTeam_req> inherit;

public:
	Pro_SvrSelTeam_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, team_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, team_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeam_req, proo)
		CLONE_VAR_PRO( proo, team_index_)
	END_CLONE_PRO()

public:
	//选择的服务器组
	S_INT_32	team_index_;
};

/**
* @class Pro_SvrSelTeam_ack
* 
* @brief 选服务器组请求回复
**/
class Pro_SvrSelTeam_ack : public AppProtocol<Pro_SvrSelTeam_ack>
{
	typedef AppProtocol<Pro_SvrSelTeam_ack> inherit;

public:
	Pro_SvrSelTeam_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, team_index_)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
		LOAD_STR_PRO( pdata, totlen, gts_ip_)
		LOAD_INT16_PRO( pdata, totlen, gts_port_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, team_index_)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
		SAVE_STR_PRO( pdata, buflen, len, gts_ip_)
		SAVE_INT16_PRO( pdata, buflen, len, gts_port_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeam_ack, proo)
		CLONE_VAR_PRO( proo, team_index_)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, proxy_index_)
		CLONE_VAR_PRO( proo, token_)
		CLONE_VAR_PRO( proo, gts_ip_)
		CLONE_VAR_PRO( proo, gts_port_)
	END_CLONE_PRO()

public:
	//选择的服务器组
	S_INT_32	team_index_;
	//0:成功 1:服务器组不可用 2:服务器组人满 3:选择服务器组失败 4:已经发送过选组请求 5:断线保持不能选服
	S_INT_8		result_;
	//proxy_index
	S_INT_32	proxy_index_;
	//返回的令牌
	S_TIMESTAMP	token_;
	//可以连接的gts信息
	StaticString<24>	gts_ip_;
	S_INT_16	gts_port_;
};

/**
* @class Pro_SvrSelTeamProxy_req
* 
* @brief 选服务器组占位请求
**/
class Pro_SvrSelTeamProxy_req : public AppProtocol<Pro_SvrSelTeamProxy_req>
{
	typedef AppProtocol<Pro_SvrSelTeamProxy_req> inherit;

public:
	Pro_SvrSelTeamProxy_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeamProxy_req, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
	END_CLONE_PRO()

public:
	//proxy index
	S_INT_32	proxy_index_;
};

/**
* @class Pro_SvrSelTeamProxy_ack
* 
* @brief 选服务器组占位请求回复
**/
class Pro_SvrSelTeamProxy_ack : public AppProtocol<Pro_SvrSelTeamProxy_ack>
{
	typedef AppProtocol<Pro_SvrSelTeamProxy_ack> inherit;

public:
	Pro_SvrSelTeamProxy_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, userid_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
		LOAD_INT32_PRO( pdata, totlen, gts_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, userid_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
		SAVE_INT32_PRO( pdata, buflen, len, gts_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeamProxy_ack, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, userid_)
		CLONE_VAR_PRO( proo, token_)
		CLONE_VAR_PRO( proo, gts_index_)
	END_CLONE_PRO()

public:
	S_INT_32	proxy_index_;
	//0:成功 1:失败
	S_INT_8		result_;
	S_INT_32	userid_;
	S_TIMESTAMP	token_;
	//gts索引
	S_INT_32	gts_index_;
};

/**
* @class Pro_SvrSelTeamConfirm_req
* 
* @brief 选服确认请求
**/
class Pro_SvrSelTeamConfirm_req : public AppProtocol<Pro_SvrSelTeamConfirm_req>
{
	typedef AppProtocol<Pro_SvrSelTeamConfirm_req> inherit;

public:
	Pro_SvrSelTeamConfirm_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeamConfirm_req, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
		CLONE_VAR_PRO( proo, token_)
	END_CLONE_PRO()

public:
	//系统分配的代理编号
	S_INT_32	proxy_index_;
	//验证用令牌
	S_TIMESTAMP	token_;
};

/**
* @class Pro_SvrSelTeamConfirm_ack
* 
* @brief 选服确认请求
**/
class Pro_SvrSelTeamConfirm_ack : public AppProtocol<Pro_SvrSelTeamConfirm_ack>
{
	typedef AppProtocol<Pro_SvrSelTeamConfirm_ack> inherit;

public:
	Pro_SvrSelTeamConfirm_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, userid_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, init_stmp_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, userid_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, init_stmp_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrSelTeamConfirm_ack, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, userid_)
		CLONE_VAR_PRO( proo, init_stmp_)
	END_CLONE_PRO()

public:
	//选择的服务器组
	S_INT_32	proxy_index_;
	//0:成功 1:占位不存在 2:令牌验证失败
	S_INT_8		result_;
	//服务器使用
	S_INT_32	userid_;
	S_TIMESTAMP	init_stmp_;
};

/**
* @class Pro_SvrQuitTeam_req
* 
* @brief 退出服务器组请求
**/
class Pro_SvrQuitTeam_req : public AppProtocol<Pro_SvrQuitTeam_req>
{
	typedef AppProtocol<Pro_SvrQuitTeam_req> inherit;

public:
	Pro_SvrQuitTeam_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrQuitTeam_req, proo)
	END_CLONE_PRO()

public:
};

/**
* @class Pro_SvrQuitTeam_ack
* 
* @brief 退出服务器组请求回复
**/
class Pro_SvrQuitTeam_ack : public AppProtocol<Pro_SvrQuitTeam_ack>
{
	typedef AppProtocol<Pro_SvrQuitTeam_ack> inherit;

public:
	Pro_SvrQuitTeam_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrQuitTeam_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, token_)
	END_CLONE_PRO()

public:
	//0:成功 1:失败
	S_INT_8		result_;
	//重连用令牌
	S_TIMESTAMP	token_;
};

/**
* @class Pro_SvrQuitTeamReconn_req
* 
* @brief 退出服务器组请求回复
**/
class Pro_SvrQuitTeamReconn_req : public AppProtocol<Pro_SvrQuitTeamReconn_req>
{
	typedef AppProtocol<Pro_SvrQuitTeamReconn_req> inherit;

public:
	Pro_SvrQuitTeamReconn_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, userid_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, userid_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrQuitTeamReconn_req, proo)
		CLONE_VAR_PRO( proo, userid_)
		CLONE_VAR_PRO( proo, token_)
	END_CLONE_PRO()

public:
	//玩家userid_
	S_INT_32	userid_;
	//重连用令牌
	S_TIMESTAMP	token_;
};

/**
* @class Pro_SvrQuitTeamReconn_ack
* 
* @brief 退出服务器组请求回复
**/
class Pro_SvrQuitTeamReconn_ack : public AppProtocol<Pro_SvrQuitTeamReconn_ack>
{
	typedef AppProtocol<Pro_SvrQuitTeamReconn_ack> inherit;

public:
	Pro_SvrQuitTeamReconn_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, userid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, userid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrQuitTeamReconn_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, userid_)
	END_CLONE_PRO()

public:
	//0:成功 1:玩家不存在 2:令牌错误 3:不是退服玩家
	S_INT_8		result_;
	S_INT_32	userid_;
};

/**
* @class Pro_SvrInstUpdate_ntf
* 
* @brief 副本服务器状态更新通知
**/
class Pro_SvrInstUpdate_ntf : public AppProtocol<Pro_SvrInstUpdate_ntf>
{
	typedef AppProtocol<Pro_SvrInstUpdate_ntf> inherit;

public:
	Pro_SvrInstUpdate_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, cellid_)
		LOAD_INT8_PRO( pdata, totlen, state_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, cellid_)
		SAVE_INT8_PRO( pdata, buflen, len, state_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrInstUpdate_ntf, proo)
		CLONE_VAR_PRO( proo, cellid_)
		CLONE_VAR_PRO( proo, state_)
	END_CLONE_PRO()

public:
	//副本编号
	S_INT_32	cellid_;
	//1:不可用 2:可以进入 3:拒绝进入
	S_INT_8		state_;
};

/**
* @class Pro_SvrUserLost_NTF
* 
* @brief 服务器内部使用的玩家断线通知
**/
class Pro_SvrUserLost_NTF : public AppProtocol<Pro_SvrUserLost_NTF>
{
	typedef AppProtocol<Pro_SvrUserLost_NTF> inherit;

public:
	Pro_SvrUserLost_NTF();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrUserLost_NTF, proo)
		CLONE_VAR_PRO( proo, proxy_index_)
	END_CLONE_PRO()

public:
	//临时使用cts->gts,等待注册时使用
	S_INT_32	proxy_index_;
};

/**
* @class Pro_Login_req
* 
* @brief 玩家登陆请求
**/
class Pro_Login_req : public AppProtocol<Pro_Login_req>
{
	typedef AppProtocol<Pro_Login_req> inherit;

public:
	Pro_Login_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_STR_PRO( pdata, totlen, token_ )
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_STR_PRO( pdata, buflen, len, token_ )
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_Login_req, proo)
		CLONE_VAR_PRO( proo, token_ )
	END_CLONE_PRO()

public:
	//username+password
	StaticString<64>	token_;
};

/**
* @class Pro_Login_ack
* 
* @brief 玩家登陆回复
**/
class Pro_Login_ack : public AppProtocol<Pro_Login_ack>
{
	typedef AppProtocol<Pro_Login_ack> inherit;
public:
	Pro_Login_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, userid_)
		LOAD_INT32_PRO( pdata, totlen, validatetime_)
		LOAD_INT16_PRO( pdata, totlen, lastregion_)
		LOAD_INT32_PRO( pdata, totlen, lastlogout_)
		LOAD_INT8_PRO( pdata, totlen, is_lnkkeep_)
		LOAD_STR_PRO( pdata, totlen, gts_ip_)
		LOAD_INT16_PRO( pdata, totlen, gts_port_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, session_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, userid_)
		SAVE_INT32_PRO( pdata, buflen, len, validatetime_);
		SAVE_INT16_PRO( pdata, buflen, len, lastregion_)
		SAVE_INT32_PRO( pdata, buflen, len, lastlogout_)
		SAVE_INT8_PRO( pdata, buflen, len, is_lnkkeep_)
		SAVE_STR_PRO( pdata, buflen, len, gts_ip_)
		SAVE_INT16_PRO( pdata, buflen, len, gts_port_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, session_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_Login_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, userid_)
		CLONE_VAR_PRO( proo, validatetime_)
		CLONE_VAR_PRO( proo, lastregion_)
		CLONE_VAR_PRO( proo, lastlogout_)
		CLONE_VAR_PRO( proo, is_lnkkeep_)
		CLONE_VAR_PRO( proo, gts_ip_)
		CLONE_VAR_PRO( proo, gts_port_)
		CLONE_VAR_PRO( proo, token_)
		CLONE_VAR_PRO( proo, proxy_index_)
		CLONE_VAR_PRO( proo, session_)
	END_CLONE_PRO()

public:
	//0:成功 1:用户名密码错误 2:玩家已经登陆 3:登陆冷却中 4:其他错误 5:大区已经满 6:数据库错误 7:帐号不存在
	S_INT_8	result_;
	S_INT_32	userid_;
	//可用的在线时间
	S_INT_32	validatetime_;
	//最后登陆服务器组
	S_INT_16	lastregion_;
	//最后登陆时间
	S_INT_32	lastlogout_;
	//是否断线重连 0:不是 1:是	缺省:0
	S_INT_8		is_lnkkeep_;
	//如果is_lnkkeep_ == 1 有效
	StaticString<24>	gts_ip_;
	S_INT_16	gts_port_;
	//如果is_lnkkeep_ == 1 有效	重连令牌
	S_TIMESTAMP	token_;
	//如果is_lnkkeep_ == 1 有效
	S_INT_32	proxy_index_;
	//如果is_lnkkeep_ == 1 有效 更新lgs的session	[服务器使用]
	S_TIMESTAMP	session_;
};

/**
* @class Pro_UserRegist_ntf
* 
* @brief 玩家注册通知 to dpx
**/
class Pro_UserRegist_ntf : public AppProtocol<Pro_UserRegist_ntf>
{
	typedef AppProtocol<Pro_UserRegist_ntf> inherit;

public:
	Pro_UserRegist_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_UserRegist_ntf, proo)
	END_CLONE_PRO()

public:
};

/**
* @class Pro_SvrDisconnKeep_req
* 
* @brief 玩家断线保持通知
**/
class Pro_SvrDisconnKeep_req : public AppProtocol<Pro_SvrDisconnKeep_req>
{
	typedef AppProtocol<Pro_SvrDisconnKeep_req> inherit;

public:
	Pro_SvrDisconnKeep_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_TIMESTAMP_PRO( pdata, totlen, token_)
		LOAD_INT32_PRO( pdata, totlen, team_id_)
		LOAD_INT32_PRO( pdata, totlen, gts_id_)
		LOAD_INT32_PRO( pdata, totlen, proxy_index_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, token_)
		SAVE_INT32_PRO( pdata, buflen, len, team_id_)
		SAVE_INT32_PRO( pdata, buflen, len, gts_id_)
		SAVE_INT32_PRO( pdata, buflen, len, proxy_index_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrDisconnKeep_req, proo)
		CLONE_VAR_PRO( proo, token_)
		CLONE_VAR_PRO( proo, team_id_)
		CLONE_VAR_PRO( proo, gts_id_)
		CLONE_VAR_PRO( proo, proxy_index_)
	END_CLONE_PRO()

public:
	//附带的令牌信息
	S_TIMESTAMP	token_;
	//team index
	S_INT_32	team_id_;
	//gts索引
	S_INT_32	gts_id_;
	//proxy index
	S_INT_32	proxy_index_;
};

/**
* @class Pro_SvrLnkkeepLoad_req
* 
* @brief 玩家登陆请求
**/
class Pro_SvrLnkkeepLoad_req : public AppProtocol<Pro_SvrLnkkeepLoad_req>
{
	typedef AppProtocol<Pro_SvrLnkkeepLoad_req> inherit;

public:
	Pro_SvrLnkkeepLoad_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SvrLnkkeepLoad_req, proo)
	END_CLONE_PRO()

public:
};

/**
* @class Pro_Logout_req
* 
* @brief 玩家注销请求
**/
class Pro_Logout_req : public AppProtocol<Pro_Logout_req>
{
	typedef AppProtocol<Pro_Logout_req> inherit;

public:
	Pro_Logout_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_Logout_req, proo)
	END_CLONE_PRO()

public:
};

/**
* @class Pro_Logout_ntf
* 
* @brief 玩家注销通知
**/
class Pro_Logout_ntf : public AppProtocol<Pro_Logout_ntf>
{
	typedef AppProtocol<Pro_Logout_ntf> inherit;
public:
	Pro_Logout_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_Logout_ntf, proo)
		CLONE_VAR_PRO( proo, chrid_)
	END_CLONE_PRO()

public:
	//注销人
	S_INT_32	chrid_;
};

PROTOCOL_NAMESPACE_END

#endif //__PRO_BOOT__H__
