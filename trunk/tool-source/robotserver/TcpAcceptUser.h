#pragma once

#include <list>

#include <prolib/core_type.h>
#include <prolib/BasicProtocol.h>

class ITcpUserNotify
{
public:
	virtual void onAccept( SOCKET socket)=0;
	virtual void onClosed( SOCKET socket)=0;
	virtual void onRecvData( char * data, int len, SOCKET socket)=0;
	virtual void onRecvProtocol( BasicProtocol* p, SOCKET socket){};
};

class TcpAcceptUser
{
public:
	TcpAcceptUser( SOCKET socket, ITcpUserNotify* notify);
	virtual ~TcpAcceptUser(void);

	void do_main( DWORD now);
	void send_protocol( BasicProtocol* p);

	int Send (const char* pch, int nSize);
	int Receive (char* pch, int nSize);

private:
	void close();
	int send_protocol();

public:
	NS_STL::string peerip_;
	unsigned short peerport_;

private:
	SOCKET socket_;
	ITcpUserNotify* notify_;
	NS_STL::list<BasicProtocol *> send_pro_pools_;
	char buffer_[8192];
};
