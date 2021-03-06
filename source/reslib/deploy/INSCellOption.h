/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSCELLOPTION__H__
#define __INSCELLOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

class INSOption;
class CSSOption;
class CSSLink;

/**
* @class INSCellOption
* 
* @brief 副本服务器单个实例配置
**/
class INSCellOption
{
public:
	INSCellOption();

	int		get_cellid(){ return inscell_id_;}
	void	set_cellid( int v){ inscell_id_ =v;}

	//支持的地图编号
	int			get_mapid();
	int			get_cssserverid();
	CSSOption*	get_cssopt();

public:
	//实例编号全局唯一
	int			inscell_id_;
	//实例所在的副本配置
	INSOption*	ins_opt_;
};

#endif	//__INSCELLOPTION__H__
