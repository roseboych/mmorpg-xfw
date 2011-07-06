/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MUSTRETURNCONTEXT__H__
#define __MUSTRETURNCONTEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "corelib/db/BasicPoolContext.h"
#include "corelib/core/BasicCommand.h"

#include "prolib/core_type.h"

/**
* @class MustReturnContext
* 
* @brief database线程池返回类型的模板类
*
**/
template<class _T>
class MustReturnContext : public BasicPoolContext, public BasicCommand
{
    typedef BasicPoolContext inherit;

public:
	MustReturnContext( _T *impl, bool deleteit =true);
	virtual ~MustReturnContext();

	virtual int get_type();
	virtual bool validate();

	/**
	* 在数据库线程中执行的入口封装
	* @param pconn
	**/
	virtual void run_it( otl_connect *pconn);

	/**
	* 在逻辑主线程中执行的入口封装
	**/
	virtual void run();

private:
	//具体的数据库任务实现
	_T		*impl_;
	//是否删除
	bool	delete_it_;
};

template<class _T>
MustReturnContext<_T>::MustReturnContext( _T *impl, bool deleteit)
:impl_( impl),delete_it_( deleteit)
{
	this->return_it_ =true;
}

template<class _T>
MustReturnContext<_T>::~MustReturnContext()
{
	if( delete_it_)
		delete impl_;
	impl_ =0;
}

template<class _T>
int MustReturnContext<_T>::get_type()
{
	return impl_->get_type();
}

template<class _T>
bool MustReturnContext<_T>::validate()
{
	return impl_->validate();
}

template<class _T>
void MustReturnContext<_T>::run_it( otl_connect *pconn)
{
	impl_->rundb( pconn);
}

template<class _T>
void MustReturnContext<_T>::run()
{
	if( validate())
		impl_->run_inmainpool();
}

#endif //__MUSTRETURNCONTEXT__H__
