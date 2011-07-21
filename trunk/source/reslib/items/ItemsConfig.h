/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __ITEMSCONFIG__H__
#define __ITEMSCONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <corelib/script/ScriptContext.h>

#include "reslib/items/ItemInfoBase.h"

class TiXmlElement;

/**
* @class ItemsConfig
* 
* @brief 物品资源信息管理
**/
class ItemsConfig
{
	typedef NS_STL::map< int, ItemInfoBase*>	ITEMS_MAP;
public:
	ItemsConfig();
	~ItemsConfig();

	/**
	* 初始化物品资源
	* @return
	**/
	bool load_itemconfig();

	/**
	* 注册所有的gamesetting脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	ItemInfoBase* get_iteminfobyid( int itemid);

	int	get_itemsize(){ return (int)items_.size();}

protected:
	void release();

private:
	//系统支持的所有物品
	ITEMS_MAP	items_;
};

#define ITEMRES_CFG		ACE_Singleton< ItemsConfig, ACE_Null_Mutex>::instance()

#endif	//__ITEMSCONFIG__H__
