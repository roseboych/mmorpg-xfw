/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "BaseStoryService.h"

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <algorithm>

#include <ace/Time_Value.h>
#include <ace/OS_NS_sys_time.h>
#include <ace/Auto_Ptr.h>

#include <corelib/log/logmacro.h>
#include <corelib/util/ShareUtil.h>
#include <reslib/skills/SkillTreeConfig.h>
#include <reslib/buffers/BufferConfig.h>

#include "CSSModule.h"
#include "scene/mappaneimpl/SceneManagePaneImpl.h"

BaseStoryService::BaseStoryService():
mapres_( 0),
last_dotime_( 0),
is_support_script_( false)
{
#ifdef SCENE_PANE_IMPL
	scene_manage_ =FRAMEWK_NEW SceneManagePaneImpl( this);
#endif

	ACE_ASSERT( scene_manage_ != 0);
}

BaseStoryService::~BaseStoryService()
{
	if( scene_manage_)
		delete scene_manage_;
	scene_manage_ =0;
}

bool BaseStoryService::init_service( StoryMapOption* r)
{
	mapres_ =r;

	//初始化脚本环境
	is_support_script_ =true;
	if( !script_context_.init_scriptcontext())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "......initialize script context failed.......");
		return false;
	}

	if( !scene_manage_->init_scenemanage())
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "......initialize scene manage of the story service failed.......");
		return false;
	}

	//注册定时器
	add_contenttimer( 1000, boost::bind( &BaseStoryService::unitobj_enterleave_timer, this, _1));
	add_contenttimer( 100, boost::bind( &BaseStoryService::unitobj_tick_timer, this, _1));
	add_contenttimer( 10000, boost::bind( &BaseStoryService::script_gc_timer, this, _1));

	return true;
}

void BaseStoryService::uninit_service()
{
	online_players_.clear();
	online_entity_.clear();

	//清除定时器
	delall_contenttimer();

	scene_manage_->uninit_scenemanage();

	if( is_support_script_)
		script_context_.uninit_scriptcontext();

	for( COMMAND_LIST::iterator iter =net_cmds_1_.begin(); iter != net_cmds_1_.end(); ++iter)
		delete (*iter);
	net_cmds_1_.clear();
}

void BaseStoryService::do_service( const ACE_UINT64& now_t)
{
	if( last_dotime_ == 0)
		last_dotime_ =now_t;

	//一次循环的处理内容
	//定时器触发
	content_timer_.timer_tick( now_t);

	//处理命令队列
	BasicCommand *pCmd =pop_one_cmd();
	ACE_Auto_Ptr<BasicCommand> a_pcmd( pCmd);

	if( pCmd)
		pCmd->run();
}

BasicCommand* BaseStoryService::pop_one_cmd()
{
	if( net_cmds_1_.size() == 0)
		return 0;

	ACE_Guard<ACE_Thread_Mutex> guard( svr_mutex_);

	BasicCommand* pCmd =net_cmds_1_.front();
	net_cmds_1_.pop_front();

	return pCmd;
}

void BaseStoryService::regist_netcmd( BasicCommand *p)
{
	if( p == 0) return;

	ACE_Guard<ACE_Thread_Mutex> mon( svr_mutex_);

	net_cmds_1_.push_back( p);
}

void BaseStoryService::regist_to_onlineplayers( Player* user)
{
	online_players_[user->get_id()] =user;

	regist_to_onlineentity( user);
}

void BaseStoryService::unregist_from_onlineplayers( Player* user)
{
	online_players_.erase( user->get_id());

	unregist_from_onlineentity( user);
}

void BaseStoryService::regist_to_onlineentity( UnitBase* pobj)
{
	if( pobj == 0)
		return;

	online_entity_[pobj->get_uuid()] =pobj;
}

void BaseStoryService::unregist_from_onlineentity( UnitBase* pobj)
{
	if( pobj == 0)
		return;

	online_entity_.erase( pobj->get_uuid());
}

UnitBase* BaseStoryService::get_onlineentity( S_INT_64 uuid)
{
	ONLINEENTITY_MAP::iterator fiter =online_entity_.find( uuid);
	if( fiter == online_entity_.end())
		return 0;
	else
		return fiter->second;
}

void BaseStoryService::unitobj_enterleave_timer( ACE_UINT64 tnow)
{
	scene_manage_->scene_aoi_ticker( tnow);
}

void BaseStoryService::unitobj_tick_timer( const ACE_UINT64& tnow)
{
	ONLINEPLAYER_MAP::iterator iter =online_players_.begin(), eiter =online_players_.end();
	for( ; iter != eiter; ++iter)
	{
		Player* user =iter->second;
		user->tick( tnow);
	}
}

void BaseStoryService::script_gc_timer( const ACE_UINT64& tnow)
{
	if( !is_support_script_)
		return;

	script_context_.do_gc();
	//int ms1 =script_context_.get_luamemsize();
	//MODULE_LOG_DEBUG( MODULE_TEMP, "script context do gc collection, lua use %d size", ms1);
}
