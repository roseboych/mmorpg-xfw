/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __OTHERITEMIMPL__H__
#define __OTHERITEMIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/items/ItemInfoBase.h"

struct lua_State;
class IPlayerProperty;

/**
* @class OtherItemImpl
* 
* @brief 其他资源信息
**/
class OtherItemImpl : public ItemInfoBase
{
	typedef ItemInfoBase inherit;
public:
	OtherItemImpl();

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

#endif	//__OTHERITEMIMPL__H__
