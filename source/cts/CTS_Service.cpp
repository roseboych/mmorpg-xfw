/**
* cts application
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

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <corelib/net/IOCPMutexAllocator.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/DPXOption.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/GTSOption.h>

#include "CTSModule.h"

#ifdef CORELIB_WIN32

CTS_Service::CTS_Service()
:inherit( IOCP_MutexApplocator, 0),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

CTS_Service::CTS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#else

CTS_Service::CTS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
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
	return (int)CTSAPP_SERVICE_CTS;
}

bool CTS_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE;
	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);

	if( !initialized()) return false;

	if( pro->iid_ == SVR_REGIST_REQ)
	{
		Pro_SvrRegist_req* req =dynamic_cast<Pro_SvrRegist_req*>(pro);

		//注册的服务器类型错误
		if( req->svr_type_ != NETSVR_DPX && req->svr_type_ != NETSVR_CSS && req->svr_type_ != NETSVR_GTS)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		bool findit =true;

		this->type_ =(NETSVR_TYPE_ENUM)req->svr_type_;
		this->svrsn_ =req->regionsn_;
		this->sn_ =req->appsn_;

		SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( svrsn_);
		if( st == 0)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		if( type_ == NETSVR_CSS)
		{
			CSSOption* opt =st->get_cssoption( svrsn_);
			if( opt)
				this->server_index_ =st->server_index_;
			else
				findit =false;
		}
		else if( type_ == NETSVR_GTS)
		{
			GTSOption* opt =st->get_gtsoption( svrsn_);
			if( opt)
				this->server_index_ =st->server_index_;
			else
				findit =false;
		}
		else if( type_ != NETSVR_DPX)
			findit =false;

		if( !findit)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		SystemCommand<RGS_Service>* pcmd =
			TASKCMD_NEW SystemCommand<RGS_Service>( boost::bind( &BasicModule::fin_registservice, CTSMODULE, this));
		CTSMODULE->regist_syscmd( pcmd);
	}
	else
	{
		if( type_ == NETSVR_GTS)
			from_gts( a_ps.release());
		else if( type_ == NETSVR_DPX)
			from_dpx( a_ps.release());
		else if( type_ == NETSVR_CSS)
			from_css( a_ps.release());
	}

	return true;
}
