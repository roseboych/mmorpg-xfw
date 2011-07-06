/**
* dpx application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYERCACHE__H__
#define __PLAYERCACHE__H__

#include "../dpx_pre.h"
#include <prolib/Pro_all.h>

#include "ChrListInfo.h"

/**
* @class PlayerCache
* 
* @brief 玩家需要缓存的数据
**/
class PlayerCache
{
public:
	typedef NS_STL::vector<ChrListInfo*>	CHRLIST_VECTOR;
	typedef NS_STL::map<S_INT_32, PRO::equipment_item*>	EQUIPITEMS_MAP;
	typedef NS_STL::map<S_INT_32, PRO::petdata_item*>	PETDATA_MAP;
	typedef NS_STL::map<S_INT_32, PRO::bufferdata_item*>	BUFFERDATA_MAP;

public:
	PlayerCache();
	virtual ~PlayerCache();

	//角色列表缓存
	ChrListInfo* is_chrinlist( S_INT_32 cid);
	void rm_chrinlist( S_INT_32 cid);

	/**
	* 加载角色数据
	* @param cid
	**/
	void chrdata_load( S_INT_32 cid);

protected:
	/**
	* 释放角色缓冲列表
	**/
	void clear_chrlist();
	void clear_itemlist();
	void clear_petlist();
	void clear_bufferlist();

public:
	//玩家数据缓冲
	//角色列表
	CHRLIST_VECTOR	chrs_;
	bool	chrs_init_;

	//是否加载完了数据
	bool	is_chrload_;
	//基本信息
	PRO::character_baseinfo	baseinfo_;
	//物品信息
	EQUIPITEMS_MAP	items_;
	//宠物信息
	PETDATA_MAP		pets_;
	//buffer信息
	BUFFERDATA_MAP	buffers_;
};

#endif	//__PLAYERCACHE__H__
