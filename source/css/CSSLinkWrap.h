/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSLINKWRAP__H__
#define __CSSLINKWRAP__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Auto_Ptr.h>

#include "CSS_Service.h"
#include "CSS2_Service.h"
#include "CSSLink.h"
#include "CSS2Link.h"
#include "CSSLocalLink.h"

/**
* @class CSSLinkWrap
*
* @brief 
*
**/
class CSSLinkWrap
{
public:
	CSSLinkWrap();

	/**
	* 初始化wrap
	**/
	void init_linkwrap();

	/**
	* 连接心跳
	* @param 
	**/
	void css2link_tick( ACE_UINT64 t);

	void fin_regist_service( CSS_Service* s, bool& autorelease);
	void fin_regist_service( CSS2_Service* s, bool& autorelease);

	void service_disconnected( CSS_Service* s);
	void service_disconnected( CSS2_Service* s);

	inline CSSLink* get_csslink( int server_index);
	inline CSS2Link* get_css2link( int server_index);

	INetLink* get_csslinkwrap( int server_index);

protected:
	//css之间的连接关系：index小的主动连接index大的服务器
	//被连接的css
	ACE_Auto_Array_Ptr<CSSLink>	css_;
	int							css_nums_;
	//主动连接的css
	ACE_Auto_Array_Ptr<CSS2Link>	css2_;
	int								css2_nums_;
	//如果2个maininstance或者ins分布在同一个进程内
	CSSLocalLink	csslocal_;

	int				owner_server_index_;
};

inline 
CSSLink* CSSLinkWrap::get_csslink( int server_index)
{
	for( int ii =0; ii < css_nums_; ++ii)
	{
		if( css_[ii].get_serverindex() == server_index)
			return &(css_[ii]);
	}
	return 0;
}

inline 
CSS2Link* CSSLinkWrap::get_css2link( int server_index)
{
	for( int ii =0; ii < css2_nums_; ++ii)
	{
		if( css2_[ii].get_serverindex() == server_index)
			return &(css2_[ii]);
	}
	return 0;
}

#endif	//__CSSLINKWRAP__H__
