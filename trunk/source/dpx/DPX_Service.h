/**
* dpx application
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

#include "dpx_pre.h"

#include <corelib/core/BasicService.h>

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
#else
	DPX_Service( ACE_Reactor *reactor);
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
	void from_css( BasicProtocol *p);

public:
	//连接类型
	NETSVR_TYPE_ENUM	type_;
	int					svrsn_;
	int					sn_;
	//全局索引
	int					server_index_;
};

#endif //__DPX_SERVICE__H__
