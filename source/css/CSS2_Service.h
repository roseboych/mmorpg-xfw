/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSS2_SERVICE__H__
#define __CSS2_SERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include "CSS_Service.h"

class CSS2Link;

/**
* @class CSS2_Service
*
* @brief 
*
**/
class CSS2_Service : public CSS_Service
{
	typedef CSS_Service inherit;
public:
	virtual ~CSS2_Service();

#ifdef CORELIB_WIN32
	explicit CSS2_Service();
	explicit CSS2_Service( ACE_Proactor *proactor);
	virtual void act( const void *);
#else
	CSS2_Service( ACE_Reactor *reactor);
	virtual void set_act( const void *pact);
#endif

	virtual bool recv_msg( BasicProtocol* protocol);

	/**
	* @see BasicService::get_serviceimpl()
	**/
	virtual int get_serviceimpl();

protected:
	virtual void begin_registservice_2();

public:
	CSS2Link*	server_link_;
};

#endif //__CSS2_SERVICE__H__
