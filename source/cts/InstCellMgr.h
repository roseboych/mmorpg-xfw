/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSTCELLMGR__H__
#define __INSTCELLMGR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "cts_pre.h"

#include <ace/Auto_Ptr.h>
#include "InstCellState.h"
#include "InstCellMapGroup.h"

#include <map>

class SvrTeamOption;

/**
* @class InstCellMgr
* 
* @brief 副本状态管理
* 
**/
class InstCellMgr
{
public:
	typedef NS_STL::map< S_INT_32, InstCellState*>		INSTCELLSTATE_MAP;
	typedef NS_STL::map< S_INT_32, InstCellMapGroup*>	INSTCELLMAPGROUP_MAP;

public:
	InstCellMgr();
	~InstCellMgr();

	void initialize( SvrTeamOption* opt);

	/**
	* 根据副本地图id获取所有相关的副本实例
	* @param mapid
	* @return InstCellMapGroup*
	**/
	InstCellMapGroup*	get_instcellsbymapid( S_INT_32 mapid);

	InstCellState*		get_canenterinstcellbymapid( S_INT_32 mapid);

	InstCellState*		get_instcellbycellid( S_INT_32 cellid);

protected:

	void release();

private:
	ACE_Auto_Array_Ptr<InstCellState>	inst_cells_;
	S_INT_32							inst_cells_nums_;

	//状态分类
	INSTCELLSTATE_MAP		cell_states_;
	INSTCELLMAPGROUP_MAP	mapgroup_states_;
};


#endif	//__INSTCELLMGR__H__
