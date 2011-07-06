/**
* rgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGS_Service.h"

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_sys_socket.h>
#include <ace/Message_Block.h>
#include <ace/Auto_Ptr.h>

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <corelib/net/IOCPMutexAllocator.h>
#include <corelib/core/BasicCommand.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/LGSOption.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CTSOption.h>

#include "RGSModule.h"

#ifdef CORELIB_WIN32

RGS_Service::RGS_Service()
:inherit( IOCP_MutexApplocator, 0),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

RGS_Service::RGS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#else

RGS_Service::RGS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#endif

RGS_Service::~RGS_Service(void)
{
}

void RGS_Service::conn_error()
{
	inherit::conn_error();
}

int RGS_Service::get_serviceimpl()
{
	return (int)RGSAPP_SERVICE_RGS;
}

bool RGS_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE

	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);

	if( !initialized()) return false;

	if( pro->iid_ == SVR_REGIST_REQ)
	{
		//服务器连接注册
		Pro_SvrRegist_req* req =dynamic_cast<Pro_SvrRegist_req*>(pro);

		//注册的服务器类型错误
		if( req->svr_type_ != NETSVR_LGS && req->svr_type_ != NETSVR_CTS)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		bool findit =true;

		this->type_ =(NETSVR_TYPE_ENUM)req->svr_type_;
		this->svrsn_ =req->regionsn_;
		this->sn_ =req->appsn_;

		if( type_ == NETSVR_LGS)
		{
			LGSOption* opt =GLOBALCONFIG_INS->get_lgsoption( sn_);
			if( opt)
				this->server_index_ =opt->server_index_;
			else
				findit =false;
		}
		else
		{
			SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( svrsn_);
			if( st)
				this->server_index_ =st->server_index_;
			else
				findit =false;
		}

		if( !findit)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		SystemCommand<RGS_Service>* pcmd =
			TASKCMD_NEW SystemCommand<RGS_Service>( boost::bind( &BasicModule::fin_registservice, RGSMODULE, this));
		RGSMODULE->regist_syscmd( pcmd);
	}
	else
	{
		if( type_ == NETSVR_LGS)
			lgs_protocol( a_ps.release());
		else if( type_ == NETSVR_CTS)
			cts_protocol( a_ps.release());
	}

	return true;
}

void RGS_Service::lgs_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP2 fun =0;
	if( p->iid_ == GAME_LOGIN_REQ)
	{
		fun =boost::bind( &RGSSvr::lgs_login_req, RGSMODULE, _1, _2, server_index_);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &RGSSvr::lgs_userlost_ntf, RGSMODULE, _1, _2, server_index_);
	}
	else if( p->iid_ == SVR_SELSERVER_REQ)
	{
		fun =boost::bind( &RGSSvr::lgs_selteam_req, RGSMODULE, _1, _2, server_index_);
	}
	else if( p->iid_ == SVR_QUITSERVERRECONN_REQ)
	{
		fun =boost::bind( &RGSSvr::lgs_quitteamreconn_req, RGSMODULE, _1, _2, server_index_);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		RGSMODULE->regist_netcmd( pcmd);
	}
}

void RGS_Service::cts_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == SVR_SVRTEAMINFO_NTF)
	{
		fun =boost::bind( &RGSSvr::svr_teaminfo_ntf, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &RGSSvr::cts_userlost_ntf, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELTEAMTIMEOUT_NTF)
	{
		fun =boost::bind( &RGSSvr::cts_teamtimeout_ntf, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVER_ACK)
	{
		fun =boost::bind( &RGSSvr::cts_selteam_ack, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVERCONFIRM_ACK)
	{
		fun =boost::bind( &RGSSvr::cts_selteamconfirm_ack, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVER_REQ)
	{
		fun =boost::bind( &RGSSvr::cts_quitteam_req, RGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_DISCONNKEEP_NTF)
	{
		fun =boost::bind( &RGSSvr::cts_lnkkeep_ntf, RGSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		RGSMODULE->regist_netcmd( pcmd);
	}
}
