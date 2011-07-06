/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/ItemsConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"
#include "reslib/items/EquipmentItemImpl.h"
#include "reslib/items/ConsumeItemImpl.h"
#include "reslib/items/MerchandiseItemImpl.h"
#include "reslib/items/TaskItemImpl.h"
#include "reslib/items/OtherItemImpl.h"

ItemsConfig::ItemsConfig()
{

}

ItemsConfig::~ItemsConfig()
{
	release();
}

void ItemsConfig::release()
{
	for( ITEMS_MAP::iterator iter =items_.begin(); iter != items_.end(); ++iter)
		delete iter->second;
	items_.clear();
}

bool ItemsConfig::load_itemconfig()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	NS_STL::string cnt =dsrc->get_txtfilecontent( "appdata/items", "items.xml");
	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();
	TiXmlElement* its =root->FirstChildElement( "equipment");
	if( its != 0)
	{
		for( TiXmlElement* e =its->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e, "itemid", NO_INITVALUE);
			if( itemid == NO_INITVALUE || get_iteminfobyid( itemid))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load items.xml error - item:%d redefine!", itemid);
				return false;
			}

			ItemInfoBase* pitem =STATICRES_NEW EquipmentItemImpl();
			pitem->set_itemid( itemid);
			pitem->load_iteminfo( e);

			items_[pitem->get_itemid()] =pitem;
		}
	}

	its =root->FirstChildElement( "consume");
	if( its != 0)
	{
		for( TiXmlElement* e =its->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e, "itemid", NO_INITVALUE);
			if( itemid == NO_INITVALUE || get_iteminfobyid( itemid))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load items.xml error - item:%d redefine!", itemid);
				return false;
			}

			ItemInfoBase* pitem =STATICRES_NEW ConsumeItemImpl();
			pitem->set_itemid( itemid);
			pitem->load_iteminfo( e);

			items_[pitem->get_itemid()] =pitem;
		}
	}

	its =root->FirstChildElement( "merchandise");
	if( its != 0)
	{
		for( TiXmlElement* e =its->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e, "itemid", NO_INITVALUE);
			if( itemid == NO_INITVALUE || get_iteminfobyid( itemid))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load items.xml error - item:%d redefine!", itemid);
				return false;
			}

			ItemInfoBase* pitem =STATICRES_NEW MerchandiseItemImpl();
			pitem->set_itemid( itemid);
			pitem->load_iteminfo( e);

			items_[pitem->get_itemid()] =pitem;
		}
	}

	its =root->FirstChildElement( "task");
	if( its != 0)
	{
		for( TiXmlElement* e =its->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e, "itemid", NO_INITVALUE);
			if( itemid == NO_INITVALUE || get_iteminfobyid( itemid))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load items.xml error - item:%d redefine!", itemid);
				return false;
			}

			ItemInfoBase* pitem =STATICRES_NEW TaskItemImpl();
			pitem->set_itemid( itemid);
			pitem->load_iteminfo( e);

			items_[pitem->get_itemid()] =pitem;
		}
	}

	its =root->FirstChildElement( "other");
	if( its != 0)
	{
		for( TiXmlElement* e =its->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e, "itemid", NO_INITVALUE);
			if( itemid == NO_INITVALUE || get_iteminfobyid( itemid))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load items.xml error - item:%d redefine!", itemid);
				return false;
			}

			ItemInfoBase* pitem =STATICRES_NEW OtherItemImpl();
			pitem->set_itemid( itemid);
			pitem->load_iteminfo( e);

			items_[pitem->get_itemid()] =pitem;
		}
	}

	return true;
}

ItemInfoBase* ItemsConfig::get_iteminfobyid( S_INT_32 itemid)
{
	ITEMS_MAP::iterator fiter =items_.find( itemid);
	if( fiter == items_.end())
		return 0;

	return fiter->second;
}

bool ItemsConfig::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	ItemInfoBase::script_regist( context.get_luastate());

	EquipmentItemImpl::script_regist( context.get_luastate());
	ConsumeItemImpl::script_regist( context.get_luastate());
	MerchandiseItemImpl::script_regist( context.get_luastate());
	TaskItemImpl::script_regist( context.get_luastate());
	OtherItemImpl::script_regist( context.get_luastate());

	return true;
}
