/**
* protocol module
*
* 定义了服务器组启动类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BOOST__H__
#define __BOOST__H__

enum PRO_BOOST_ENUM
{
	// 服务器初始化协议
	GAME_INIT	=0,
	// 服务器注册
	SVR_REGIST_REQ,
	SVR_REGIST_ACK,
	//网络心跳
	SVR_ONLINETICK_NTF,

	// 服务器组处理
	//RGS发送的各个服务器组的更新信息
	SVR_SVRTEAMINFO_NTF,
	//选服超时通知
	SVR_SELTEAMTIMEOUT_NTF,
	//选服请求
	SVR_SELSERVER_REQ,
	SVR_SELSERVER_ACK,
	//选服占位协议
	SVR_SELSERVERPROXY_REQ,
	SVR_SELSERVERPROXY_ACK,
	//选服确认
	SVR_SELSERVERCONFIRM_REQ,
	SVR_SELSERVERCONFIRM_ACK,
	//退服请求
	SVR_QUITSERVER_REQ,
	SVR_QUITSERVER_ACK,
	//退服重连请求
	SVR_QUITSERVERRECONN_REQ,
	SVR_QUITSERVERRECONN_ACK,
	//断线保持
	SVR_DISCONNKEEP_NTF,
	SVR_LNKEEPLOAD_NTF,
	//副本状态更新通知
	SVR_INSTUPDATE_NTF,

	//玩家断线，不需返回
	SVR_USERLOST_NTF,

	// 玩家登陆
	GAME_LOGIN_REQ,
	GAME_LOGIN_ACK,
	GAME_USERREGIST_NTF,
	//玩家注销
	GAME_LOGOUT_REQ,
	//注销通知
	GAME_LOGOUT_NTF,

	//角色管理
	//角色列表请求
	GAME_CHRLIST_REQ,
	GAME_CHRLIST_ACK,
	//创建角色
	GAME_CHRCREATE_REQ,
	GAME_CHRCREATE_ACK,
	//删除角色
	GAME_CHRDEL_REQ,
	GAME_CHRDEL_ACK,
	//选择角色
	GAME_CHRSEL_REQ,
	GAME_CHRSEL_ACK,
	//角色注册到世界
	GAME_CHRREGTOWORLD_REQ,

	//玩家角色数据加载
	GAME_CHRLOAD_REQ,
	GAME_CHRLOAD_ACK,
	//选线结束后的玩家信息初始化完成通知
	GAME_CHRFIN_NTF,

	//AOI
	AOI_UNITENTER_NTF,
	AOI_UNITLEAVE_NTF,
	AOI_TELEPORT_REQ,
	AOI_TELEPORT_ACK,
	//进入退出副本
	AOI_ENTERINS_REQ,
	AOI_ENTERINS_ACK,
	//进入副本超时
	AOI_ENTERINSOT_NTF,
	//进入副本确认
	AOI_ENTERINSCONFIRM_NTF,

	// 初始化协议最大序号
	GAME_MAX,
};

#endif	//__BOOST__H__
