/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGS_Service.h"

#include <ace/Auto_Ptr.h>

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include "CTSModule.h"

void RGS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;

	if( p->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &CTSSvr::controller_profiler_filterctrl, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &CTSSvr::controller_profiler_analysisctrl, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &CTSSvr::rgs_userlost_ntf, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVER_REQ)
	{
		fun =boost::bind( &CTSSvr::rgs_team_selreq, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVER_ACK)
	{
		fun =boost::bind( &CTSSvr::rgs_quitteam_ack, CTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		CTSMODULE->regist_netcmd( pcmd);
	}
}
