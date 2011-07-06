/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/ItemInfoBase.h"

#include <corelib/xml/XmlUtil.h>
#include "reslib/IPlayerProperty.h"

ItemInfoBase::ItemInfoBase():
itemid_( NO_INITVALUE)
{
}

bool ItemInfoBase::load_iteminfo( TiXmlElement* e)
{
	item_desc_ =XmlUtil::GetXmlAttrStr( e, "desc", "");

	return true;
}
