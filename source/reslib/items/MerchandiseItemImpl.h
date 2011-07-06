/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MERCHANDISEITEMIMPL__H__
#define __MERCHANDISEITEMIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/items/ItemInfoBase.h"

struct lua_State;
class IPlayerProperty;

/**
* @class MerchandiseItemImpl
* 
* @brief 商品资源信息
**/
class MerchandiseItemImpl : public ItemInfoBase
{
	typedef ItemInfoBase inherit;
public:
	MerchandiseItemImpl();

	virtual bool load_iteminfo( TiXmlElement* e);
	virtual void calcuate_playerproperty( IPlayerProperty* prop);

	virtual bool can_equipment(){ return false;}
	virtual bool can_stack(){ return false;}

	virtual bool can_buy_gm();
	virtual bool can_buy_rmb();

	virtual bool can_sale_gm();
	virtual bool can_sale_rmb();

	virtual bool is_taskitem(){ return false;}

public:
	static void script_regist( lua_State* l);
};

#endif	//__MERCHANDISEITEMIMPL__H__
