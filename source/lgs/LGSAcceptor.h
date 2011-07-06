/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGSACCEPTOR__H__
#define __LGSACCEPTOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <corelib/net/GuardAcceptor.h>
#include "LGS_Service.h"

/**
* @class LGSAcceptor
* 
* @brief LGS网络监听器。
* 
**/
class LGSAcceptor : public GuardAcceptor<LGS_Service>
{
public:

#ifdef CORELIB_WIN32
	virtual LGS_Service *make_handler(void);
#else
	virtual int handle_input( ACE_HANDLE = ACE_INVALID_HANDLE);
#endif

};

#endif	//__LGSACCEPTOR__H__
