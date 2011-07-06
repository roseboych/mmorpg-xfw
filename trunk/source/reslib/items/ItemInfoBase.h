/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __ITEMINFOBASE__H__
#define __ITEMINFOBASE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <string>

struct lua_State;
class IPlayerProperty;
class TiXmlElement;

/**
* @class ItemInfoBase
* 
* @brief 物品资源信息
**/
class ItemInfoBase
{
public:
	ItemInfoBase();

	/**
	* 从xml加载item配置信息
	* @param e
	* @return
	**/
	virtual bool load_iteminfo( TiXmlElement* e);

	virtual void calcuate_playerproperty( IPlayerProperty* prop){}

public:
	//---------------------------------属性对外接口----------------------------------
	/**
	* itemid全局唯一属性
	**/
	int get_itemid(){ return itemid_;}
	void set_itemid( int id){ itemid_ =id;}

	NS_STL::string get_itemdesc(){ return item_desc_;}
	void set_itemdesc( const char* v){ if( v) item_desc_ =v;}

	/**
	* 是否能够装备
	* @return
	**/
	virtual bool can_equipment(){ return false;}
	virtual char get_equippos(){ return -1;}

	/**
	* 是否能够堆叠
	* @return
	**/
	virtual bool can_stack(){ return false;}
	virtual short get_stack_num(){ return -1;}

	/**
	* 是否能够买
	* @return
	**/
	virtual bool can_buy_gm(){ return false;}
	virtual bool can_buy_rmb(){ return false;}

	virtual int get_buy_gm(){ return 0;}
	virtual int get_buy_rmb(){ return 0;}

	/**
	* 是否能够卖掉
	**/
	virtual bool can_sale_gm(){ return false;}
	virtual bool can_sale_rmb(){ return false;}

	virtual int get_sale_gm(){ return -1;}
	virtual int get_sale_rmb(){ return -1;}

	/**
	* 是否是任务物品
	* @return
	**/
	virtual bool is_taskitem(){ return false;}

public:
	/**
	* 脚本环境注册
	**/
	static void script_regist( lua_State* l);

protected:
	//itemid
	int				itemid_;
	//item文字描述
	NS_STL::string	item_desc_;
};

#endif	//__ITEMINFOBASE__H__
