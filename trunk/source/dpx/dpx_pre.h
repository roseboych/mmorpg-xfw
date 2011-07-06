/**
* dpx application
*
* @category		pre define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPX_PRE__H__
#define __DPX_PRE__H__

#include <prolib/core_type.h>
#include <prolib/StaticString.h>
#include <prolib/pre.h>

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
enum DPXAPP_SERVICE_ENUM{
	//DPX_Service
	DPXAPP_SERVICE_DPX =0,
	//CTS_Service
	DPXAPP_SERVICE_CTS
};

#define PRO_UUID_FILL( pro, sindex, sess)	\
	{	\
##pro->set_uuidglobalindex( sindex);	\
##pro->set_uuiduserid( sess.userid_);	\
##pro->set_uuidinitstmp( sess.init_timestamp_);	\
	}

#endif //__DPX_PRE__H__
