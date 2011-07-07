#include "StdAfx.h"

#include "sharelib/Pro_Robot.h"
#include "tcpacceptuser.h"
#include "OptionUtil.h"

using namespace ROBOT_PROTOCOL;

extern OptionUtil g_opt;

#ifdef _WIN32
#define write(s,b,l) send(s,(const char*)b,l,0)
#define read(s,b,l) recv(s,(char*)b,l,0)
#define EWOULDBLOCK WSAEWOULDBLOCK
#undef errno
#define errno WSAGetLastError()
#undef EINTR
#define EINTR WSAEINTR
#endif

TcpAcceptUser::TcpAcceptUser( SOCKET socket, ITcpUserNotify* notify) :
socket_(socket),
notify_(notify)
{
}

TcpAcceptUser::~TcpAcceptUser(void)
{
	close();
}

void TcpAcceptUser::close()
{
	if (  INVALID_SOCKET != socket_)
	{
		closesocket( socket_);
		socket_ = INVALID_SOCKET;
	}

	notify_ = 0;

	NS_STL::list<BasicProtocol *>::iterator it = send_pro_pools_.begin();
	while ( it != send_pro_pools_.end())
	{
		BasicProtocol* p = *it;
		delete p;
		it = send_pro_pools_.erase(it);
	}
}

void TcpAcceptUser::do_main( DWORD now)
{
	unsigned long len = 0;

	do{
		int nRecv = Receive((char *)&len,4);
		if( nRecv == 0)
		{
			if ( notify_)
				notify_->onClosed( socket_);
			break;
		}

		if( nRecv < 0)
			break;

		if ( len > sizeof(buffer_))
			break;

		while(true)
		{
			nRecv = Receive(buffer_,len);
			if ( nRecv == 0 )
			{
				if ( notify_)
					notify_->onClosed( socket_);
				return;
			}

			if ( nRecv > 0 )
				break;
		}

		if ( notify_)
			notify_->onRecvData( buffer_, len, socket_);

		BasicProtocol* p = RobotProFactory::data_protocol( buffer_, len);
		if ( p && notify_) 
			notify_->onRecvProtocol( p, socket_);

	}while (false);	

	if ( 0 == send_protocol())
	{
		if ( notify_)
			notify_->onClosed( socket_);
	}
}

void TcpAcceptUser::send_protocol( BasicProtocol* p)
{
	send_pro_pools_.push_back( p);
}

int TcpAcceptUser::send_protocol()
{
	int ret = -1;

	NS_STL::list<BasicProtocol *>::iterator it = send_pro_pools_.begin();
	while ( it != send_pro_pools_.end())
	{
		BasicProtocol* p = *it;
		int len = 0;
		p->data( buffer_, 8192, len, (void*)0, false);

		delete p;
		it = send_pro_pools_.erase(it);

		if ( (ret = Send( (const char *)(&len), sizeof(int))) == 0)
			break;
		if ( (ret = Send( buffer_, len)) == 0)
			break;
	}

	return ret;
}

int TcpAcceptUser::Send (const char* pch,int nSize)
{
	if (socket_ == 0)
		return 0;

	while(nSize){
		int ret = ::write( socket_, pch, nSize );
		if(ret <= 0)
			return 0;
		pch	+= ret;
		nSize -= ret;
	}

	return 1;
}

int TcpAcceptUser::Receive (char* pch, int nSize)
{
	if (socket_ == 0)
		return 0;

	while(nSize){
		int ret = ::read(socket_, pch, nSize); 
		if(ret <= 0)
			return ret;
		pch	+= ret;
		nSize -= ret;
	}
	return 1;
}


