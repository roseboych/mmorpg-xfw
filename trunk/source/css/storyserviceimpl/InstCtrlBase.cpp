/**
* css application
*
* @category		transcript service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstCtrlBase.h"

#include "InstanceStoryServiceImpl.h"

InstCtrlBase::InstCtrlBase( InstanceStoryServiceImpl* iss):
inst_service_( iss),
is_initialized_( false)
{
}

InstCtrlBase::~InstCtrlBase()
{

}

void InstCtrlBase::init_ctrl()
{
	if( is_initialized_)
		return;

	this->is_initialized_ =true;
}

bool InstCtrlBase::tick( const ACE_UINT64& now_t)
{
	if( !is_initialized_)
		return false;

	return true;
}
