/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSS_Service.h"

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
#include <reslib/deploy/GTSOption.h>

#include "CSSModule.h"

#ifdef CORELIB_WIN32

CSS_Service::CSS_Service()
:inherit( IOCP_MutexApplocator, 0),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

CSS_Service::CSS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#else

CSS_Service::CSS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
type_( NETSVR_NONE),
server_index_( NO_INITVALUE)
{
}

#endif

CSS_Service::~CSS_Service(void)
{
}

void CSS_Service::conn_error()
{
	inherit::conn_error();
}

int CSS_Service::get_serviceimpl()
{
	return (int)CSSAPP_SERVICE_CSS;
}

bool CSS_Service::recv_msg( BasicProtocol* pro)
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
		if( req->svr_type_ != NETSVR_GTS && req->svr_type_ != NETSVR_CSS)
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
		else if( type_ == NETSVR_GTS)
		{
			SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( svrsn_);
			if( st == 0)
				findit =false;
			else
			{
				GTSOption* opt =st->get_gtsoption( sn_);
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

		SystemCommand<CSS_Service>* pcmd =
			TASKCMD_NEW SystemCommand<CSS_Service>( boost::bind( &BasicModule::fin_registservice, CSSMODULE, this));
		CSSMODULE->regist_syscmd( pcmd);
	}
	else
	{
		NETCMD_FUN_MAP fun =0;
		if( type_ == NETSVR_CSS)
			fun =boost::bind( &CSSSvr::css_netadapter, CSSMODULE, _1, _2);
		else if( type_ == NETSVR_GTS)
			fun =boost::bind( &CSSSvr::gts_netadapter, CSSMODULE, _1, _2);

		if( fun)
		{
			NetCommand* pcmd =TASKCMD_NEW NetCommand( a_ps.release(), fun, true);
			CSSMODULE->regist_netcmd( pcmd);
		}
	}

	return true;
}
