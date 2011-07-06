/**
* corelib
*
* @category		net module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GUARDCONNECTOR__H__
#define __GUARDCONNECTOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "corelib/corelibdef.h"

#ifdef CORELIB_WIN32
#include "ace/Asynch_Connector.h"
#endif

#include "corelib/core/LinkBase.h"
#include "corelib/core/BasicModule.h"

/**
* @class GuardConnector
* 
* @brief socket连接向导
* 
**/
template< class T>
class GuardConnector
#ifdef CORELIB_WIN32
	 : public ACE_Asynch_Connector<T>
#endif
{
public:
	GuardConnector();
	
	/**
	* 连接到服务器
	* @param ip
	* @param port
	* @param lk
	* @return
	**/
	bool connect_2_server( const char* ip, int port, LinkBase<T>* lk);

	inline void set_connector( bool c);
	inline bool get_connector();

protected:

#ifdef CORELIB_WIN32
	/**
	* 创建对应的service
	**/
	virtual T *make_handler(void);

	virtual int validate_connection (const ACE_Asynch_Connect::Result& result,
		const ACE_INET_Addr &remote, const ACE_INET_Addr& local);
#endif

	//是否能够发起连接
	bool can_connector_;
};

template<class T>
GuardConnector<T>::GuardConnector():can_connector_( true)
{
#ifdef CORELIB_WIN32
	ACE_Asynch_Connector<T>::validate_new_connection( true);
#endif
}

template<class T>
void GuardConnector<T>::set_connector( bool c)
{
	can_connector_ =c;
}

template<class T>
bool GuardConnector<T>::get_connector()
{
	return can_connector_;
}

#ifdef CORELIB_WIN32

template<class T>
T* GuardConnector<T>::make_handler(void)
{
	T* ret =0;
	ret =PROTOCOL_NEW T( this->proactor());
	
	return ret;
}

template<class T>
int GuardConnector<T>::validate_connection (const ACE_Asynch_Connect::Result& result,
						 const ACE_INET_Addr &remote, const ACE_INET_Addr& local)
{
	return 0;
}

#endif

template<class T>
bool GuardConnector<T>::connect_2_server( const char* ip, int port, LinkBase<T>* lk)
{
	//是否允许发起连接
	if( !can_connector_)
		return false;

	//连接到server
	ACE_INET_Addr conn_addr( (u_short)port, ip);

#ifdef CORELIB_WIN32
	if( this->open( 0, BasicModule::base_instance()->proactor()) == -1)
		return false;
	if( this->connect( conn_addr, (const ACE_INET_Addr &)ACE_Addr::sap_any, 1, lk) == -1)
		return false;

#else
	
	ACE_SOCK_Connector	connector_;
	T *service =PROTOCOL_NEW T( BasicModule::base_instance()->reactor());
	service->set_act( lk);
	if( connector_.connect( service->peer(), conn_addr) != 0)
	{
		delete service;
		return false;
	}
	else if( service->open() != 0)
	{
		service->handle_close();
		return false;
	}

#endif

	return true;
}

#endif	//__GUARDCONNECTOR__H__
