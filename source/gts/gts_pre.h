/**
* gts application
*
* @category		pre define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTS_PRE__H__
#define __GTS_PRE__H__

#include <prolib/core_type.h>

#include <prolib/StaticString.h>

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
enum GTSAPP_SERVICE_ENUM{
	//CSS_Service
	GTSAPP_SERVICE_CSS =0,
	//CTS_Service
	GTSAPP_SERVICE_CTS,
	//GTS_Service
	GTSAPP_SERVICE_GTS
};

//玩家状态定义
enum PLAYERSTATUS_TYPE_ENUM{
	PLAYERSTATUS_NONE =0,
	//占位等待
	PLAYERSTATUS_PROXYW,
	//选服成功
	PLAYERSTATUS_INTEAM,
	//选角色成功
	PLAYERSTATUS_INGAME,
	//选服退出
	PLAYERSTATUS_OUTTEAM,
	PLAYERSTATUS_OUTTEAM_FIN,
	//注销
	PLAYERSTATUS_LOGOUT,
};

//填充玩家uuid信息
#define PLAYERC_UUID_FILL( pro, sindex, userid, stmp)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( userid);	\
##pro->set_uuidinitstmp( stmp);	\
	}

#define PRO_UUID_FILL( pro, sindex, sess)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( sess.userid_);	\
##pro->set_uuidinitstmp( sess.init_timestamp_);	\
	}

#define PRO_UUID_FILL2( des, src)							\
	{														\
##des->set_uuidglobalindex( ##src->uuid_.global_index_);	\
##des->set_uuiduserid( ##src->uuid_.userid_);				\
##des->set_uuidinitstmp( ##src->uuid_.init_stmp_);			\
	}

#endif //__GTS_PRE__H__
