/**
* rgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __RGS_SERVICE__H__
#define __RGS_SERVICE__H__

#include "rgs_pre.h"

#include <corelib/core/BasicService.h>

/**
* @class RGS_Service
*
* @brief 和global服务器连接的socket连接描述
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
#else
	RGS_Service( ACE_Reactor *reactor);
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
	void lgs_protocol( BasicProtocol *p);

	void cts_protocol( BasicProtocol* p);

public:
	//连接类型
	NETSVR_TYPE_ENUM	type_;
	int					svrsn_;
	int					sn_;
	//全局索引
	int					server_index_;
};

#endif //__RGS_SERVICE__H__
