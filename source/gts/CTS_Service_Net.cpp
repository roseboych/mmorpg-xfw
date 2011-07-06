/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTS_Service.h"

#include <ace/Auto_Ptr.h>

#include <memory>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "GTSModule.h"

void CTS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;

	if( p->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &GTSSvr::controller_profiler_filterctrl, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &GTSSvr::controller_profiler_analysisctrl, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &GTSSvr::cts_userlost_ntf, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELTEAMTIMEOUT_NTF)
	{
		fun =boost::bind( &GTSSvr::cts_teamtimeout_ntf, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVERPROXY_REQ)
	{
		fun =boost::bind( &GTSSvr::cts_teamproxy_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVER_ACK)
	{
		fun =boost::bind( &GTSSvr::cts_quitteam_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRLIST_ACK)
	{
		fun =boost::bind( &GTSSvr::cts_chrlist_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRCREATE_ACK)
	{
		fun =boost::bind( &GTSSvr::cts_chradd_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRSEL_ACK)
	{
		fun =boost::bind( &GTSSvr::cts_chrsel_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRDEL_ACK)
	{
		fun =boost::bind( &GTSSvr::cts_chrdel_ack, GTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		GTSMODULE->regist_netcmd( pcmd);
	}
}
