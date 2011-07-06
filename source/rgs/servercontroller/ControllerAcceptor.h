/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CONTROLLERACCEPTOR__H__
#define __CONTROLLERACCEPTOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../rgs_pre.h"

#include <corelib/net/GuardAcceptor.h>
#include "Controller_Service.h"

/**
* @class ControllerAcceptor
* 
* @brief
* 
**/
class ControllerAcceptor : public GuardAcceptor<Controller_Service>
{
public:

#ifdef CORELIB_WIN32
	virtual Controller_Service *make_handler(void);
#else
	virtual int handle_input( ACE_HANDLE = ACE_INVALID_HANDLE);
#endif

};

#endif	//__CONTROLLERACCEPTOR__H__
