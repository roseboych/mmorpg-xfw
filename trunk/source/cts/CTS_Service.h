/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTS_SERVICE__H__
#define __CTS_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <corelib/core/BasicService.h>

/**
* @class CTS_Service
*
* @brief 
*
**/
class CTS_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~CTS_Service(void);

#ifdef CORELIB_WIN32
	explicit CTS_Service();
	explicit CTS_Service( ACE_Proactor *proactor);
#else
	CTS_Service( ACE_Reactor *reactor);
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
	/**
	* 协议和处理逻辑关联
	* @param p
	**/
	void from_gts( BasicProtocol *p);
	void from_css( BasicProtocol *p);
	void from_dpx( BasicProtocol *p);

public:
	//连接类型
	NETSVR_TYPE_ENUM	type_;
	int					svrsn_;
	int					sn_;
	//全局索引
	int					server_index_;
};

#endif //__CTS_SERVICE__H__
