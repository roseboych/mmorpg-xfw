/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __ACTORSCENEEXTPANE__H__
#define __ACTORSCENEEXTPANE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../../css_pre.h"
#include "../ActorSceneExt.h"
#include "scenepane_pre.h"
#include "../../character/UnitBase.h"

#include <map>

class MapPane;

/**
* @class ActorSceneExtPane
* 
* @brief Actor场景附加数据pane实现
**/
class ActorSceneExtPane : public ActorSceneExt
{
public:
	typedef ActorSceneExt inherit;
	typedef NS_STL::map< S_INT_64, UnitBase*>	UNITOBJECT_MAP;

public:
	ActorSceneExtPane( UnitBase* pOwner);

	virtual void reset();

public:
	bool init_unitobj( MapPane* pane);

	/**
	* 切换到新的mappane
	* @param newpane
	**/
	void change_inpane( MapPane* newpane);

	inline MapPane* get_curpane();

	//------------------------------------进出视野管理----------------------------------------//
	/**
	* in,out动作的计算
	* @param pctrl
	**/
	void viewinout_calculate();

	/**
	* 清除进出视野缓存
	**/
	inline void clear_viewinout_tmp();

	/**
	* 加入进入视野缓存
	* @param pobj
	**/
	inline void join_inview( UnitBase* pobj);

	/**
	* 加入退出视野缓存
	* @param pobj
	**/
	inline void join_outview( UnitBase* pobj);

	inline void change_oldpane();

	/**
	* 获取活动对象的移动区域
	* @param r_enter	pane_region[2]
	* @param r_exit		pane_region[2]
	* @return
	**/
	inline bool get_moveinregion( pane_region* r_enter);
	inline bool get_moveoutregion( pane_region* r_exit);

public:
	//进出视野使用的临时变量
	UNITOBJECT_MAP	in_view_objs_;
	UNITOBJECT_MAP	out_view_objs_;
	//原来的区域
	S_INT_32	oldpanex_, oldpaney_;
	//当前所在的pane
	MapPane*	cur_pane_;
};

#include "ActorSceneExtPane.inl"

#endif	//__ACTORSCENEEXTPANE__H__
