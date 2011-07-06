/**
* corelib
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "corelib/util/ValueSubject.h"

#include "corelib/util/IValueObserver.h"
#include <algorithm>

ValueSubject::~ValueSubject()
{
	clear_all();
}

void ValueSubject::regist_one( IValueObserver* obsvr)
{
	value_listeners_.push_back( obsvr);
}

void ValueSubject::remove_one( IValueObserver* obsvr)
{
	NS_STL::vector<IValueObserver*>::iterator fiter =
		NS_STL::find( value_listeners_.begin(), value_listeners_.end(), obsvr);
	
	if( fiter == value_listeners_.end())
		return;

	value_listeners_.erase( fiter);
}

void ValueSubject::clear_all()
{
	value_listeners_.clear();
}

void ValueSubject::notify_valuechange()
{
	for( size_t i =0; i < value_listeners_.size(); ++i)
		value_listeners_[i]->notify_valuechange();
}
