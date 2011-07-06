/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __TASKITEMIMPL__H__
#define __TASKITEMIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/items/ItemInfoBase.h"

struct lua_State;
class IPlayerProperty;

/**
* @class TaskItemImpl
* 
* @brief 任务物品资源信息
**/
class TaskItemImpl : public ItemInfoBase
{
	typedef ItemInfoBase inherit;
public:
	TaskItemImpl();

	virtual bool load_iteminfo( TiXmlElement* e);
	virtual void calcuate_playerproperty( IPlayerProperty* prop);

	virtual bool can_equipment(){ return false;}
	virtual bool can_stack(){ return stack_ > 0;}
	virtual short get_stack_num(){ return stack_;}

	virtual bool can_buy_gm();
	virtual bool can_buy_rmb();

	virtual bool can_sale_gm();
	virtual bool can_sale_rmb();

	virtual bool is_taskitem(){ return true;}

public:
	static void script_regist( lua_State* l);

protected:
	//0不允许堆叠，堆叠个数
	short	stack_;
};

#endif	//__TASKITEMIMPL__H__
