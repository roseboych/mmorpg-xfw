/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __UNITBASE__H__
#define __UNITBASE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"

#include <prolib/datastore/character_define.h>
#include <reslib/world/res_struct_def.h>
#include "UnitEventListener.h"

#include <list>

struct lua_State;
class ActorSceneExt;

/**
* @class UnitBase
* 
* @brief 移动对象的基类
**/
class UnitBase
{
	typedef NS_STL::list<UnitEventListener*>	UNITEVENT_LISTENER_LIST;
public:
	UnitBase();
	virtual ~UnitBase();

	/**
	* unit对象的每帧计算入口
	* @param tnow
	**/
	virtual void tick( const ACE_UINT64& tnow){}

	/**
	* 重置资源
	**/
	virtual void reset();

	/**
	* 获取移动对象的唯一索引
	**/
	virtual S_INT_32 get_id(){ return NO_INITVALUE;}
	inline S_INT_64 get_uuid();

	/**
	* 是否初始化完成
	**/
	virtual bool is_initfinish(){ return false;}

	/**
	* 获取对象的类型
	**/
	virtual ACTOR_TYPE_ENUM get_actortype(){ return ACTOR_TYPE_NONE;}

	/**
	* 是否能被指定对象攻击
	* 仅仅检查类型
	* @param pattach 发起攻击的对象
	* @return
	**/
	virtual bool can_beattacked( UnitBase* pattack){ return false;}

	/**
	* 活动对象，坐标管理
	**/
	inline virtual void set_position( S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z){}
	inline virtual void set_position( coordinate_pointer& p){}
	inline virtual void set_positionx( S_FLOAT_32 v){}
	inline virtual void set_positiony( S_FLOAT_32 v){}
	inline virtual void set_positionz( S_FLOAT_32 v){}
	inline virtual coordinate_pointer get_position(){ coordinate_pointer c; return c;}
	inline virtual S_FLOAT_32 get_positionx(){ return NO_INITVALUE;}
	inline virtual S_FLOAT_32 get_positiony(){ return NO_INITVALUE;}
	inline virtual S_FLOAT_32 get_positionz(){ return NO_INITVALUE;}
	inline virtual void set_facing( S_FLOAT_32 v){}
	inline virtual S_FLOAT_32 get_facing(){ return NO_INITVALUE;}

	/**
	* 移动到指定位置
	* @param pos
	* @param rot
	* @param vec
	**/
	virtual void moveto( coordinate_pointer& pos, S_FLOAT_32& face){}

	template< class T>
	T* get_scene_ext(){ return (T*)scene_ext_;}

public:
	//-------------------------unit event --------------------------
	void regist_eventlistener( UnitEventListener* lnsr);
	void unregist_eventlistener( UnitEventListener* lnsr);
	void trigger_unitentityreset();

public:
	/**
	* 脚本注册
	**/
	static void script_regist( lua_State* l);

protected:
	//场景管理扩展信息
	ActorSceneExt*	scene_ext_;

	//事件监听器
	UNITEVENT_LISTENER_LIST		event_listener_;
};

#include "UnitBase.inl"

#endif	//__UNITBASE__H__
