#include "StdAfx.h"

#include "windows.h"

#include <sharelib/Pro_Robot.h>

#include "logmacro.h"
#include "testcase.h"
#include "BasicImpl.h"
#include "MsgNotify.h"

#include "testcase/LogonTestImpl.h"
#include "testcase/SelSvrTeamTestImpl.h"
#include "testcase/SelRoleTestImpl.h"

extern CString	g_ip;
extern int		g_port;
extern CWnd*	g_mainwnd;
extern TEST_TYPE g_testtype;

#define WM_INFONOTIFY  WM_USER+0x100
#define WM_ERRORNOTIFY  WM_USER+0x101

void post_msg( MsgNotify *msg)
{
	if( g_mainwnd == 0)
	{
		delete msg;
		return;
	}

	g_mainwnd->PostMessage( WM_INFONOTIFY, (WPARAM)msg, 0);
}

void post_error( MsgNotify *msg)
{
	if( g_mainwnd == 0)
	{
		delete msg;
		return;
	}

	g_mainwnd->PostMessage( WM_ERRORNOTIFY, (WPARAM)msg, 0);
}

TestCase::TestCase()
:data_( 0),
impl_( 0),
need_connect_2_lgs_( true),
lgs_data_len_( 0),
lgs_socket_( INVALID_SOCKET),
gts_data_len_( 0),
gts_socket_( INVALID_SOCKET),
cancel_( false)
{
	start_connect_t_ =BasicImpl::GetMillisecondTime();
	delay_connect_ =0;

	data_ =new ProtocolFactory();

	switch( g_testtype) 
	{

	case T_LOGIN_LOGOUT:
		impl_=new LogonTestImpl( this);
		break;

	case T_LOGIN_SELSVRTEAM:
		impl_=new SelSvrTeamTestImpl( this);
		break;
		
	case T_LOGIN_SELROLE:
		impl_=new SelRoleTestImpl( this);
		break;

	default:
		break;
	}
}

void TestCase::set_userid( int id)
{
	userid_ =id;
	if ( impl_)
		impl_->set_namepwd( userid_);
}

TestCase::~TestCase(void)
{
	if( impl_)
		delete impl_;
	if( data_)
		delete data_;
}

void TestCase::run_it( ULONGLONG now)
{
	if( cancel_)
		return;

	if( need_connect_2_lgs_)
	{
		if( start_connect_t_ + delay_connect_ > now)
			return;

		int err =0;
		if( !connect_2_lgs( err))
		{
			MODULE_LOG_DEBUG((MODULE_TEMP, "--------------------------------------- connect to lgs failed err:%d", err));
			return;
		}

		need_connect_2_lgs_ =false;
	}

	if( impl_)
	{
		if( !impl_->islogon())
			impl_->logon();
	}

	if ( impl_)
		impl_->status_check( now);

	recv_lgs_protocol();

	recv_gts_protocol();
}

#define MAXCASE_PRETHREAD	20

void TestCase::start_case()
{
	if ( (userid_ % MAXCASE_PRETHREAD) == ( MAXCASE_PRETHREAD - 1 ))
	{
		MsgNotify *msg =new MsgNotify();
		msg->userid_ =userid_;
		msg->msg_.Format("user%d start....",userid_);
		post_msg( msg);
	}
}

void TestCase::stop_case()
{
	if ( (userid_ % MAXCASE_PRETHREAD) == ( MAXCASE_PRETHREAD - 1 ))
	{
		MsgNotify *msg =new MsgNotify();
		msg->userid_ =userid_;
		msg->msg_.Format("user%d end....", userid_);
		post_msg( msg);
	}

	wait_socket_close();
}

void TestCase::send_lgs_protocol( BasicProtocol *p)
{
	if( lgs_socket_ == INVALID_SOCKET)
		return;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	p->data( pbuf, SINGLE_PACK_LEN, len, (void*)0, false);
	*((S_INT_32*)buf) =len;

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
			::closesocket( lgs_socket_);
			lgs_socket_ =INVALID_SOCKET;
			lgs_data_len_ =0;
		}
	}
}

void TestCase::recv_lgs_protocol()
{
	if( lgs_socket_ == INVALID_SOCKET)
		return;

	char *pbuf =lgs_recv_buffer_+lgs_data_len_;
	int len =MAX_PACK_LEN - lgs_data_len_;

	int rlen =::recv( lgs_socket_, pbuf, len, 0);

	if( rlen == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket( lgs_socket_);
			lgs_socket_ =INVALID_SOCKET;
			lgs_data_len_ =0;

			return;
		}
		
		if( lgs_data_len_ < sizeof( S_INT_32))
			return;
	}

	if( rlen >= 0)
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
		BasicProtocol *ret =data_->data_protocol( pbuf, dlen, 0, false);
		if( ret)
		{
			if ( impl_)
				impl_->do_lgs( ret);
			delete ret;
		}

		lgs_data_len_ -= sizeof( S_INT_32);
		lgs_data_len_ -= dlen;
		pbuf += dlen;
		memmove( &(lgs_recv_buffer_[0]), pbuf, lgs_data_len_);
	}
}

bool TestCase::connect_2_lgs( int& err)
{
	if( lgs_socket_ != INVALID_SOCKET)
		disconnect_2_lgs();

	lgs_socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( lgs_socket_ == INVALID_SOCKET)
		return false;

	int one =1;
	::setsockopt( lgs_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof( one));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( (LPCTSTR)g_ip);
	serv_addr.sin_port			= htons( g_port);

	if( ::connect( lgs_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		err =WSAGetLastError();
		::closesocket( lgs_socket_);
		lgs_socket_ =INVALID_SOCKET;

		return false;
	}
	else
	{
		u_long ll =1;
		ioctlsocket( lgs_socket_, FIONBIO, &ll);

		return true;
	}
}

bool TestCase::connect_2_gts( int& err)
{
	if( gts_socket_ != INVALID_SOCKET)
		disconnect_2_gts();

	gts_socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( gts_socket_ == INVALID_SOCKET)
		return false;

	int one =1;
	::setsockopt( gts_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof( one));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( impl_->gts_ip_.c_str());
	serv_addr.sin_port			= htons( impl_->gts_port_);

	if( ::connect( gts_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		err =WSAGetLastError();
		::closesocket( gts_socket_);
		gts_socket_ =INVALID_SOCKET;

		return false;
	}
	else
	{
		u_long ll =1;
		ioctlsocket( gts_socket_, FIONBIO, &ll);

		return true;
	}
}

void TestCase::send_gts_protocol( BasicProtocol *p)
{
	if( gts_socket_ == INVALID_SOCKET)
		return;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	p->data( pbuf, SINGLE_PACK_LEN, len, (void*)0, false);
	*((S_INT_32*)buf) =len;

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
			::closesocket( gts_socket_);
			gts_socket_ =INVALID_SOCKET;

			gts_data_len_ =0;
		}
	}
}

void TestCase::recv_gts_protocol()
{
	if( gts_socket_ == INVALID_SOCKET)
		return;

	char *pbuf =gts_recv_buffer_+gts_data_len_;
	int len =MAX_PACK_LEN - gts_data_len_;

	int rlen =::recv( gts_socket_, pbuf, len, 0);

	if( rlen == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket( gts_socket_);
			gts_socket_ =INVALID_SOCKET;
			gts_data_len_ =0;

			return;
		}

		if( gts_data_len_ < sizeof( S_INT_32))
			return;
	}

	if( rlen >= 0)
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
		BasicProtocol *ret =data_->data_protocol( pbuf, dlen, 0, false);
		if( ret)
		{
			if ( impl_)
				impl_->do_gts( ret);
			delete ret;
		}

		gts_data_len_ -= sizeof( S_INT_32);
		gts_data_len_ -= dlen;
		pbuf += dlen;
		memmove( &(gts_recv_buffer_[0]), pbuf, gts_data_len_);
	}
}

void TestCase::disconnect_2_lgs()
{
	if( lgs_socket_ != INVALID_SOCKET)
	{
		::closesocket( lgs_socket_);

		lgs_socket_ =INVALID_SOCKET;
	}
}

void TestCase::reconnect_2_lgs()
{
	need_connect_2_lgs_ =true;
}

void TestCase::disconnect_2_gts()
{
	if( gts_socket_ != INVALID_SOCKET)
	{
		::closesocket( gts_socket_);
		gts_socket_ =INVALID_SOCKET;
	}
}

void TestCase::wait_socket_close()
{
	if( lgs_socket_ != INVALID_SOCKET)
	{
		::closesocket( lgs_socket_);

		lgs_socket_ =INVALID_SOCKET;
	}

	if( gts_socket_ != INVALID_SOCKET)
	{
		::closesocket( gts_socket_);

		gts_socket_ =INVALID_SOCKET;
	}
}

bool TestCase::force_close()
{
	wait_socket_close();

	start_connect_t_ =BasicImpl::GetMillisecondTime();

	return true;
}
