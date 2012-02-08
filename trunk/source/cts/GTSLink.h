/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GTSLINK__H__
#define __GTSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <corelib/core/LinkBase.h>
#include <reslib/deploy/GTSOption.h>

#include "CTS_Service.h"

class GTSLink : public LinkBase<CTS_Service>
{
	typedef LinkBase<CTS_Service> inherit;
public:
	GTSLink();

	virtual void reset();

	void set_option( GTSOption* s);

	/**
	* 检查玩家索引是否在gts上
	* @param userindex
	**/
	bool is_useringts( int userindex);

public:
	//配置
	GTSOption* opt_;
};

#endif	//__GTSLINK__H__
