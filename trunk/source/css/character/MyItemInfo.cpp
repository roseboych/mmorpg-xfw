/**
* css application
*
* @category		player item
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "MyItemInfo.h"

#include <reslib/IPlayerProperty.h>

MyItemInfo::MyItemInfo( PRO::equipment_item& info, ItemInfoBase* p):
item_conf_( p)
{
	this->itemdata_ =info;
}

void MyItemInfo::calcuate_playerproperty( IPlayerProperty* prop)
{
	item_conf_->calcuate_playerproperty( prop);
}
