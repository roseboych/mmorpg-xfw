/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskParamBase.h"

#include "reslib/tasks/TaskDefaultImplParam.h"

#include <ace/Assert.h>

TaskParamBase* TaskParamBase::create( NS_STL::string clsname)
{
	clsname +="Param";
	if( clsname == TaskDefaultImplParam::get_classname())
		return FRAMEWK_NEW TaskDefaultImplParam();

	ACE_ASSERT( false && "task implementation param not regist");
	return 0;
}
