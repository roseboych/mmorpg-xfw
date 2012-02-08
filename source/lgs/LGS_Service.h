/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGS_SERVICE__H__
#define __LGS_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <corelib/core/BasicService.h>
#include "Encrypt_Ext.h"

class Player;

/**
* @class LGS_Service
*
* @brief 
*
**/
class LGS_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~LGS_Service(void);

#ifdef CORELIB_WIN32
	explicit LGS_Service();
	explicit LGS_Service( ACE_Proactor *proactor);

	virtual void open( ACE_HANDLE new_handle, ACE_Message_Block &message_block);
#else
	LGS_Service( ACE_Reactor *reactor);

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
	//加密数据
	Encrypt_Ext		encrypt_;
	Player*			player_;

	//是否连接完成了
	bool	is_connect_;

	//全局索引
	int		server_index_;
};

#endif //__LGS_SERVICE__H__
