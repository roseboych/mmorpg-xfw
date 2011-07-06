/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/MerchandiseItemImpl.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

MerchandiseItemImpl::MerchandiseItemImpl():
inherit()
{
}

bool MerchandiseItemImpl::load_iteminfo( TiXmlElement* e)
{
	if( !inherit::load_iteminfo( e))
		return false;

	return true;
}

void MerchandiseItemImpl::calcuate_playerproperty( IPlayerProperty* prop)
{
}

bool MerchandiseItemImpl::can_buy_gm()
{
	return false;
}

bool MerchandiseItemImpl::can_buy_rmb()
{
	return false;
}

bool MerchandiseItemImpl::can_sale_gm()
{
	return false;
}

bool MerchandiseItemImpl::can_sale_rmb()
{
	return false;
}

