/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/TaskItemImpl.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

TaskItemImpl::TaskItemImpl():
inherit()
{
	stack_ =0;
}

bool TaskItemImpl::load_iteminfo( TiXmlElement* e)
{
	if( !inherit::load_iteminfo( e))
		return false;

	stack_ =(S_INT_16)XmlUtil::GetXmlAttrInt( e, "stack", 0);

	return true;
}

void TaskItemImpl::calcuate_playerproperty( IPlayerProperty* prop)
{
}

bool TaskItemImpl::can_buy_gm()
{
	return false;
}

bool TaskItemImpl::can_buy_rmb()
{
	return false;
}

bool TaskItemImpl::can_sale_gm()
{
	return false;
}

bool TaskItemImpl::can_sale_rmb()
{
	return false;
}

