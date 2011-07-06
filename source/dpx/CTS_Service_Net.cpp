/**
* dpx application
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

#include "DPXModule.h"

void CTS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;

	if( p->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &DPXSvr::controller_profiler_filterctrl, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &DPXSvr::controller_profiler_analysisctrl, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &DPXSvr::cts_userlost_ntf, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_USERREGIST_NTF)
	{
		fun =boost::bind( &DPXSvr::cts_userregist_ntf, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRLIST_REQ)
	{
		fun =boost::bind( &DPXSvr::cts_chrlist_req, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRCREATE_REQ)
	{
		fun =boost::bind( &DPXSvr::cts_chradd_req, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRSEL_REQ)
	{
		fun =boost::bind( &DPXSvr::cts_chrsel_req, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRDEL_REQ)
	{
		fun =boost::bind( &DPXSvr::cts_chrdel_req, DPXMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		DPXMODULE->regist_netcmd( pcmd);
	}
}
