/**
* dpx application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSLINK__H__
#define __CSSLINK__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "dpx_pre.h"

#include <corelib/core/LinkBase.h>
#include <reslib/deploy/CSSOption.h>

#include "DPX_Service.h"

/**
* @class CSSLink
*
* @brief 
*
**/
class CSSLink : public LinkBase<DPX_Service>
{
	typedef LinkBase<DPX_Service> inherit;
public:
	CSSLink();

	virtual void reset();

	void set_option( CSSOption* s);

public:
	//配置
	CSSOption* opt_;
};

#endif	//__CSSLINK__H__
