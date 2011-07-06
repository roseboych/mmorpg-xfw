/**
* corelib
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __IVALUEOBSERVER__H__
#define __IVALUEOBSERVER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <vector>

/**
* @class IValueObserver
* 
* @brief 
**/
class IValueObserver
{
public:
	/**
	* 值变化通知
	**/
	virtual void notify_valuechange() =0;
};

#endif	//__IVALUEOBSERVER__H__