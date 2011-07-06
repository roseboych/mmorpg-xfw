/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTS_Service.h"

#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "CTSModule.h"

void CTS_Service::from_gts( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &CTSSvr::gts_userlost_ntf, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVERPROXY_ACK)
	{
		fun =boost::bind( &CTSSvr::gts_team_selproxyack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVERCONFIRM_ACK)
	{
		fun =boost::bind( &CTSSvr::gts_team_confirmack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVER_REQ)
	{
		fun =boost::bind( &CTSSvr::gts_quitteam_req, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRLIST_REQ)
	{
		fun =boost::bind( &CTSSvr::gts_chrlist_req, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRCREATE_REQ)
	{
		fun =boost::bind( &CTSSvr::gts_chradd_req, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRSEL_REQ)
	{
		fun =boost::bind( &CTSSvr::gts_chrsel_req, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRDEL_REQ)
	{
		fun =boost::bind( &CTSSvr::gts_chrdel_req, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_DISCONNKEEP_NTF)
	{
		fun =boost::bind( &CTSSvr::gts_lnkkeep_ntf, CTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		CTSMODULE->regist_netcmd( pcmd);
	}
}

void CTS_Service::from_css( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == AOI_TELEPORT_ACK)
	{
		fun =boost::bind( &CTSSvr::css_teleport_ack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_INSTUPDATE_NTF)
	{
		fun =boost::bind( &CTSSvr::css_instupdate_ntf, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == AOI_ENTERINS_REQ)
	{
		fun =boost::bind( &CTSSvr::css_enterinst_req, CTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		CTSMODULE->regist_netcmd( pcmd);
	}
}

void CTS_Service::from_dpx( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == GAME_CHRLIST_ACK)
	{
		fun =boost::bind( &CTSSvr::dpx_chrlist_ack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRCREATE_ACK)
	{
		fun =boost::bind( &CTSSvr::dpx_chradd_ack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRSEL_ACK)
	{
		fun =boost::bind( &CTSSvr::dpx_chrsel_ack, CTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRDEL_ACK)
	{
		fun =boost::bind( &CTSSvr::dpx_chrdel_ack, CTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		CTSMODULE->regist_netcmd( pcmd);
	}
}
