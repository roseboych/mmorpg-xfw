/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASESTORYSERVICE__H__
#define __BASESTORYSERVICE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Auto_Ptr.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/core/TimerContainer.h>
#include <corelib/core/BasicCommand.h>
#include <reslib/world/StoryMapOption.h>

#include "scene/BaseSceneManage.h"
#include "character/Player.h"

#define CSS_GETPLAYER_FROMSTORY( usr, pro)						\
	Player* ##usr =get_player( ##pro->uuid_);					\
	if( ##usr == 0 || !##pro->same_session( ##usr->uuid_))		\
		return

/**
* @class BaseStoryService
* 
* @brief 游戏内容服务基类
**/
class BaseStoryService
{
public:
	typedef NS_STL::list<BasicCommand* >		COMMAND_LIST;
	typedef NS_STL::map< int, Player*>		ONLINEPLAYER_MAP;
	typedef NS_STL::map< S_INT_64, UnitBase*>	ONLINEENTITY_MAP;

public:
	BaseStoryService();
	virtual ~BaseStoryService();

	/**
	* 内容初始化
	* @param r
	* @return
	**/
	virtual bool init_service( StoryMapOption* opt);
	virtual void uninit_service();

	/**
	* 服务处理入口
	**/
	virtual void do_service( const ACE_UINT64& now_t);

	virtual bool is_mainstoryservice() =0;
	virtual bool is_instancestoryservice() =0;

	/**
	* 检查是否是支持该地图的服务
	* @param m
	* @return
	**/
	bool is_mapsupported( StoryMapOption* m){ return m == mapres_;}

	StoryMapOption*		get_storyoption(){ return mapres_;}
	BaseSceneManage*	get_scenemanage(){ return scene_manage_;}

	app::script::ScriptContext& get_scriptcontext(){ return script_context_;}

	/**
	* 命令队列处理函数
	**/
	void regist_netcmd( BasicCommand *p);
	BasicCommand* pop_one_cmd();

public:
	//------------------------------------------在线用户接口--------------------------------------------
	virtual Player* get_player( const BasicProtocol::internal_uuid& uuid) =0;
	/**
	* 玩家注册和注销
	**/
	virtual Player* player_regist( const BasicProtocol::internal_uuid& uuid, S_INT_32 chrid) =0;
	virtual void player_unregist( const BasicProtocol::internal_uuid& uuid) =0;
	virtual void player_unregist( Player* user) =0;

	void regist_to_onlineplayers( Player* user);
	void unregist_from_onlineplayers( Player* user);

	ONLINEPLAYER_MAP& get_onlineplayers(){ return online_players_;}

	void regist_to_onlineentity( UnitBase* pobj);
	void unregist_from_onlineentity( UnitBase* pobj);

	UnitBase* get_onlineentity( S_INT_64 uuid);

public:
	//------------------------------------------服务线程定时器------------------------------------------
	void contenttimer_reset(){ content_timer_.reset();}

	/**
	* 增加一个定时器
	* @param step
	* @param f
	* @return int 定时器句柄
	**/
	int add_contenttimer( int step, MODULETIMER_FUN_MAP f){ return content_timer_.add_timer( step, f);}

	/**
	* 清除一个定时器
	* @param tid
	**/
	void del_contenttimer( int tid){ content_timer_.del_timer( tid);}

	/**
	* 清除所有的定时器
	**/
	void delall_contenttimer(){ content_timer_.delall_timer();}

protected:
	/**
	* 移动对象进入退出定时器
	* @param tnow
	**/
	void unitobj_enterleave_timer( ACE_UINT64 tnow);

	void unitobj_tick_timer( const ACE_UINT64& tnow);

	void script_gc_timer( const ACE_UINT64& tnow);

protected:
	//脚本环境支持
	app::script::ScriptContext	script_context_;
	bool						is_support_script_;

	//服务线程定时器
	TimerContainer		content_timer_;
	//上一次处理时间
	S_TIMESTAMP			last_dotime_;

	//处理的地图配置
	StoryMapOption*		mapres_;

	//场景管理
	BaseSceneManage*	scene_manage_;
	//仅仅用来作为mainstory和instancestory的中间接口
	ONLINEPLAYER_MAP	online_players_;

	//维护了所有的活动对象，包括player,npc,monster....
	ONLINEENTITY_MAP	online_entity_;

	//锁
	ACE_Thread_Mutex	svr_mutex_;
	//网络命令队列
	COMMAND_LIST		net_cmds_1_;

public:
	//-------------------------------------------网络协议处理-----------------------------------
	//玩家断线
	void cts_userlost_ntf( BasicProtocol* p, bool& autorelease);
	//断线保持，数据重新加载请求
	void gts_lnkkeepload_req( BasicProtocol* p, bool& autorelease);
	//进入副本
	virtual void gts_instenter_req( BasicProtocol* p, bool& autorelease){}
	virtual void cts_instenter_req( BasicProtocol* p, bool& autorelease){}
	virtual void cts_instenter_ack( BasicProtocol* p, bool& autorelease){}
	virtual void cts_enterinstovertime_ntf( BasicProtocol* p, bool& autorelease){}
	virtual void gts_instenterconfirm_ntf( BasicProtocol* p, bool& autorelease){}
	virtual void gts_instquit_req( BasicProtocol* p, bool& autorelease){}

	//玩家信息初始化
	void dpx_chrload_ack( BasicProtocol* p, bool& autorelease);
	void dpx_chrfin_ntf( BasicProtocol* p, bool& autorelease);
	void cts_chrreg2world_req( BasicProtocol* p, bool& autorelease);
	//地图转跳
	void gts_teleport_req( BasicProtocol* p, bool& autorelease);
	void cts_teleport_ack( BasicProtocol* p, bool& autorelease);
	//更换装备
	void gts_equipitem_req( BasicProtocol* p, bool& autorelease);
	//宠物召唤
	void gts_petsummon_req( BasicProtocol* p, bool& autorelease);
	//聊天
	void gts_teamchat_req( BasicProtocol* p, bool& autorelease);
	void gts_p2pchat_req( BasicProtocol* p, bool& autorelease);
	//技能相关
	void gts_skillstudy_req( BasicProtocol* p, bool& autorelease);
	void gts_skillused_req( BasicProtocol* p, bool& autorelease);
	//玩家移动
	void gts_playermoveto_req( BasicProtocol* p, bool& autorelease);
};

extern BaseStoryService* get_thiscontext();

#define CONTENTSERVICE_INS	get_thiscontext()

#endif	//__BASESTORYSERVICE__H__
