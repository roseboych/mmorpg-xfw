/**
* reslib
*
* @category		��������
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/world/EnterInstParamBase.h"

#include "reslib/world/EnterInstTaskParam.h"
#include <ace/Assert.h>

EnterInstParamBase* EnterInstParamBase::create( NS_STL::string clsname)
{
	//����������������ʵ����֮���Param
	clsname +="Param";

	if( clsname == EnterInstTaskParam::get_classname())
		return FRAMEWK_NEW EnterInstTaskParam();

	ACE_ASSERT( false && "enter inst param not regist");
	return 0;
}
