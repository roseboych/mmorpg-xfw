/**
* rts application
*
* @category		pre define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGS_PRE__H__
#define __RGS_PRE__H__

#include <prolib/core_type.h>
#include <corelib/corelibdef.h>
//log macro
#include <corelib/log/logmacro.h>

#include <string>
#include <map>
#include <list>

//app定义的service类型
enum RGSAPP_SERVICE_ENUM{
	//RGS_Service
	RGSAPP_SERVICE_RGS =0,
	//Controller_Service
	RGSAPP_SERVICE_CONTROLLER,
};

//玩家状态定义
enum PLAYERSTATUS_TYPE_ENUM{
	PLAYERSTATUS_NONE =0,
	//等待验证身份
	PLAYERSTATUS_WAITLOGIN,
	//正在验证
	PLAYERSTATUS_LOGINING,
	//等待选择服务器组
	PLAYERSTATUS_SELTEAM,
	//选择服务器组进行中
	PLAYERSTATUS_SELTEAMING,
	//等待gts验证
	PLAYERSTATUS_GTSVALIDATE,
	//登陆完成
	PLAYERSTATUS_FIN,
	//退服
	PLAYERSTATUS_QUITTEAM,
	//断线保持
	PLAYERSTATUS_LNKKEEP,
	//断线保持，等待选服
	PLAYERSTATUS_LNKKEEP_SELTEAMING,
};

//填充玩家uuid信息
#define PLAYERC_UUID_FILL( pro, sindex, userid, stmp)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( userid);	\
##pro->set_uuidinitstmp( stmp);	\
	}

#define PROTOCOL_UUID_FILL( pro, sindex, sess)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( sess.userid_);	\
##pro->set_uuidinitstmp( sess.init_timestamp_);	\
	}

#endif //__RGS_PRE__H__
