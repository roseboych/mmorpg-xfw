/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "ControllerLink.h"

#include "../RGSModule.h"

ControllerLink::ControllerLink():
is_login_( false),
initialized_( false),
login_failed_nums_( 0)
{
}

void ControllerLink::send_protocol( BasicProtocol* p)
{
	service_.append_write_msg( p);
}

bool ControllerLink::initialize_link()
{
	ACE_Guard<ACE_Thread_Mutex> mon( mutex_);

	if( initialized_)
		return false;

	initialized_ =true;
	is_login_ =false;
	login_failed_nums_ =0;

#ifdef CORELIB_WIN32
	service_.proactor( RGSMODULE->proactor());
#else
	service_.reactor( RGSMODULE->reactor());
#endif

	return initialized_;
}

void ControllerLink::reset()
{
	ACE_Guard<ACE_Thread_Mutex> mon( mutex_);

	this->is_login_ =false;
	this->initialized_ =false;
	login_failed_nums_ =0;

	service_.close_service();
}

bool ControllerLink::finish_login( bool succ)
{
	if( !initialized_)
		return false;

	if( succ)
	{
		this->is_login_ =true;
		login_failed_nums_ =0;
		return true;
	}
	else
	{
		this->is_login_ =false;
		++login_failed_nums_;

		if( login_failed_nums_ > 3)
			this->reset();

		return false;
	}
}

bool ControllerLink::is_login()
{
	return (initialized_ && is_login_);
}
