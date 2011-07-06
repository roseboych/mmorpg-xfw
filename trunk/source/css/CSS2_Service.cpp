/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSS2_Service.h"

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

CSS2_Service::CSS2_Service()
:inherit(),
server_link_(0)
{
	type_ = NETSVR_CSS;
}

CSS2_Service::CSS2_Service( ACE_Proactor *proactor)
:inherit( proactor),
server_link_(0)
{
	type_ = NETSVR_CSS;
}

void CSS2_Service::act (const void *pact)
{
	server_link_ =reinterpret_cast<CSS2Link*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
}

#else

CSS2_Service::CSS2_Service( ACE_Reactor *reactor)
:inherit( reactor),
server_link_(0)
{
	type_ = NETSVR_CSS;
}

void CSS2_Service::set_act( const void *pact)
{
	server_link_ =reinterpret_cast<CSS2Link*>(const_cast<void*>(pact));
	this->server_index_ =server_link_->get_serverindex();
}

#endif

CSS2_Service::~CSS2_Service(void)
{
}

int CSS2_Service::get_serviceimpl()
{
	return (int)CSSAPP_SERVICE_CSS2;
}

void CSS2_Service::begin_registservice_2()
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

bool CSS2_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE;

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
			SystemCommand<CSS2_Service>* pcmd =
				TASKCMD_NEW SystemCommand<CSS2_Service>( boost::bind( &BasicModule::fin_registservice, CSSMODULE, this));

			CSSMODULE->regist_syscmd( pcmd);
		}
	}
	else
		inherit::recv_msg( a_ps.release());

	return true;
}