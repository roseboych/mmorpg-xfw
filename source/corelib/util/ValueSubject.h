/**
* corelib
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __VALUESUBJECT__H__
#define __VALUESUBJECT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include <vector>

class IValueObserver;

/**
* @class ValueSubject
* 
* @brief 
**/
class ValueSubject
{
public:
	virtual ~ValueSubject();

	void regist_one( IValueObserver* obsvr);
	void remove_one( IValueObserver* obsvr);
	void clear_all();

protected:
	void notify_valuechange();

protected:
	NS_STL::vector<IValueObserver*>	value_listeners_;
};

#endif	//__VALUESUBJECT__H__