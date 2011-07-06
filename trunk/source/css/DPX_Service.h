/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPX_SERVICE__H__
#define __DPX_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <corelib/core/BasicService.h>

class DPXLink;

/**
* @class DPX_Service
*
* @brief 
*
**/
class DPX_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~DPX_Service(void);

#ifdef CORELIB_WIN32
	explicit DPX_Service();
	explicit DPX_Service( ACE_Proactor *proactor);
	virtual void act( const void *);
#else
	CTS_Service( ACE_Reactor *reactor);
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

public:
	//全局索引
	S_INT_16	server_index_;
	DPXLink*	server_link_;
};

#endif //__DPX_SERVICE__H__
