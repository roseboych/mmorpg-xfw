/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/EquipmentItemImpl.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

EquipmentItemImpl::EquipmentItemImpl():
inherit()
{
	force_ =agile_ =endurance_ =intellect_ =spirit_ =armor_ =0;
	gmbuy_ =rmbuy_ =gmsale_ =rmsale_ =-1;
}

bool EquipmentItemImpl::load_iteminfo( TiXmlElement* e)
{
	if( !inherit::load_iteminfo( e))
		return false;

	avatar_pos_ =(char)XmlUtil::GetXmlAttrInt( e, "pos", 0);
	level_ =(short)XmlUtil::GetXmlAttrInt( e, "level", 0);
	twohande_ =XmlUtil::GetXmlAttrBoolFromInt( e, "twohande", false);

	TiXmlElement* extprops =e->FirstChildElement( "props");
	if( extprops)
	{
		force_ =XmlUtil::GetXmlAttrInt( extprops, "force", 0);
		agile_ =XmlUtil::GetXmlAttrInt( extprops, "agile", 0);
		endurance_ =XmlUtil::GetXmlAttrInt( extprops, "endurance", 0);
		intellect_ =XmlUtil::GetXmlAttrInt( extprops, "intellect", 0);
		spirit_ =XmlUtil::GetXmlAttrInt( extprops, "spirit", 0);
		armor_ =XmlUtil::GetXmlAttrInt( extprops, "armor", 0);
	}

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

void EquipmentItemImpl::calcuate_playerproperty( IPlayerProperty* prop)
{
	prop->inc_force( force_);
	prop->inc_agile( agile_);
	prop->inc_endurance( endurance_);
	prop->inc_intellect( intellect_);
	prop->inc_spirit( spirit_);
	prop->inc_armor( armor_);
}

bool EquipmentItemImpl::can_buy_gm()
{
	return gmbuy_ != -1;
}

bool EquipmentItemImpl::can_buy_rmb()
{
	return rmbuy_ != -1;
}

bool EquipmentItemImpl::can_sale_gm()
{
	return gmsale_ != -1;
}

bool EquipmentItemImpl::can_sale_rmb()
{
	return rmsale_ != -1;
}
