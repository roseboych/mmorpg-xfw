/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CONTROLLER_SERVICE__H__
#define __CONTROLLER_SERVICE__H__

#include "../rgs_pre.h"

#include <corelib/core/BasicService.h>

/**
* @class Controller_Service
* 
* @brief
* 
**/
class Controller_Service : public BasicService
{
	typedef BasicService inherit;
public:
	virtual ~Controller_Service(void);

#ifdef CORELIB_WIN32
	explicit Controller_Service();
	explicit Controller_Service( ACE_Proactor *proactor);

	virtual void open( ACE_HANDLE new_handle, ACE_Message_Block &message_block);

#else
	
	Controller_Service( ACE_Reactor *reactor);

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

public:
	NETSVR_TYPE_ENUM	type_;
};

#endif //__CONTROLLER_SERVICE__H__
