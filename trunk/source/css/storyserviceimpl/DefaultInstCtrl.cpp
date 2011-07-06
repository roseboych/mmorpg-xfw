/**
* css application
*
* @category		transcript service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "DefaultInstCtrl.h"

#include <reslib/deploy/INSCellOption.h>
#include "InstanceStoryServiceImpl.h"

DefaultInstCtrl::DefaultInstCtrl(InstanceStoryServiceImpl *iss):
InstCtrlBase( iss)
{
}

DefaultInstCtrl::~DefaultInstCtrl()
{
}

void DefaultInstCtrl::init_ctrl()
{
	if( is_initialized())
		return;

	inherit::init_ctrl();

	inst_service_->set_serverstate( TRANSCRIPT_SVRST_CANENTER);
}

S_BOOL DefaultInstCtrl::tick( const ACE_UINT64& now_t)
{
	return inherit::tick( now_t);
}
