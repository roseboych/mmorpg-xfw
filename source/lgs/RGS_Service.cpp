/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "RGS_Service.h"

#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_sys_socket.h>
#include <ace/Message_Block.h>
#include <ace/Auto_Ptr.h>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <corelib/net/IOCPMutexAllocator.h>
#include <reslib/deploy/GlobalConfig.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include "LGSModule.h"
#include "RGSLink.h"

#ifdef CORELIB_WIN32

RGS_Service::RGS_Service()
:inherit( IOCP_MutexApplocator, 0),
server_index_( NO_INITVALUE),
server_link_(0)
{
}

RGS_Service::RGS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
server_index_( NO_INITVALUE),
server_link_( 0)
{
}

void RGS_Service::act (const void *pact)
{
	server_link_ =reinterpret_cast<RGSLink*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
}

#else

RGS_Service::RGS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
server_index_( NO_INITVALUE),
server_link_( 0)
{
}

void RGS_Service::set_act( const void *pact)
{
	server_link_ =reinterpret_cast<RGSLink*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
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
	return (int)LGSAPP_SERVICE_RGS;
}

void RGS_Service::begin_registservice_2()
{
	//等待回复
	server_link_->set_status( SVRREGIST_ST_WAITACK);

	USE_PROTOCOL_NAMESPACE;

	Pro_SvrRegist_req* req =PROTOCOL_NEW Pro_SvrRegist_req();
	req->svr_type_ =NETSVR_LGS;
	req->regionsn_ =GLOBALCONFIG_INS->get_regionsn();
	req->appsn_ =GLOBALCONFIG_INS->get_appsn();

	this->append_write_msg( req);
}

bool RGS_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE;
	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);

	if( !initialized()) return false;

	if( pro->iid_ == SVR_REGIST_ACK)
	{
		//注册回复
		Pro_SvrRegist_ack* pack =dynamic_cast<Pro_SvrRegist_ack*>(pro);
		if( pack->result_ == 1)
		{
			this->close_service();
		}
		else
		{
			SystemCommand<RGS_Service>* pcmd =
				TASKCMD_NEW SystemCommand<RGS_Service>( boost::bind( &BasicModule::fin_registservice, LGSMODULE, this));

			LGSMODULE->regist_syscmd( pcmd);
		}
	}
	else 
		assort_protocol( a_ps.release());

	return true;
}

void RGS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &LGSSvr::controller_profiler_filterctrl, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &LGSSvr::controller_profiler_analysisctrl, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SVRTEAMINFO_NTF)
	{
		fun =boost::bind( &LGSSvr::rgs_teaminfo_ntf, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_USERLOST_NTF)
	{
		fun =boost::bind( &LGSSvr::rgs_userlost_ntf, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_LOGIN_ACK)
	{
		fun =boost::bind( &LGSSvr::rgs_user_loginack, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELTEAMTIMEOUT_NTF)
	{
		fun =boost::bind( &LGSSvr::rgs_teamtimeout_ntf, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVER_ACK)
	{
		fun =boost::bind( &LGSSvr::rgs_team_selack, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVERCONFIRM_ACK)
	{
		fun =boost::bind( &LGSSvr::rgs_team_confirmack, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVERRECONN_ACK)
	{
		fun =boost::bind( &LGSSvr::rgs_quitteamreconn_ack, LGSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		LGSMODULE->regist_netcmd( pcmd);
	}
}
