/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CTS_Service.h"

#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_sys_socket.h>
#include <ace/Message_Block.h>
#include <ace/Auto_Ptr.h>

#include <memory>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <corelib/net/IOCPMutexAllocator.h>
#include <reslib/deploy/GlobalConfig.h>

#include "CSSModule.h"

#ifdef CORELIB_WIN32

CTS_Service::CTS_Service()
:inherit( IOCP_MutexApplocator, 0),
server_index_( NO_INITVALUE),
server_link_(0)
{
}

CTS_Service::CTS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
server_index_( NO_INITVALUE),
server_link_(0)
{
}

void CTS_Service::act (const void *pact)
{
	server_link_ =reinterpret_cast<CTSLink*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
}

#else

CTS_Service::CTS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
server_index_( NO_INITVALUE),
server_link_(0)
{
}

void CTS_Service::set_act( const void *pact)
{
	server_link_ =reinterpret_cast<CTSLink*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
}

#endif

CTS_Service::~CTS_Service(void)
{
}

void CTS_Service::conn_error()
{
	inherit::conn_error();
}

int CTS_Service::get_serviceimpl()
{
	return (int)CSSAPP_SERVICE_CTS;
}

void CTS_Service::begin_registservice_2()
{
	//等待回复
	server_link_->set_status( SVRREGIST_ST_WAITACK);

	USE_PROTOCOL_NAMESPACE;

	Pro_SvrRegist_req* req =PROTOCOL_NEW Pro_SvrRegist_req();
	req->svr_type_ =NETSVR_CSS;
	req->regionsn_ =GLOBALCONFIG_INS->get_regionsn();
	req->appsn_ =GLOBALCONFIG_INS->get_appsn();

	this->append_write_msg( req);
}

bool CTS_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE;
	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);

	if( !initialized()) return false;

	NETCMD_FUN_MAP fun =0;

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
			SystemCommand<CTS_Service>* pcmd =
				TASKCMD_NEW SystemCommand<CTS_Service>( boost::bind( &BasicModule::fin_registservice, CSSMODULE, this));

			CSSMODULE->regist_syscmd( pcmd);
		}
	}
	else if( pro->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &CSSSvr::controller_profiler_filterctrl, CSSMODULE, _1, _2);
	}
	else if( pro->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &CSSSvr::controller_profiler_analysisctrl, CSSMODULE, _1, _2);
	}
	else
	{
		fun =boost::bind( &CSSSvr::cts_netadapter, CSSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( a_ps.release(), fun, true);
		CSSMODULE->regist_netcmd( pcmd);
	}

	return true;
}
