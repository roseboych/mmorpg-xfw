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
	typedef NS_STL::list<int>	MAPS_LIST;
	typedef NS_STL::map<int, INSOption*>	INSOPT_MAP;

public:
	CSSOption();
	~CSSOption();

	/**
	* 获取支持的最大玩家数
	* @return
	**/
	int get_maxuser();

	/**
	* 需要打开的地图
	* @return
	**/
	int get_mapsize();

	int get_instancesize();

	/**
	* 检查给出的地图是否在css服务器中
	* @param mid
	* @return
	**/
	bool test_mapincss( int mid);

	INSOption* get_insoption( int sn);

	void get_insoptions( NS_STL::list<INSOption*>& ret);

protected:
	void release();

public:
	//css的编号
	int				sn_;
	//服务器地址和端口
	NS_STL::string	ip_;
	int				port_;

	//需要加载的地图
	MAPS_LIST	mapids_;
	//副本配置
	INSOPT_MAP	inss_;

	//所在的服务器组
	SvrTeamOption*	svrteamopt_;

	//系统分配的序列号
	int			server_index_;
};

#endif	//__CSSOPTION__H__
