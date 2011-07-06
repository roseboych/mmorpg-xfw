/**
* css application
*
* @category		player item
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MYITEMINFO__H__
#define __MYITEMINFO__H__

#include "../css_pre.h"

#include <reslib/items/ItemInfoBase.h>
#include <prolib/datastore/character_define.h>

class IPlayerProperty;

/**
* @class MyItemInfo
* 
* @brief 玩家物品信息
**/
class MyItemInfo
{
public:
	MyItemInfo( PRO::equipment_item& info, ItemInfoBase* p);

	inline S_INT_32 get_itemid();
	inline S_INT_32 get_itemcode();

	/**
	* 是否已经装备
	* @return
	**/
	inline bool in_avatar();

	/**
	* 叠加属性
	* @param prop
	**/
	void calcuate_playerproperty( IPlayerProperty* prop);

	inline PRO::equipment_item& get_itemdata();

protected:
	//物品信息
	PRO::equipment_item	itemdata_;
	//物品配置信息
	ItemInfoBase*	item_conf_;
};

#include "MyItemInfo.inl"

#endif	//__MYITEMINFO__H__
