/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYER__H__
#define __PLAYER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"

#include <ace/Thread_Mutex.h>
#include <prolib/BasicProtocol.h>
#include <boost/pool/object_pool.hpp>

#include "UnitBase.h"
#include "PlayerData.h"
#include "PlayerActionSelector.h"
#include "UnitEventListener.h"

class GTSLink;

/**
* @class Player
* 
* @brief 玩家信息
**/
class Player : public UnitBase, public PlayerData, public UnitEventListener
{
	typedef UnitBase inherit;
public:
	Player();
	virtual ~Player();

	//---------------------------------UnitBase virtual function---------------------------//
	virtual void tick( const ACE_UINT64& tnow);

	/**
	* @see UnitBase::reset()
	**/
	virtual void reset();

	/**
	* @see UnitBase::get_id()
	**/
	virtual S_INT_32 get_id();

	virtual ACTOR_TYPE_ENUM get_actortype(){ return ACTOR_TYPE_PLAYER;}

	bool is_unused(){ return status_ == PLAYERSTATUS_NONE;}

	inline virtual void set_position( S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z);
	inline virtual void set_position( coordinate_pointer& p);
	inline virtual void set_positionx( S_FLOAT_32 v);
	inline virtual void set_positiony( S_FLOAT_32 v);
	inline virtual void set_positionz( S_FLOAT_32 v);
	inline virtual coordinate_pointer get_position();
	inline virtual S_FLOAT_32 get_positionx();
	inline virtual S_FLOAT_32 get_positiony();
	inline virtual S_FLOAT_32 get_positionz();
	inline virtual void set_facing( S_FLOAT_32 v);
	inline virtual S_FLOAT_32 get_facing();

	inline bool is_needupdatepos( const ACE_UINT64& tnow);

	/**
	* @see UnitBase::moveto( coordinate_pointer& pos, S_FLOAT_32& face)
	**/
	virtual void moveto( coordinate_pointer& pos, S_FLOAT_32& face);

	/**
	* 属性加载完成
	* @return
	**/
	virtual bool is_initfinish(){ return status_ > PLAYERSTATUS_REGIST;}

	virtual bool can_beattacked( UnitBase* pattack);

public:
	/**
	* 战斗对象相关
	**/
	void set_fighttargetobj( UnitBase* obj);
	UnitBase* get_fighttargetobj(){ return this->fight_target_obj_;}

	virtual void event_unitentity_reset( UnitBase* pobj);

public:
	/**
	* 注册玩家
	* @param uid
	* @param stmp
	* @param chrid
	**/
	void regist( S_INT_32 uid, S_TIMESTAMP stmp, S_INT_32 chrid);
	void regist( S_INT_32 gid, S_INT_32 uid, S_TIMESTAMP stmp, S_INT_32 chrid);
	/**
	* 玩家数据初始化
	**/
	void props_load( PRO::Pro_ChrLoad_ack* ack);
	void regist_fin( PRO::Pro_ChrFin_NTF* fin);

	/**
	* 根据属性计算出完整的属性
	**/
	void calcuate_playerproperty();

	/**
	* 断线保持重发
	**/
	void lnkkeep_reload();

	/**
	* 发送到gts服务器
	* @param p
	**/
	void send_to_gts( BasicProtocol* p);

	/**
	* 脚本系统绑定
	**/
	static void script_regist( lua_State* l);

	/**
	* 构建一个玩家类
	**/
	static Player*	create_player();
	static void		destroy_player( Player* p);

public:
	//-----------------------------------skill对外接口封装函数------------------------------------
	/**
	* 学习一个技能
	* @param skid
	**/
	void skill_study( S_INT_32 skid);

	/**
	* 使用一个技能
	* #引起player状态变化的一个入口
	* @param skid		技能编号
	* @param targetid	目标id
	* @param st			玩家的当前状态
	**/
	void skill_used( S_INT_32 skid, S_INT_64 targetid, CHRSTATE_TYPE_ENUM st);

public:
	//---------------------------------其他函数---------------------------------------------------
	/**
	* 进入副本
	**/
	void begin_entertranscript();
	void end_entertranscript();

public:
	//player state machine
	PlayerActionSelector	action_selector_;

	//玩家状态
	PLAYERSTATUS_TYPE_ENUM	status_;
	//帐号id
	uuid_session	uuid_;
	//关联的gts链接
	GTSLink*		gts_link_;
	//是否正在进入副本
	bool			is_entertranscript_;
	//在副本中
	bool			is_in_inst;

	//上次更新坐标时间
	ACE_UINT64		t_lastupdate_;
	S_FLOAT_32		lastposx_, lastposy_, lastposz_, lastfacing_;

protected:
	//当前的攻击目标
	UnitBase*	fight_target_obj_;

public:
	//玩家全局索引，系统定义
	S_INT_32	global_index_;

	//玩家分配pool
	static boost::object_pool<Player>	player_pools_;
	static ACE_Thread_Mutex				pool_mutex_;
};

#include "Player.inl"

#endif	//__PLAYER__H__
