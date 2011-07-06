/**
* corelib
*
* @category		corelib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "TLSContext.h"

#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_Thread.h>

ACE_thread_key_t TLSContext::tls_key_ =NO_INITVALUE;

TLSContext::TLSContext():
profiler_userdata_( 0),
app_userdata_( 0)
{
}

TLSContext::~TLSContext()
{
}

bool TLSContext::init_tls_support()
{
	if( ACE_OS::thr_keycreate( &tls_key_, 0) != 0)
		return false;
	else
		return true;
}

void TLSContext::uninit_tls_support()
{
	if( tls_key_ != NO_INITVALUE)
		ACE_OS::thr_keyfree( tls_key_);
	tls_key_ =NO_INITVALUE;
}

bool TLSContext::init_tlscontext()
{
	if( TLSContext::tls_key_ == NO_INITVALUE)
		return false;

	return ACE_OS::thr_setspecific( TLSContext::tls_key_, (void*)this) == 0;
}

void TLSContext::uninit_tlscontext()
{
	if( TLSContext::tls_key_ != NO_INITVALUE)
		ACE_OS::thr_setspecific( TLSContext::tls_key_, (void*)0);
}

void* TLSContext::get_profiler_userdata()
{
	void* pdata =0;
	ACE_OS::thr_getspecific( tls_key_, &pdata);
	ACE_ASSERT( pdata != 0);

	TLSContext* pThis =( TLSContext*)pdata;
	return pThis->profiler_userdata_;
}

void TLSContext::set_profiler_userdata( void* p)
{
	void* pdata =0;
	ACE_OS::thr_getspecific( tls_key_, &pdata);
	ACE_ASSERT( pdata != 0);

	TLSContext* pThis =( TLSContext*)pdata;
	pThis->profiler_userdata_ =p;
}

void* TLSContext::get_app_userdata()
{
	void* pdata =0;
	ACE_OS::thr_getspecific( tls_key_, &pdata);
	ACE_ASSERT( pdata != 0);

	TLSContext* pThis =( TLSContext*)pdata;
	return pThis->app_userdata_;
}

void TLSContext::set_app_userdata( void* p)
{
	void* pdata =0;
	ACE_OS::thr_getspecific( tls_key_, &pdata);
	ACE_ASSERT( pdata != 0);

	TLSContext* pThis =( TLSContext*)pdata;
	pThis->app_userdata_ =p;
}
