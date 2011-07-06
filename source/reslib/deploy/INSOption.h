/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __INSOPTION__H__
#define __INSOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <string>
#include <list>

class CSSOption;
class INSCellOption;

/**
* @class INSOption
* 
* @brief 游戏和副本服务器配置
**/
class INSOption
{
public:
	typedef NS_STL::list<INSCellOption*>	INSCELL_LIST;

public:
	INSOption();
	~INSOption();

public:
	//ins在world中的编号
	int sn_;

	//实例数量
	int nums_;

	//分配的cell信息
	INSCELL_LIST	inscells_;

	//所在的服务器组
	CSSOption*	cssopt_;
};

#endif	//__CSSOPTION__H__
