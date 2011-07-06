/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/ConsumeItemImpl.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

ConsumeItemImpl::ConsumeItemImpl():
inherit()
{
	stack_ =0;
	gmbuy_ =rmbuy_ =gmsale_ =rmsale_ =-1;
}

bool ConsumeItemImpl::load_iteminfo( TiXmlElement* e)
{
	if( !inherit::load_iteminfo( e))
		return false;

	stack_ =(short)XmlUtil::GetXmlAttrInt( e, "stack", 0);

	TiXmlElement* saleprops =e->FirstChildElement( "sale");
	if( saleprops)
	{
		gmbuy_ =XmlUtil::GetXmlAttrInt( saleprops, "gmbuy", -1);
		rmbuy_ =XmlUtil::GetXmlAttrInt( saleprops, "rmbuy", -1);
		gmsale_ =XmlUtil::GetXmlAttrInt( saleprops, "gmsale", -1);
		rmsale_ =XmlUtil::GetXmlAttrInt( saleprops, "rmsale", -1);
	}

	return true;
}

void ConsumeItemImpl::calcuate_playerproperty( IPlayerProperty* prop)
{
}

bool ConsumeItemImpl::can_buy_gm()
{
	return gmbuy_ != -1;
}

bool ConsumeItemImpl::can_buy_rmb()
{
	return rmbuy_ != -1;
}

bool ConsumeItemImpl::can_sale_gm()
{
	return gmsale_ != -1;
}

bool ConsumeItemImpl::can_sale_rmb()
{
	return rmsale_ != -1;
}
