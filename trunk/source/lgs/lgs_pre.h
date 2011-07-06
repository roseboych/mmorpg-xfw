/**
* lgs application
*
* @category		pre define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGS_PRE__H__
#define __LGS_PRE__H__

#include <prolib/core_type.h>

//log macro
#include <corelib/log/logmacro.h>

#include <string>
#include <map>
#include <list>
#include <vector>

//服务器注册状态
enum SERVER_REGIST_STATUS_ENUM{
	//未连接
	SVRREGIST_ST_NONE =0,
	//等待回复
	SVRREGIST_ST_WAITACK,
	//注册成功
	SVRREGIST_ST_SUCC
};

//app定义的service类型
enum LGSAPP_SERVICE_ENUM{
	//RGS_Service
	LGSAPP_SERVICE_RGS =0,
	//LGS_Service
	LGSAPP_SERVICE_LGS
};

//定义了玩家状态
enum PLAYER_STATUS_ENUM{
	//空闲玩家
	PLAYERSTATUS_NONE =0,
	//等待登陆请求
	PLAYERSTATUS_WAITLOGIN,
	//等待登陆验证
	PLAYERSTATUS_LOGINING,
	//登陆成功
	PLAYERSTATUS_LOGINS,
	//等待选服务器组
	PLAYERSTATUS_WAITSELTEAM,
	//等待登陆服务器组
	PLAYERSTATUS_LOGINTEAM,
	//登陆服务器组成功
	PLAYERSTATUS_LOGINTEAMS,
	//退服重连
	PLAYERSTATUS_QUITTEAMRECONN,
	//断线保持
	PLAYERSTATUS_LNKKEEP,
};

//填充玩家uuid信息
#define PLAYERC_UUID_FILL( pro, sindex, uid, st)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( uid);	\
##pro->set_uuidinitstmp( st);	\
	}

//填充玩家uuid信息
#define SESSION_UUID_FILL( pro, sindex, sess)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( sess.userid_);	\
##pro->set_uuidinitstmp( sess.init_timestamp_);	\
	}

#endif //__LGS_PRE__H__
