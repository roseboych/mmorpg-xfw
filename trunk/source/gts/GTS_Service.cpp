/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTS_Service.h"

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

#include "GTSModule.h"
#include "player/Player.h"
#include "player/Player_Qos.h"

#ifdef CORELIB_WIN32

GTS_Service::GTS_Service()
:inherit( IOCP_MutexApplocator, 0, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

GTS_Service::GTS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

void GTS_Service::open( ACE_HANDLE new_handle, ACE_Message_Block &message_block)
{
	this->handle( new_handle);

	if( this->reader_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0 ||
		this->writer_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0)
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "open connect socket failed");
		GTSMODULE->release_player( this->player_);

		return;
	}
	else
	{
		//设置连接状态
		set_initalized();

		is_connect_ =true;

		//开始接收数据
		if( reader_.read( read_message_, read_message_.space()) != 0)
		{
			this->conn_error();
			read_message_.reset();
		}
	}
}

#else

GTS_Service::GTS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

int GTS_Service::open()
{
	//设置连接状态
	set_initalized();

	is_connect_ =true;

	return reactor()->register_handler( this, ACE_Event_Handler::RWE_MASK);
}

#endif

GTS_Service::~GTS_Service(void)
{
}

void GTS_Service::conn_error()
{
#ifdef CORELIB_WIN32
#else
	reactor()->remove_handler( this, ACE_Event_Handler::RWE_MASK|ACE_Event_Handler::DONT_CALL);
#endif

	close_service();

	SystemCommand<GTS_Service>* pcmd =
		TASKCMD_NEW SystemCommand<GTS_Service>( boost::bind( &GTSSvr::player_disconnected, GTSMODULE, this));
	GTSMODULE->regist_syscmd( pcmd);
}

int GTS_Service::get_serviceimpl()
{
	return (int)GTSAPP_SERVICE_GTS;
}

void GTS_Service::reset()
{
	is_connect_ =false;

	release_queue();
	close_service();
}

void GTS_Service::online_tickit( ACE_UINT64& now)
{
	if( !is_connect_)
		return;

	//ACE_Guard<ACE_Thread_Mutex> mon( *write_mutex_);

	//1m心跳检查
	if( qos_->last_time() + 1000*60 > now)
		return;

	USE_PROTOCOL_NAMESPACE;
	Pro_SvrOnlineTick_NTF *ack =PROTOCOL_NEW Pro_SvrOnlineTick_NTF();
	this->append_write_msg( ack);
}

bool GTS_Service::recv_msg( BasicProtocol* protocol)
{
	//MODULE_LOG_DEBUG((MODULE_TEMP, "receive protocol id:%d", protocol->iid_));
	ACE_Auto_Ptr<BasicProtocol> a_ps( protocol);

	if( !initialized()) return false;

	PRO_UUID_FILL( protocol, player_->global_index_, player_->uuid_)
	assort_protocol( a_ps.release());

	return true;
}
