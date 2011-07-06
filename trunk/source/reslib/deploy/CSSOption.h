/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSOPTION__H__
#define __CSSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <string>
#include <list>
#include <map>

#include "reslib/deploy/INSOption.h"

class SvrTeamOption;

/**
* @class CSSOption
* 
* @brief 游戏和副本服务器配置
**/
class CSSOption
{
public:
	typedef NS_STL::list<S_INT_32>	MAPS_LIST;
	typedef NS_STL::map<S_INT_32, INSOption*>	INSOPT_MAP;

public:
	CSSOption();
	~CSSOption();

	/**
	* 获取支持的最大玩家数
	* @return
	**/
	S_INT_32 get_maxuser();

	/**
	* 需要打开的地图
	* @return
	**/
	S_INT_32 get_mapsize();

	S_INT_32 get_instancesize();

	/**
	* 检查给出的地图是否在css服务器中
	* @param mid
	* @return
	**/
	bool test_mapincss( S_INT_32 mid);

	INSOption* get_insoption( S_INT_32 sn);

	void get_insoptions( NS_STL::list<INSOption*>& ret);

protected:
	void release();

public:
	//css的编号
	S_INT_32		sn_;
	//服务器地址和端口
	NS_STL::string	ip_;
	S_INT_32		port_;

	//需要加载的地图
	MAPS_LIST	mapids_;
	//副本配置
	INSOPT_MAP	inss_;

	//所在的服务器组
	SvrTeamOption*	svrteamopt_;

	//系统分配的序列号
	S_INT_32 server_index_;
};

#endif	//__CSSOPTION__H__
