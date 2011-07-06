/**
* protocol module
*
* 定义了服务器组管理类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SERVER_CONTROLLER__H__
#define __SERVER_CONTROLLER__H__

#define CONTROLLER_PRO_BASE 		1000

enum PRO_CONTROLLER_ENUM
{
	CONTROLLER_INIT =CONTROLLER_PRO_BASE,
	//管理员登陆认证
	CONTROLLER_LOGIN_REQ,
	CONTROLLER_LOGIN_ACK,

	//性能分析参数设置
	CONTROLLER_PROFILER_FILTERCTRL,
	//性能分析开关控制
	CONTROLLER_PROFILER_ANALYSISCTRL,
};

#endif	//__SERVER_CONTROLLER__H__
