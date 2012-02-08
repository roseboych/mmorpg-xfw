/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSS_SERVICE__H__
#define __CSS_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <corelib/core/BasicService.h>

/**
* @class CSS_Service
*
* @brief 
*
**/
class CSS_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~CSS_Service(void);

#ifdef CORELIB_WIN32
	explicit CSS_Service();
	explicit CSS_Service( ACE_Proactor *proactor);
#else
	CSS_Service( ACE_Reactor *reactor);
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

public:
	//连接类型
	NETSVR_TYPE_ENUM	type_;
	int					svrsn_;
	int					sn_;

	//全局索引
	int					server_index_;
};

#endif //__CSS_SERVICE__H__
