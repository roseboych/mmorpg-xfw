/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BASESCENEMANAGE__H__
#define __BASESCENEMANAGE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"
#include <reslib/world/res_struct_def.h>

class BaseStoryService;
class UnitBase;
class Player;
class BasicProtocol;

/**
* @class BaseSceneManage
* 
* @brief 场景管理基类
**/
class BaseSceneManage
{
public:
	BaseSceneManage( BaseStoryService* pService);
	virtual ~BaseSceneManage();

	virtual bool init_scenemanage();
	virtual void uninit_scenemanage();

	/**
	* 场景管理定时aoi计算定时器
	* @param tnow
	**/
	virtual void scene_aoi_ticker( const ACE_UINT64& tnow) =0;

	/**
	* 在可视范围内广播协议
	* @param sender	发送人
	* @param p	协议内容
	* @param includesender	是否发送给本人
	**/
	virtual void broadcast_inview( UnitBase* sender, BasicProtocol* p, bool includesender =false) =0;

	/**
	* 移动对象进入地图
	* @param pobj
	**/
	virtual void unitobj_entermap( UnitBase* pobj) =0;

	/**
	* 移动对象离开地图
	* @param pobj
	* @param notify	true:发送离开通知
	**/
	virtual void unitobj_leavemap( UnitBase* pobj) =0;

	/**
	* 移动对象移动到指定坐标
	* @param pobj
	* @param pos
	* @param face
	**/
	virtual void unitobj_moveto( Player* pobj, coordinate_pointer& pos, S_FLOAT_32& face) =0;

	/**
	* 通知视野范围内的所有玩家信息
	**/
	virtual void notify_allobjs_inview( Player* user) =0;

	/**
	* 发送in,out通知
	**/
	virtual void broadcast_inout_notify() =0;

public:
	//坐标转化
	void world_to_local( S_FLOAT_32& x, S_FLOAT_32& y);
	void local_to_world( S_FLOAT_32& x, S_FLOAT_32& y);

protected:
	BaseStoryService*	owner_service_;
};

#endif	//__BASESCENEMANAGE__H__
