/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LINKBASE__H__
#define __LINKBASE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include "corelib/core/BasicService.h"
#include "corelib/core/INetLink.h"

/**
* @class LinkBase
* 
* @brief 服务器连接的包装类
**/
template< class T>
class LinkBase : public INetLink
{
public:
	LinkBase();
	virtual ~LinkBase();

	inline virtual void set_linkservice( T* s);

	/**
	* @see INetLink::send_protocol( BasicProtocol* p)
	**/
	virtual void send_protocol( BasicProtocol* p);

	/**
	* 是否已经注册成功
	* @return
	**/
	virtual bool is_connected(){ return service_ > 0;}

	/**
	* 是否需要发起连接
	* @return
	**/
	virtual bool need_connected(){ return false;}

	/**
	* 是否等待验证返回
	* @return
	**/
	virtual bool is_waitregistack(){ return false;}

	/**
	* 连接到服务器
	* @param ip
	* @param port
	* @return
	**/
	virtual bool connect_to_server(){ return false;}

	virtual void reset();

	/**
	* 断开和服务器的连接
	**/
	virtual void disconn_to_server();

	operator T* (){ return service_;}

	virtual int get_serverindex(){ return server_index_;}

	void set_serverindex( int s){ server_index_ =s;}

protected:
	T*	service_;
	int	server_index_;
};

template< class T>
LinkBase<T>::LinkBase():service_(0)
{}

template< class T>
LinkBase<T>::~LinkBase()
{
	if( service_)
		delete service_;
	service_ =0;
}

template< class T>
inline void LinkBase<T>::set_linkservice(T *s)
{
	this->service_ =s;
}

template< class T>
inline void LinkBase<T>::send_protocol( BasicProtocol* p)
{
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	if( service_)
	{
		if( service_->append_write_msg( p) == 0)
			p_pro.release();
	}
}

template< class T>
void LinkBase<T>::disconn_to_server()
{
	if( service_)
		service_->close_service( true);
}

template< class T>
void LinkBase<T>::reset()
{
	service_ =0;
}

#endif	//__LINKBASE__H__
