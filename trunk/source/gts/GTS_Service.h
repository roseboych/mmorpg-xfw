/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTS_SERVICE__H__
#define __GTS_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "gts_pre.h"

#include <corelib/core/BasicService.h>
#include "player/Encrypt_Ext.h"

class Player;

/**
* @class GTS_Service
*
* @brief 
*
**/
class GTS_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~GTS_Service(void);

#ifdef CORELIB_WIN32
	explicit GTS_Service();
	explicit GTS_Service( ACE_Proactor *proactor);

	virtual void open( ACE_HANDLE new_handle, ACE_Message_Block &message_block);
#else
	GTS_Service( ACE_Reactor *reactor);

	virtual int open();
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

	/**
	* 心跳检查
	* @param now
	**/
	void online_tickit( ACE_UINT64& now);

	void reset();

protected:
	/**
	* 协议和处理逻辑关联
	* @param p
	**/
	void assort_protocol( BasicProtocol *p);

public:
	//连接的玩家信息
	//加密数据
	Encrypt_Ext		encrypt_;
	Player*			player_;

	//是否连接完成了
	bool	is_connect_;

	//全局索引
	int		server_index_;
};

#endif //__GTS_SERVICE__H__
