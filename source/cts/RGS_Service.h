/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGS_SERVICE__H__
#define __RGS_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <corelib/core/BasicService.h>

class RGSLink;

/**
* @class RGS_Service
*
* @brief 
*
**/
class RGS_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~RGS_Service(void);

#ifdef CORELIB_WIN32
	explicit RGS_Service();
	explicit RGS_Service( ACE_Proactor *proactor);
	virtual void act( const void *);
#else
	RGS_Service( ACE_Reactor *reactor);
	virtual void set_act( const void *pact);
#endif

	/**
	* @see BasicService::conn_error()
	**/
	virtual void conn_error();
	
	/**
	* @see BasicService::recv_msg( BasicProtocol* protocol)
	**/
	virtual bool recv_msg( BasicProtocol* protocol);

	/**
	* @see BasicService::get_serviceimpl()
	**/
	virtual int get_serviceimpl();

protected:
	virtual void begin_registservice_2();

	/**
	* 协议和处理逻辑关联
	* @param p
	**/
	void assort_protocol( BasicProtocol *p);

public:
	//全局索引
	int			server_index_;
	RGSLink*	server_link_;
};

#endif //__RGS_SERVICE__H__
