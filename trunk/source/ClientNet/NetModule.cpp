/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "NetModule.h"

#include "BaseTestCase.h"
#include <process.h>

#include <algorithm>

NetModule::NetModule():
connevt_fun_( 0),
disconn_fun_( 0),
netcmd_fun_( 0),
testcase_( 0),
lgs_socket_( INVALID_SOCKET),
lgs_data_len_( 0),
gts_socket_( INVALID_SOCKET),
gts_data_len_( 0),
thread_handle_lgs_( 0),
thread_handle_gts_( 0)
{
	::InitializeCriticalSection( &lock_);
	::InitializeCriticalSection( &lock2_);

	pro_factory_ =new ProtocolFactory();
}

NetModule::~NetModule()
{
	::DeleteCriticalSection( &lock_);
	::DeleteCriticalSection( &lock2_);

	delete pro_factory_;
	pro_factory_ =0;
}

void NetModule::release()
{
	{
		MutexGuard guard( &lock_);

		for( PROTOCOL_LIST::iterator iter =lgs_send_pros_.begin(); iter != lgs_send_pros_.end(); ++iter)
			delete (*iter);
		lgs_send_pros_.clear();

		for( PROTOCOL_LIST::iterator iter =gts_send_pros_.begin(); iter != gts_send_pros_.end(); ++iter)
			delete (*iter);
		gts_send_pros_.clear();
	}

	{
		MutexGuard guard( &lock2_);

		for( COMMAND_LIST::iterator iter =sys_cmds_.begin(); iter != sys_cmds_.end(); ++iter)
			delete (*iter);
		sys_cmds_.clear();

		for( COMMAND_LIST::iterator iter =net_cmds_.begin(); iter != net_cmds_.end(); ++iter)
			delete (*iter);
		net_cmds_.clear();
	}

	if( testcase_)
		delete testcase_;
	testcase_ =0;
}

bool NetModule::init_netmodule(FUN_NETCONNECT_CALLBACK f1, FUN_NETDISCONN_CALLBACK disfun, FUN_NETCOMMAND_CALLBACK netfun, 
								bool offline, const char* testcase)
{
	//初始化socket环境
	WORD wVersionRequested = MAKEWORD( 2, 2 );
	WSADATA wsaData;

	int err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
		return false;

	this->connevt_fun_ =f1;
	this->disconn_fun_ =disfun;
	this->netcmd_fun_ =netfun;
	this->is_offline_ =offline;
	this->casename_ =testcase;

	if( is_offline_)
		testcase_ =BaseTestCase::new_testcase( casename_);

	return true;
}

void NetModule::uninit_netmodule()
{
	release();

	//释放socket环境
	WSACleanup();
}

void NetModule::send_lgs_protocol( BasicProtocol* pro)
{
	if( this->is_offline_)
	{
		if( pro && testcase_)
			testcase_->deal_echo( pro);

		if( pro)
			delete pro;
	}
	else
	{
		MutexGuard guard( &lock_);

		lgs_send_pros_.push_back( pro);
	}
}

void NetModule::send_gts_protocol( BasicProtocol* pro)
{
	if( this->is_offline_)
	{
		if( pro && testcase_)
			testcase_->deal_echo( pro);

		if( pro)
			delete pro;
	}
	else
	{
		MutexGuard guard( &lock_);

		gts_send_pros_.push_back( pro);
	}
}

void NetModule::offline_sendtoclient( BasicProtocol* pro)
{
	if( netcmd_fun_)
	{
		NetCommand* pcmd =new NetCommand( this->netcmd_fun_, pro);

		this->push_netcmd( pcmd);
	}
	else
	{
		if( pro)
			delete pro;
	}
}

void NetModule::thread_proc_lgs( void* pdata)
{
	NetModule* pThis =( NetModule*)pdata;

	while( pThis->exit_thread_lgs_ == false)
	{
		::Sleep( 1);

		pThis->recv_lgs_protocol();

		pThis->send_lgs_protocol();
	}

	::_endthread();

	pThis->thread_handle_lgs_ =0;
}

void NetModule::thread_proc_gts( void* pdata)
{
	NetModule* pThis =( NetModule*)pdata;

	while( pThis->exit_thread_gts_ == false)
	{
		::Sleep( 1);

		pThis->recv_gts_protocol();
		pThis->send_gts_protocol();
	}

	::_endthread();

	pThis->thread_handle_gts_ =0;
}

void NetModule::connect_lgs( const char* ip, int port, const char* uname, const char* pwd)
{
	this->ip_ =ip;
	this->port_ =port;
	this->username_ =uname;
	this->password_ =pwd;

	if( is_offline_)
	{
		PRO::Pro_Login_req* req =new PRO::Pro_Login_req();
		NS_STL::string token( uname);
		token +=":";
		token +=pwd;
		req->token_ =token;

		this->send_lgs_protocol( req);

		return;
	}

	if( lgs_socket_ != INVALID_SOCKET || thread_handle_lgs_)
	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_HASREQCONN, GAMESERVER_LGS);
		this->push_syscmd( pcmd);

		return;
	}

	lgs_socket_ =::socket( AF_INET, SOCK_STREAM, 0);
	if( lgs_socket_ == INVALID_SOCKET)
	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTFAIL, GAMESERVER_LGS);
		this->push_syscmd( pcmd);

		return;
	}

	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTING, GAMESERVER_LGS);
		this->push_syscmd( pcmd);
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( (LPCTSTR)ip_.c_str());
	serv_addr.sin_port			= htons( port_);

	if( ::connect( lgs_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		while( ::closesocket( lgs_socket_) != 0){
			::Sleep( 1);
		}

		lgs_socket_ =INVALID_SOCKET;

		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTFAIL, GAMESERVER_LGS);
		this->push_syscmd( pcmd);

		return;
	}

	u_long ll =1;
	ioctlsocket( lgs_socket_, FIONBIO, &ll);

	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTED, GAMESERVER_LGS);
		this->push_syscmd( pcmd);
	}

	lgs_data_len_ =0;

	exit_thread_lgs_ =false;
	thread_handle_lgs_ =::_beginthread( NetModule::thread_proc_lgs, 0, (void*)this);

	PRO::Pro_Login_req* req =new PRO::Pro_Login_req();
	NS_STL::string token( username_);
	token += ":";
	token += password_;
	req->token_ =token;

	this->send_lgs_protocol( req);
}

void NetModule::disconnect_lgs()
{
	if( !is_connected_lgs())
		return;

	exit_thread_lgs_ =true;
	wait_socket_close( 0);

//	this->release();
}

void NetModule::disconnect_gts()
{
	if( !is_connected_gts())
		return;

	exit_thread_gts_ =true;
	wait_socket_close( 1);
}

void NetModule::connect_gts( const char* ip, int port, S_INT_32 server_index, S_TIMESTAMP token)
{
	this->gts_ip_ =ip;
	this->gts_port_ =port;
	this->server_index_ =server_index;
	this->token_ =token;

	if( is_offline_)
	{
		PRO::Pro_SvrSelTeamConfirm_req* req =new PRO::Pro_SvrSelTeamConfirm_req();
		req->proxy_index_ =server_index;
		req->token_ =token;

		this->send_gts_protocol( req);

		return;
	}

	if( gts_socket_ != INVALID_SOCKET || thread_handle_gts_)
	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_HASREQCONN, GAMESERVER_GTS);
		this->push_syscmd( pcmd);

		return;
	}

	gts_socket_ =::socket( AF_INET, SOCK_STREAM, 0);
	if( gts_socket_ == INVALID_SOCKET)
	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTFAIL, GAMESERVER_GTS);
		this->push_syscmd( pcmd);

		return;
	}

	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTING, GAMESERVER_GTS);
		this->push_syscmd( pcmd);
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( (LPCTSTR)gts_ip_.c_str());
	serv_addr.sin_port			= htons( gts_port_);

	if( ::connect( gts_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		while( ::closesocket( gts_socket_) != 0){
			::Sleep( 1);
		}

		gts_socket_ =INVALID_SOCKET;

		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTFAIL, GAMESERVER_GTS);
		this->push_syscmd( pcmd);

		return;
	}

	u_long ll =1;
	ioctlsocket( gts_socket_, FIONBIO, &ll);

	{
		ConnectEvtCommand* pcmd =new ConnectEvtCommand( connevt_fun_, CONNEVT_CONNECTED, GAMESERVER_GTS);
		this->push_syscmd( pcmd);
	}

	gts_data_len_ =0;

	exit_thread_gts_ =false;
	thread_handle_gts_ =::_beginthread( NetModule::thread_proc_gts, 0, (void*)this);

	PRO::Pro_SvrSelTeamConfirm_req* req =new PRO::Pro_SvrSelTeamConfirm_req();
	req->proxy_index_ =server_index;
	req->token_ =token;

	this->send_gts_protocol( req);
}

void NetModule::net_loop()
{
	if( is_offline_)
		offline_tick();

	COMMAND_LIST cmds;

	{
		MutexGuard guard( &lock2_);
		if( sys_cmds_.size() > 0)
		{
			NS_STL::copy( sys_cmds_.begin(), sys_cmds_.end(), NS_STL::back_inserter(cmds));
			sys_cmds_.clear();
		}

		if( net_cmds_.size() > 0)
		{
			NS_STL::copy( net_cmds_.begin(), net_cmds_.end(), NS_STL::back_inserter(cmds));
			net_cmds_.clear();
		}
	}

	//执行系统命令
	for( COMMAND_LIST::iterator iter =cmds.begin(); iter != cmds.end(); ++iter)
	{
		BasicCommand* pcmd =(*iter);
		pcmd->run();
		delete pcmd;
	}

	cmds.clear();
}

void NetModule::push_syscmd( BasicCommand* pcmd)
{
	if( pcmd == 0)
		return;

	MutexGuard guard( &lock2_);

	sys_cmds_.push_back( pcmd);
}

void NetModule::push_netcmd( BasicCommand* pcmd)
{
	if( pcmd == 0)
		return;

	MutexGuard guard( &lock2_);

	net_cmds_.push_back( pcmd);
}

void NetModule::offline_tick()
{
	if( testcase_ == 0)
		return;

	testcase_->case_tick();
}

void NetModule::send_lgs_protocol()
{
	if( !is_connected_lgs())
		return;

	BasicProtocol* p =0;

	{
		if( lgs_send_pros_.size() == 0)
			return;

		MutexGuard guard( &lock_);

		p =lgs_send_pros_.front();
		lgs_send_pros_.pop_front();
	}

	if( p == 0)
		return;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	//TODO:
	p->data( pbuf, SINGLE_PACK_LEN - sizeof( S_INT_32), len, 0, false);
	*((S_INT_32*)buf) =len;

	delete p;
	p =0;

	int totlen =len + sizeof( S_INT_32);
	int len2 =0;
	pbuf =buf;

	while( ( len2 =::send( lgs_socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			lgs_data_len_ =0;

			this->exit_thread_lgs_ =true;
			wait_socket_close( 0);

			DisConnectCommand* pcmd =new DisConnectCommand( this->disconn_fun_, GAMESERVER_LGS);
			this->push_syscmd( pcmd);
		}
	}
}

void NetModule::recv_lgs_protocol()
{
	if( !is_connected_lgs())
		return;

	char *pbuf =lgs_recv_buffer_+lgs_data_len_;
	int len =MAX_PACK_LEN - lgs_data_len_;

	int rlen =::recv( lgs_socket_, pbuf, len, 0);

	if( rlen == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			lgs_data_len_ =0;
			this->exit_thread_lgs_ =true;
			wait_socket_close( 0);

			DisConnectCommand* pcmd =new DisConnectCommand( this->disconn_fun_, GAMESERVER_LGS);
			this->push_syscmd( pcmd);
		}

		return;
	}

	lgs_data_len_ += rlen;

	while( 1)
	{
		if( lgs_data_len_ < sizeof( S_INT_32))
			break;
		pbuf =lgs_recv_buffer_;
		S_INT_32 dlen =*((S_INT_32*)pbuf);
		if( dlen > lgs_data_len_ - sizeof( S_INT_32))
			break;

		pbuf += sizeof( S_INT_32);
		//TODO:
		BasicProtocol *ret =pro_factory_->data_protocol( pbuf, dlen, 0, false);
		if( ret)
		{
			NetCommand* pcmd =new NetCommand( this->netcmd_fun_, ret);
			this->push_netcmd( pcmd);
		}

		lgs_data_len_ -= sizeof( S_INT_32);
		lgs_data_len_ -= dlen;
		pbuf += dlen;
		memmove( &(lgs_recv_buffer_[0]), pbuf, lgs_data_len_);
	}
}

void NetModule::send_gts_protocol()
{
	if( !is_connected_gts())
		return;

	BasicProtocol* p =0;

	{
		if( gts_send_pros_.size() == 0)
			return;

		MutexGuard guard( &lock_);

		p =gts_send_pros_.front();
		gts_send_pros_.pop_front();
	}

	if( p == 0)
		return;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	//TODO:
	p->data( pbuf, SINGLE_PACK_LEN - sizeof( S_INT_32), len, 0, false);
	*((S_INT_32*)buf) =len;

	delete p;
	p =0;

	int totlen =len + sizeof( S_INT_32);
	int len2 =0;
	pbuf =buf;

	while( ( len2 =::send( gts_socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			gts_data_len_ =0;

			this->exit_thread_gts_ =true;
			wait_socket_close(1);

			DisConnectCommand* pcmd =new DisConnectCommand( this->disconn_fun_, GAMESERVER_GTS);
			this->push_syscmd( pcmd);
		}
	}
}

void NetModule::recv_gts_protocol()
{
	if( !is_connected_gts())
		return;

	char *pbuf =gts_recv_buffer_+gts_data_len_;
	int len =MAX_PACK_LEN - gts_data_len_;

	int rlen =::recv( gts_socket_, pbuf, len, 0);

	if( rlen == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			gts_data_len_ =0;
			this->exit_thread_gts_ =true;
			wait_socket_close(1);

			DisConnectCommand* pcmd =new DisConnectCommand( this->disconn_fun_, GAMESERVER_GTS);
			this->push_syscmd( pcmd);
		}

		return;
	}

	gts_data_len_ += rlen;

	while( 1)
	{
		if( gts_data_len_ < sizeof( S_INT_32))
			break;
		pbuf =gts_recv_buffer_;
		S_INT_32 dlen =*((S_INT_32*)pbuf);
		if( dlen > gts_data_len_ - sizeof( S_INT_32))
			break;

		pbuf += sizeof( S_INT_32);
		//TODO:
		BasicProtocol *ret =pro_factory_->data_protocol( pbuf, dlen, 0, false);
		if( ret)
		{
			NetCommand* pcmd =new NetCommand( this->netcmd_fun_, ret);
			this->push_netcmd( pcmd);
		}

		gts_data_len_ -= sizeof( S_INT_32);
		gts_data_len_ -= dlen;
		pbuf += dlen;
		memmove( &(gts_recv_buffer_[0]), pbuf, gts_data_len_);
	}
}

void NetModule::wait_socket_close( int type)
{
	if( type == 0 || type == 2)
	{
		if( lgs_socket_ != INVALID_SOCKET)
		{
			while( ::closesocket( lgs_socket_) != 0)
				::Sleep( 1);

			lgs_socket_ =INVALID_SOCKET;
		}
	}

	if( type == 1 || type == 2)
	{
		if( gts_socket_ != INVALID_SOCKET)
		{
			while( ::closesocket( gts_socket_) != 0)
				::Sleep( 1);

			gts_socket_ =INVALID_SOCKET;
		}
	}
}
