/**
* css application
*
* @category		pre define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSS_PRE__H__
#define __CSS_PRE__H__

#include <prolib/core_type.h>

#ifndef SCENE_PANE_IMPL
	#define SCENE_PANE_IMPL
#endif

#include <ace/Basic_Types.h>
#include <string>
#include <map>
#include <list>
#include <vector>

//玩家定时更新位置
#define PLAYER_UPDATEPOS_TIMESTEP	5

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
enum CSSAPP_SERVICE_ENUM{
	//DPX_Service
	CSSAPP_SERVICE_DPX =0,
	//CTS_Service
	CSSAPP_SERVICE_CTS,
	//CSS_Service
	CSSAPP_SERVICE_CSS,
	//CSS2_Service
	CSSAPP_SERVICE_CSS2
};

//玩家状态
enum PLAYERSTATUS_TYPE_ENUM{
	PLAYERSTATUS_NONE =0,
	//玩家注册
	PLAYERSTATUS_REGIST,
	//选线完成
	PLAYERSTATUS_INSCENE,
};

//填充玩家uuid信息
#define PLAYERC_UUID_FILL( pro, sindex, userid, stmp)	\
	{													\
##pro->set_uuidglobalindex( sindex);					\
##pro->set_uuiduserid( userid);							\
##pro->set_uuidinitstmp( stmp);							\
	}

#define PRO_UUID_FILL( pro, sindex, sess)		\
	{											\
##pro->set_uuidglobalindex( sindex);			\
##pro->set_uuiduserid( sess.userid_);			\
##pro->set_uuidinitstmp( sess.init_timestamp_);	\
	}

#define PRO_UUID_FILL2( des, src)							\
	{														\
##des->set_uuidglobalindex( ##src->uuid_.global_index_);	\
##des->set_uuiduserid( ##src->uuid_.userid_);				\
##des->set_uuidinitstmp( ##src->uuid_.init_stmp_);			\
	}

#endif //__CSS_PRE__H__
