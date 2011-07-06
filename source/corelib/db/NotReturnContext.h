/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __NOTRETURNCONTEXT__H__
#define __NOTRETURNCONTEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "corelib/db/BasicPoolContext.h"
#include "corelib/core/BasicCommand.h"
#include "prolib/core_type.h"

/**
* @class NotReturnContext
* 
* @brief database线程池无返回类型的模板类
*
**/
template<class _T>
class NotReturnContext : public BasicPoolContext
{
    typedef BasicPoolContext inherit;
public:
	//无序命令
	NotReturnContext( _T *impl, bool deleteit =true);
	//有序命令
	virtual ~NotReturnContext();

	virtual int get_type();
	virtual bool validate();

	/**
	* 在数据库线程中执行的入口封装
	* @param pconn
	**/
	virtual void run_it( otl_connect *pconn);

private:
	_T		*impl_;
	bool	delete_it_;
};

template<class _T>
NotReturnContext<_T>::NotReturnContext( _T *impl, bool deleteit)
:impl_( impl),delete_it_( deleteit)
{
	this->return_it_ =true;
}

template<class _T>
NotReturnContext<_T>::~NotReturnContext()
{
	if( delete_it_)
		delete impl_;
	impl_ =0;
}

template<class _T>
int NotReturnContext<_T>::get_type()
{
	return impl_->get_type();
}

template<class _T>
bool NotReturnContext<_T>::validate()
{
	return impl_->validate();
}

template<class _T>
void NotReturnContext<_T>::run_it( otl_connect *pconn)
{
	impl_->rundb( pconn);
}


#endif //__NOTRETURNCONTEXT__H__
