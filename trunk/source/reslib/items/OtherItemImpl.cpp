/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/OtherItemImpl.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

OtherItemImpl::OtherItemImpl():
inherit()
{
}

bool OtherItemImpl::load_iteminfo( TiXmlElement* e)
{
	if( !inherit::load_iteminfo( e))
		return false;

	return true;
}

void OtherItemImpl::calcuate_playerproperty( IPlayerProperty* prop)
{
}

bool OtherItemImpl::can_buy_gm()
{
	return false;
}

bool OtherItemImpl::can_buy_rmb()
{
	return false;
}

bool OtherItemImpl::can_sale_gm()
{
	return false;
}

bool OtherItemImpl::can_sale_rmb()
{
	return false;
}
