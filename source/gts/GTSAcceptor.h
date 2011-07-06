/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSACCEPTOR__H__
#define __GTSACCEPTOR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "gts_pre.h"

#include <corelib/net/GuardAcceptor.h>
#include "GTS_Service.h"

/**
* @class GTSAcceptor
* 
* @brief GTS网络监听器。
* 
**/
class GTSAcceptor : public GuardAcceptor<GTS_Service>
{
public:

#ifdef CORELIB_WIN32
	virtual GTS_Service *make_handler(void);
#else
	virtual int handle_input( ACE_HANDLE = ACE_INVALID_HANDLE);
#endif

};

#endif	//__GTSACCEPTOR__H__
