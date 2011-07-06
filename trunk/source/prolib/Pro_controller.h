/**
* protocol module
*
* 定义了服务器组管理类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_CONTROLLER__H__
#define __PRO_CONTROLLER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/server_controller.h"
#include "prolib/BasicProtocol.h"

#include <list>

#pragma warning(disable:4121)

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_CtrlLogin_req
* 
* @brief 管理工具登陆
**/
class Pro_CtrlLogin_req : public AppProtocol<Pro_CtrlLogin_req>
{
	typedef AppProtocol<Pro_CtrlLogin_req> inherit;

public:
	Pro_CtrlLogin_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_STR_PRO( pdata, totlen, name_)
		LOAD_STR_PRO( pdata, totlen, pwd_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_STR_PRO( pdata, buflen, len, name_)
		SAVE_STR_PRO( pdata, buflen, len, pwd_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_CtrlLogin_req, proo)
		CLONE_VAR_PRO( proo, name_)
		CLONE_VAR_PRO( proo, pwd_)
	END_CLONE_PRO()

public:
	StaticString<16>	name_;
	StaticString<16>	pwd_;
};

/**
* @class Pro_CtrlLogin_ack
* 
* @brief 管理工具登陆
**/
class Pro_CtrlLogin_ack : public AppProtocol<Pro_CtrlLogin_ack>
{
	typedef AppProtocol<Pro_CtrlLogin_ack> inherit;

public:
	Pro_CtrlLogin_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_CtrlLogin_ack, proo)
		CLONE_VAR_PRO( proo, result_)
	END_CLONE_PRO()

public:
	//0:success 1:failed
	S_INT_8	result_;
};

/**
* @class Pro_CtrlProfilerFilterCtrl
* 
* @brief 性能分析参数设置
**/
class Pro_CtrlProfilerFilterCtrl : public AppProtocol<Pro_CtrlProfilerFilterCtrl>
{
	typedef AppProtocol<Pro_CtrlProfilerFilterCtrl> inherit;

public:
	Pro_CtrlProfilerFilterCtrl();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, machine_type_)
		LOAD_INT32_PRO( pdata, totlen, machine_id1_)
		LOAD_INT32_PRO( pdata, totlen, machine_id2_)
		LOAD_INT32_PRO( pdata, totlen, profiler_scope_)
		LOAD_INT32_PRO( pdata, totlen, profiler_thread_)
		LOAD_INT8_PRO( pdata, totlen, level_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, machine_type_)
		SAVE_INT32_PRO( pdata, buflen, len, machine_id1_)
		SAVE_INT32_PRO( pdata, buflen, len, machine_id2_)
		SAVE_INT32_PRO( pdata, buflen, len, profiler_scope_)
		SAVE_INT32_PRO( pdata, buflen, len, profiler_thread_)
		SAVE_INT8_PRO( pdata, buflen, len, level_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_CtrlProfilerFilterCtrl, proo)
		CLONE_VAR_PRO( proo, machine_type_)
		CLONE_VAR_PRO( proo, machine_id1_)
		CLONE_VAR_PRO( proo, machine_id2_)
		CLONE_VAR_PRO( proo, profiler_scope_)
		CLONE_VAR_PRO( proo, profiler_thread_)
		CLONE_VAR_PRO( proo, level_)
	END_CLONE_PRO()

public:
	//1:rgs 2:lgs 3:cts 4:dpx 5:css 6:gts
	S_INT_8		machine_type_;
	//when machine_type = 2 machine_id1_ = lgs sn
	//when machine_type = 3, 4, 5, 6 machine_id1_ = svrteam sn
	S_INT_32	machine_id1_;
	//when machine_type = 3, 4, 5, 6 machine_id2_ = sn
	S_INT_32	machine_id2_;
	//filter flag
	S_INT_32	profiler_scope_;
	S_INT_32	profiler_thread_;
	S_INT_8		level_;
};

/**
* @class Pro_CtrlProfilerFilterCtrl
* 
* @brief 性能分析开关控制
**/
class Pro_CtrlProfilerAnalysisCtrl : public AppProtocol<Pro_CtrlProfilerAnalysisCtrl>
{
	typedef AppProtocol<Pro_CtrlProfilerAnalysisCtrl> inherit;

public:
	Pro_CtrlProfilerAnalysisCtrl();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, machine_type_)
		LOAD_INT32_PRO( pdata, totlen, machine_id1_)
		LOAD_INT32_PRO( pdata, totlen, machine_id2_)
		LOAD_INT8_PRO( pdata, totlen, ctrl_)
		LOAD_INT8_PRO( pdata, totlen, transfer_)
		LOAD_STR_PRO( pdata, totlen, transfer_net_ip_)
		LOAD_INT32_PRO( pdata, totlen, transfer_net_port_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, machine_type_)
		SAVE_INT32_PRO( pdata, buflen, len, machine_id1_)
		SAVE_INT32_PRO( pdata, buflen, len, machine_id2_)
		SAVE_INT8_PRO( pdata, buflen, len, ctrl_)
		SAVE_INT8_PRO( pdata, buflen, len, transfer_)
		SAVE_STR_PRO( pdata, buflen, len, transfer_net_ip_)
		SAVE_INT32_PRO( pdata, buflen, len, transfer_net_port_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_CtrlProfilerAnalysisCtrl, proo)
		CLONE_VAR_PRO( proo, machine_type_)
		CLONE_VAR_PRO( proo, machine_id1_)
		CLONE_VAR_PRO( proo, machine_id2_)
		CLONE_VAR_PRO( proo, ctrl_)
		CLONE_VAR_PRO( proo, transfer_)
		CLONE_VAR_PRO( proo, transfer_net_ip_)
		CLONE_VAR_PRO( proo, transfer_net_port_)
	END_CLONE_PRO()

public:
	//1:rgs 2:lgs 3:cts 4:dpx 5:css 6:gts
	S_INT_8		machine_type_;
	//when machine_type = 2 machine_id1_ = lgs sn
	//when machine_type = 3, 4, 5, 6 machine_id1_ = svrteam sn
	S_INT_32	machine_id1_;
	//when machine_type = 3, 4, 5, 6 machine_id2_ = sn
	S_INT_32	machine_id2_;
	//1:start 0:stop
	S_INT_8		ctrl_;
	//1:net 2:local file 4:all
	S_INT_8		transfer_;
	StaticString<24>	transfer_net_ip_;
	S_INT_32	transfer_net_port_;
};

PROTOCOL_NAMESPACE_END

#endif	//__PRO_CONTROLLER__H__
