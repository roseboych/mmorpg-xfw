/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CONTROLLERLINK__H__
#define __CONTROLLERLINK__H__

#include <prolib/core_type.h>
#include <corelib/core/INetLink.h>

#include "Controller_Service.h"

/**
* @class ControllerLink
* 
* @brief controller连接器
* 
**/
class ControllerLink : public INetLink
{
public:
	ControllerLink();

	virtual void send_protocol( BasicProtocol* p);

	virtual int get_serverindex(){ return NO_INITVALUE;}

	Controller_Service* get_controllerservice(){ return &service_;}

	bool initialize_link();

	void reset();

	bool finish_login( bool succ);

	bool is_login();

protected:
	Controller_Service	service_;

	bool				is_login_;
	int					login_failed_nums_;

	ACE_Thread_Mutex	mutex_;
	bool				initialized_;
};

#endif //__CONTROLLERLINK__H__
