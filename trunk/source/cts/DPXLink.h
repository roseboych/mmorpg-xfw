/**
* cts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DPXLINK__H__
#define __DPXLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <corelib/core/LinkBase.h>
#include <reslib/deploy/DPXOption.h>

#include "CTS_Service.h"

class DPXLink : public LinkBase<CTS_Service>
{
	typedef LinkBase<CTS_Service> inherit;
public:
	DPXLink();

	virtual void reset();

	void set_option( DPXOption* s);

public:
	//配置
	DPXOption* opt_;
};

#endif	//__DPXLINK__H__
