/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __EQUIPMENTITEMIMPL__H__
#define __EQUIPMENTITEMIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/items/ItemInfoBase.h"

struct lua_State;
class IPlayerProperty;

/**
* @class EquipmentItemImpl
* 
* @brief 装备物品资源信息
**/
class EquipmentItemImpl : public ItemInfoBase
{
	typedef ItemInfoBase inherit;
public:
	EquipmentItemImpl();

	virtual bool load_iteminfo( TiXmlElement* e);
	virtual void calcuate_playerproperty( IPlayerProperty* prop);

	virtual bool can_equipment(){ return true;}
	virtual char get_equippos(){ return avatar_pos_;}

	virtual bool can_stack(){ return false;}

	virtual bool can_buy_gm();
	virtual bool can_buy_rmb();

	virtual int get_buy_gm(){ return gmbuy_;}
	virtual int get_buy_rmb(){ return rmbuy_;}

	virtual bool can_sale_gm();
	virtual bool can_sale_rmb();

	virtual int get_sale_gm(){ return gmsale_;}
	virtual int get_sale_rmb(){ return rmsale_;}

	virtual bool is_taskitem(){ return false;}

	bool  is_twohande(){ return twohande_;}
	short get_level(){ return level_;}

public:
	static void script_regist( lua_State* l);

protected:
	//装备位置
	char		avatar_pos_;
	//装备等级
	short		level_;
	//1:双手武器 0:单手武器
	bool		twohande_;

	//对player的属性影响
	//力量force，敏捷agile，耐力endurance，智力intellect，精神spirit，护甲armor
	int	force_, agile_, endurance_, intellect_, spirit_, armor_;

	//买 gmbuy:游戏币 rmbuy:人民币 卖 gmsale:游戏币 rmsale:人民币 (-1:不支持)
	int	gmbuy_, rmbuy_, gmsale_, rmsale_;
};

#endif	//__EQUIPMENTITEMIMPL__H__
