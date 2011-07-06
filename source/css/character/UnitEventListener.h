/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __UNITEVENTLISTENER__H__
#define __UNITEVENTLISTENER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"

class UnitBase;

/**
* @class UnitEventListener
* 
* @brief unit对象的事件监听器
**/
class UnitEventListener
{
public:
	UnitEventListener(){}

	/**
	* 对象重置通知
	**/
	virtual void event_unitentity_reset( UnitBase* pobj) =0;
};

#endif	//__UNITEVENTLISTENER__H__
