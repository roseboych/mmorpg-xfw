/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SCENEMANAGEPANEIMPL__H__
#define __SCENEMANAGEPANEIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../../css_pre.h"

#include <ace/Min_Max.h>
#include <ace/Auto_Ptr.h>

#include <prolib/Pro_all.h>

#include "../BaseSceneManage.h"
#include "scenepane_pre.h"

#include "../../character/UnitBase.h"
#include "../../character/Player.h"
#include "MapPane.h"

/**
* @class BaseSceneManage
* 
* @brief 场景管理基类
**/
class SceneManagePaneImpl : public BaseSceneManage
{
	typedef BaseSceneManage inherit;
public:
	SceneManagePaneImpl( BaseStoryService* pService);
	virtual ~SceneManagePaneImpl();

	virtual bool init_scenemanage();
	virtual void uninit_scenemanage();

	virtual void scene_aoi_ticker( const ACE_UINT64& tnow);

	/**
	* 在可视范围内广播协议
	* @param sender	发送人
	* @param p	协议内容
	* @param includesender	是否发送给本人
	**/
	virtual void broadcast_inview( UnitBase* sender, BasicProtocol* p, bool includesender =false);

	/**
	* 移动对象进入地图
	* @param pobj
	**/
	virtual void unitobj_entermap( UnitBase* pobj);

	/**
	* 移动对象离开地图
	* @param pobj
	* @param notify	true:发送离开通知
	**/
	virtual void unitobj_leavemap( UnitBase* pobj);

	/**
	* 移动对象移动到指定坐标
	* @param pobj
	* @param pos
	* @param face
	**/
	virtual void unitobj_moveto( Player* pobj, coordinate_pointer& pos, S_FLOAT_32& face);

	/**
	* 通知视野范围内的所有玩家信息
	**/
	virtual void notify_allobjs_inview( Player* user);

	/**
	* 发送in,out通知
	**/
	virtual void broadcast_inout_notify();

public:
	/**
	* 根据行号和列号获取格子描述
	* @param column
	* @param row
	* @return MapPane*
	**/
	inline MapPane* get_mappane( S_INT_32 column, S_INT_32 row);

	/**
	* 根据x,y坐标获取pane
	* @param x
	* @param y
	* @return
	**/
	inline MapPane* get_mappanebyxy( S_FLOAT_32 x, S_FLOAT_32 y);

	/**
	* 获取指定玩家的可视区域
	* @param c	玩家所在的column
	* @param r	玩家所在的row
	* @param mp
	**/
	inline void get_playerviewpanes( S_INT_32 c, S_INT_32 r, NS_STL::vector<MapPane*>& mp);
	inline void get_playerviewpanes( S_INT_32 x1, S_INT_32 y1, S_INT_32 x2, S_INT_32 y2, NS_STL::vector<MapPane*>& mp);

	/**
	* 定时计算活动对象的的in,out动作
	* @param tnow
	**/
	void calcuate_unitobj_inoutview( const ACE_UINT64& tnow);

protected:
	/**
	* 进入和退出视野计算
	**/
	void calcuate_unit_inview_tmp();
	void calcuate_unit_outview_tmp();

protected:
	//格子数
	ACE_Auto_Array_Ptr<MapPane>	panes_;
	//格子的列数和行数
	S_INT_32	pane_columns_;
	S_INT_32	pane_rows_;
	S_INT_32	pane_column_min_, pane_column_max_;
	S_INT_32	pane_row_min_, pane_row_max_;
	//地图的宽和高
	S_INT_32	map_width_;
	S_INT_32	map_height_;

};

#include "SceneManagePaneImpl.inl"

#endif	//__SCENEMANAGEPANEIMPL__H__