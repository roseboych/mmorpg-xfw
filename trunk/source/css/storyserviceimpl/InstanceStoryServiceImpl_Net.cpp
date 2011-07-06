/**
* css application
*
* @category		Transcript Story Service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstanceStoryServiceImpl.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "../CSSModule.h"

USE_PROTOCOL_NAMESPACE;

void InstanceStoryServiceImpl::gts_instenter_req( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}

void InstanceStoryServiceImpl::cts_instenter_ack( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}
