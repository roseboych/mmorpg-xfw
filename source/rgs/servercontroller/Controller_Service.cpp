/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "Controller_Service.h"

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

#include "../RGSModule.h"
#include "ControllerLink.h"


#ifdef CORELIB_WIN32

Controller_Service::Controller_Service()
:inherit( IOCP_MutexApplocator, 0),
type_( NETSVR_NONE)
{
}

Controller_Service::Controller_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor),
type_( NETSVR_NONE)
{
}

#else

Controller_Service::Controller_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor),
type_( NETSVR_NONE)
{
}

#endif

Controller_Service::~Controller_Service(void)
{
}

int Controller_Service::get_serviceimpl()
{
	return (int)RGSAPP_SERVICE_CONTROLLER;
}

#ifdef CORELIB_WIN32

void Controller_Service::open( ACE_HANDLE new_handle, ACE_Message_Block &message_block)
{
	this->handle( new_handle);

	if( this->reader_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0 ||
		this->writer_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "Controller_Service open connect socket failed");

		ControllerLink* pLink =RGSMODULE->get_controllerlink();
		pLink->reset();
		
		return;
	}
	else
	{
		set_initalized();

		if( reader_.read( read_message_, read_message_.space()) != 0)
		{
			this->conn_error();
			read_message_.reset();
		}
	}
}

#else

int Controller_Service::open()
{
	set_initalized();

	return reactor()->register_handler( this, ACE_Event_Handler::RWE_MASK);
}

#endif	//CORELIB_WIN32

void Controller_Service::conn_error()
{
#ifdef CORELIB_WIN32
#else
	reactor()->remove_handler( this, ACE_Event_Handler::RWE_MASK|ACE_Event_Handler::DONT_CALL);
#endif

	close_service();

	SystemCommand<void> *pcmd =
		TASKCMD_NEW SystemCommand<void>( boost::bind( &RGSModule::svr_controller_disconnect, RGSMODULE));

	RGSMODULE->regist_syscmd( pcmd);
}

bool Controller_Service::recv_msg( BasicProtocol* pro)
{
	USE_PROTOCOL_NAMESPACE

	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);

	if( !initialized()) return false;

	NETCMD_FUN_MAP2 fun =0;

	if( pro->iid_ == CONTROLLER_LOGIN_REQ)
	{
		fun =boost::bind( &RGSSvr::controller_login_req, RGSMODULE, _1, _2);
	}
	else if( pro->iid_ == CONTROLLER_PROFILER_FILTERCTRL)
	{
		fun =boost::bind( &RGSSvr::controller_profiler_filterctrl, RGSMODULE, _1, _2);
	}
	else if( pro->iid_ == CONTROLLER_PROFILER_ANALYSISCTRL)
	{
		fun =boost::bind( &RGSSvr::controller_profiler_analysisctrl, RGSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( a_ps.release(), fun, true);
		RGSMODULE->regist_netcmd( pcmd);
	}

	return true;
}
