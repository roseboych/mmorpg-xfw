/**
* corelib
*
* @category		net module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GUARDACCEPTOR__H__
#define __GUARDACCEPTOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "corelib/corelibdef.h"

#ifdef CORELIB_WIN32
	#include <ace/Asynch_Acceptor.h>
#else
	#include <ace/SOCK_Acceptor.h>
	#include <ace/Event_Handler.h>
	#include <ace/Reactor.h>
#endif

#include "corelib/core/BasicModule.h"

/**
* @class GuardAcceptor
* 
* @brief socket监听器
* 
**/
template<class T>
class GuardAcceptor
#ifdef CORELIB_WIN32
	: public ACE_Asynch_Acceptor<T>
#else
	: public ACE_Event_Handler
#endif
{
public:
	GuardAcceptor(void);
	virtual ~GuardAcceptor(void);

#ifdef CORELIB_WIN32
	/**
	* 构造连接对象
	* @return
	**/
	virtual T *make_handler(void);

	virtual int validate_connection ( const ACE_Asynch_Accept::Result& result,
		const ACE_INET_Addr &remote, const ACE_INET_Addr& local);

	/**
	* 开始监听
	* @param port
	* @return
	**/
	bool begin_listen( u_short port);

#else
	virtual int open (const ACE_INET_Addr &local_addr);

	// Called by a reactor when there's a new connection to accept.
	virtual int handle_input( ACE_HANDLE = ACE_INVALID_HANDLE);

	// Called when this object is destroyed, e.g., when it's
	// removed from a reactor.
	virtual int handle_close( ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);

	virtual ACE_HANDLE get_handle () const{ 
		return acceptor_.get_handle (); 
	};

	ACE_SOCK_Acceptor &acceptor (){ 
		return acceptor_;
	};

	/**
	* 开始监听
	* @param port
	* @return
	**/
	bool begin_listen( u_short port);

#endif

	//close acceptor
	void close_acceptor();

	inline void set_acceptor( bool a);
	inline bool get_acceptor();

private:

#ifdef CORELIB_WIN32
#else
	ACE_SOCK_Acceptor acceptor_;
#endif

	//是否接受监听
	bool can_acceptor_;
};

#include <corelib/log/logmacro.h>
#include <ace/Thread.h>

template<class T>
GuardAcceptor<T>::GuardAcceptor(void):can_acceptor_( true)
{
}

template<class T>
GuardAcceptor<T>::~GuardAcceptor(void)
{
#ifndef CORELIB_WIN32
	this->handle_close( ACE_INVALID_HANDLE, 0);
#endif
}

template<class T>
void GuardAcceptor<T>::close_acceptor()
{
#ifdef CORELIB_WIN32
	if( get_handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::closesocket( get_handle());
		handle( ACE_INVALID_HANDLE);
	}

#else
	acceptor_.close();

#endif
}

template<class T>
void GuardAcceptor<T>::set_acceptor( bool a)
{
	this->can_acceptor_ =a;
}

template<class T>
bool GuardAcceptor<T>::get_acceptor()
{
	return this->can_acceptor_;
}

#ifdef CORELIB_WIN32

template<class T>
T* GuardAcceptor<T>::make_handler(void)
{
	T *ret =0;
	ret =PROTOCOL_NEW T( this->proactor());

	return ret;
}

template<class T>
int GuardAcceptor<T>::validate_connection ( const ACE_Asynch_Accept::Result& result,
						 const ACE_INET_Addr &remote, const ACE_INET_Addr& local)
{
	return can_acceptor_?0:-1;
}

template<class T>
bool GuardAcceptor<T>::begin_listen( u_short port)
{
	ACE_INET_Addr listen_addr;
	listen_addr.set_port_number( port);

	//set proactor instance
	if( open( listen_addr, 0, 0, ACE_DEFAULT_ASYNCH_BACKLOG, 1, BasicModule::base_instance()->proactor(), true) != 0)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "GuardAcceptor<T> can't open listen socket!");
		return false;
	}

	return true;
}

#else

template<class T>
int GuardAcceptor<T>::open( const ACE_INET_Addr &local_addr)
{
	if( acceptor_.open( local_addr) == -1)
		return -1;

	acceptor_.enable( ACE_NONBLOCK);

	return reactor()->register_handler( this, ACE_Event_Handler::ACCEPT_MASK);
}

template<class T>
int GuardAcceptor<T>::handle_input( ACE_HANDLE handle)
{
	T *service =PROTOCOL_NEW T( reactor());
	if( acceptor_.accept( service->peer()) == -1)
	{
		delete service;
		MODULE_LOG_WARN( MODULE_BOOT, "GuardAcceptor::handle_input accept failed");
	}
	else if( service->open() != 0)
	{
		service->handle_close();
	}

	return 0;
}

template<class T>
int GuardAcceptor<T>::handle_close( ACE_HANDLE, ACE_Reactor_Mask)
{
	if( acceptor_.get_handle() != ACE_INVALID_HANDLE)
	{
		ACE_Reactor_Mask m =ACE_Event_Handler::ACCEPT_MASK|ACE_Event_Handler::DONT_CALL;

		this->reactor()->remove_handler( this, m);
		this->acceptor_.close();
	}

	return 0; 
}

template<class T>
bool GuardAcceptor<T>::begin_listen( u_short port)
{
	ACE_INET_Addr listen_addr;
	listen_addr.set_port_number( port);

	accepter_.reactor( BasicModule::base_instance()->reactor());

	if( accepter_.open( listen_addr) != 0)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "GuardAcceptor<T> can't open listen socket!");
		return false;
	}

	return true;
}

#endif	//CORELIB_WIN32

#endif	//__GUARDACCEPTOR__H__
