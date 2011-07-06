/**
* dpx application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DPX_Service.h"

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
#include <reslib/deploy/CSSOption.h>

#include "DPXModule.h"

#ifdef CORELIB_WIN32

DPX_Service::DPX_Service()
:inherit( IOCP_MutexApplocator, 0),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

DPX_Service::DPX_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#else

DPX_Service::DPX_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#endif

DPX_Service::~DPX_Service(void)
{
}

void DPX_Service::conn_error()
{
	inherit::conn_error();
}

int DPX_Service::get_serviceimpl()
{
	return (int)DPXAPP_SERVICE_DPX;
}

bool DPX_Service::recv_msg( BasicProtocol* pro)
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
		if( req->svr_type_ != NETSVR_CSS)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		bool findit =true;

		this->type_ =(NETSVR_TYPE_ENUM)req->svr_type_;
		this->svrsn_ =req->regionsn_;
		this->sn_ =req->appsn_;

		if( type_ == NETSVR_CSS)
		{
			SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( svrsn_);
			if( st == 0)
				findit =false;
			else
			{
				CSSOption* opt =st->get_cssoption( sn_);
				if( opt)
					this->server_index_ =opt->server_index_;
				else
					findit =false;
			}
		}
		else
			findit =false;

		if( !findit)
		{
			MODULE_LOG_INFO( MODULE_BOOT, "server:[%d][%d][%d] regist to me, err server type", req->svr_type_, req->regionsn_, req->appsn_);
			this->close_service();
			return true;
		}

		SystemCommand<DPX_Service>* pcmd =
			TASKCMD_NEW SystemCommand<DPX_Service>( boost::bind( &BasicModule::fin_registservice, DPXMODULE, this));
		DPXMODULE->regist_syscmd( pcmd);
	}
	else
	{
		if( type_ == NETSVR_CSS)
			from_css( a_ps.release());
	}

	return true;
}
