/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGS_Service.h"

#include <ace/OS_NS_unistd.h>
#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <corelib/net/IOCPMutexAllocator.h>

#include "LGSModule.h"
#include "Player.h"
#include "Player_Qos.h"

#ifdef CORELIB_WIN32

LGS_Service::LGS_Service()
:inherit( IOCP_MutexApplocator, 0, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

LGS_Service::LGS_Service( ACE_Proactor *proactor)
:inherit( IOCP_MutexApplocator, proactor, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

void LGS_Service::open( ACE_HANDLE new_handle, ACE_Message_Block &message_block)
{
	this->handle( new_handle);

	if( this->reader_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0 ||
		this->writer_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0)
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "open connect socket failed");
		LGSMODULE->release_player( this->player_);

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

LGS_Service::LGS_Service( ACE_Reactor *reactor)
:inherit( IOCP_MutexApplocator, reactor, 0, 0, false),
server_index_( NO_INITVALUE),
player_( 0),
is_connect_( false)
{
	this->ext_data_ =&encrypt_;
	this->set_qos( FRAMEWK_NEW Player_Qos(), true);
}

int LGS_Service::open()
{
	//设置连接状态
	set_initalized();

	is_connect_ =true;

	return reactor()->register_handler( this, ACE_Event_Handler::RWE_MASK);
}

#endif

LGS_Service::~LGS_Service(void)
{
}

void LGS_Service::conn_error()
{
#ifdef CORELIB_WIN32
#else
	reactor()->remove_handler( this, ACE_Event_Handler::RWE_MASK|ACE_Event_Handler::DONT_CALL);
#endif

	close_service();

	SystemCommand<LGS_Service>* pcmd =
		TASKCMD_NEW SystemCommand<LGS_Service>( boost::bind( &LGSSvr::player_disconnected, LGSMODULE, this));
	LGSMODULE->regist_syscmd( pcmd);
}

int LGS_Service::get_serviceimpl()
{
	return (int)LGSAPP_SERVICE_LGS;
}

void LGS_Service::reset()
{
	is_connect_ =false;

	release_queue();
	close_service();
}

void LGS_Service::online_tickit( ACE_UINT64& now)
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

bool LGS_Service::recv_msg( BasicProtocol* pro)
{
	ACE_Auto_Ptr<BasicProtocol> a_ps( pro);
	
	if( !initialized()) return false;

	assort_protocol( a_ps.release());

	return true;
}

