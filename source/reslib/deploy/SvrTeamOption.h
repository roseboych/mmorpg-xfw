/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SVRTEAMOPTION__H__
#define __SVRTEAMOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include "reslib/deploy/CTSOption.h"
#include "reslib/deploy/GTSOption.h"
#include "reslib/deploy/DPXOption.h"
#include "reslib/deploy/CSSOption.h"
#include "reslib/deploy/INSCellOption.h"

#include <string>
#include <list>
#include <map>

/**
* @class SvrTeamOption
* 
* @brief 服务器组配置信息
**/
class SvrTeamOption
{
public:
	typedef NS_STL::map< S_INT_32, GTSOption*>		GTSOPTION_MAP;
	typedef NS_STL::map< S_INT_32, CSSOption*>		CSSOPTION_MAP;
	typedef NS_STL::map< S_INT_32, INSCellOption*>	INSCELLOPTION_MAP;
	
public:
	SvrTeamOption();
	~SvrTeamOption();

	CTSOption* get_ctsoption();

	DPXOption* get_dpxoption();

	/**
	* 根据sn获取gts配置信息
	* @param sn
	* @return
	**/
	GTSOption* get_gtsoption( S_INT_32 sn);

	/**
	* 获取所有的gts配置信息
	* @param ret
	**/
	void get_gtsoptions( NS_STL::list<GTSOption*>& ret);

	/**
	* 获取gts的数量
	**/
	S_INT_32 get_gtsnums();

	/**
	* 根据sn获取css配置信息
	* @param sn
	* @return
	**/
	CSSOption*	get_cssoption( S_INT_32 sn);

	/**
	* 获取所有的css配置信息
	* @param ret
	**/
	void		get_cssoptions( NS_STL::list<CSSOption*>& ret);

	CSSOption*	get_cssoptbymapid( S_INT_32 mid);

	/**
	* 获取css的数量
	**/
	S_INT_32	get_cssnums();

	S_INT_32		get_inscellnums();
	INSCellOption*	get_inscellopt( S_INT_32 cellid);

	/**
	* 检查配置的有效性
	* @return
	**/
	bool check_validate();

	void build_config();

protected:
	bool check_gtsoption();

	bool check_svrteamoption();

	void release();

public:
	//服务器组编号
	S_INT_32	sn_;
	//支持的最大玩家数
	S_INT_32	maxuser_;

	//cts配置信息
	CTSOption			ctsopt_;
	//gts配置信息
	GTSOPTION_MAP		gtss_;
	//dpx配置信息
	DPXOption			dpxopt_;
	//css配置信息
	CSSOPTION_MAP		csss_;
	//副本实例配置信息
	INSCELLOPTION_MAP	inscells_;

	//系统分配的序列号
	S_INT_32	server_index_;
};

#endif	//__SVRTEAMOPTION__H__
