/**
* protocol module
*
* 定义了服务器组管理类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/Pro_controller.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_CtrlLogin_req--------------------//
Pro_CtrlLogin_req::Pro_CtrlLogin_req()
:inherit( CONTROLLER_LOGIN_REQ)
{
}

//-----------------------------Pro_CtrlLogin_ack--------------------//
Pro_CtrlLogin_ack::Pro_CtrlLogin_ack()
:inherit( CONTROLLER_LOGIN_ACK)
{
}

//-----------------------------Pro_CtrlProfilerFilterCtrl--------------------//
Pro_CtrlProfilerFilterCtrl::Pro_CtrlProfilerFilterCtrl()
:inherit( CONTROLLER_PROFILER_FILTERCTRL)
{
}

//-----------------------------Pro_CtrlProfilerAnalysisCtrl--------------------//
Pro_CtrlProfilerAnalysisCtrl::Pro_CtrlProfilerAnalysisCtrl()
:inherit( CONTROLLER_PROFILER_ANALYSISCTRL)
{
}

PROTOCOL_NAMESPACE_END
