/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CONSUMEITEMIMPL__H__
#define __CONSUMEITEMIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/items/ItemInfoBase.h"

struct lua_State;
class IPlayerProperty;

/**
* @class ConsumeItemImpl
* 
* @brief 消费品资源信息
**/
class ConsumeItemImpl : public ItemInfoBase
{
	typedef ItemInfoBase inherit;
public:
	ConsumeItemImpl();

	virtual bool load_iteminfo( TiXmlElement* e);
	virtual void calcuate_playerproperty( IPlayerProperty* prop);

	virtual bool can_equipment(){ return false;}

	virtual bool can_stack(){ return stack_ > 0;}
	virtual short get_stack_num(){ return stack_;}

	virtual bool can_buy_gm();
	virtual bool can_buy_rmb();

	virtual int get_buy_gm(){ return gmbuy_;}
	virtual int get_buy_rmb(){ return rmbuy_;}

	virtual bool can_sale_gm();
	virtual bool can_sale_rmb();

	virtual int get_sale_gm(){ return gmsale_;}
	virtual int get_sale_rmb(){ return rmsale_;}

	virtual bool is_taskitem(){ return false;}

public:
	static void script_regist( lua_State* l);

protected:
	//0不允许堆叠，堆叠个数
	short	stack_;

	//买 gmbuy:游戏币 rmbuy:人民币 卖 gmsale:游戏币 rmsale:人民币 (-1:不支持)
	int	gmbuy_, rmbuy_, gmsale_, rmsale_;
};

#endif	//__CONSUMEITEMIMPL__H__
