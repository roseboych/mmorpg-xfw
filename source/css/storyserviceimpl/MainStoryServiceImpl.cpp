/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "MainStoryServiceImpl.h"

#include "../CSSModule.h"

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <corelib/log/logmacro.h>

MainStoryServiceImpl::MainStoryServiceImpl():inherit()
{
}

MainStoryServiceImpl::~MainStoryServiceImpl()
{
}

bool MainStoryServiceImpl::init_service( StoryMapOption* opt)
{
	if( !inherit::init_service( opt))
		return false;

	//构造最大玩家
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());
	player_nums_ =st->maxuser_;
	players_.reset( FRAMEWK_NEW Player[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		players_[ii].reset();
		players_[ii].global_index_ =ii;
	}

	return true;
}

void MainStoryServiceImpl::uninit_service()
{
	inherit::uninit_service();
}

Player* MainStoryServiceImpl::get_player( const BasicProtocol::internal_uuid& uuid)
{
	if( uuid.global_index_ < 0 || uuid.global_index_ >= player_nums_)
		return 0;
	return &players_[uuid.global_index_];
}

Player* MainStoryServiceImpl::player_regist( const BasicProtocol::internal_uuid& uuid, S_INT_32 chrid)
{
	Player* user =get_player( uuid);
	ACE_ASSERT( user != 0);

	//初始化玩家基本信息
	user->regist( uuid.userid_, uuid.init_stmp_, chrid);
	//注册到在线玩家
	BaseStoryService::regist_to_onlineplayers( user);

	GTSLink* l =CSSMODULE->get_gtslinkbyuserglobal( user->global_index_);
	ACE_ASSERT( l != 0);
	user->gts_link_ =l;

	MODULE_LOG_INFO( MODULE_TEMP, ">>>>>>>>>>>>>>>>>>>>> user:%d regist", user->uuid_.userid_);

	return user;
}

void MainStoryServiceImpl::player_unregist( const BasicProtocol::internal_uuid& uuid)
{
	Player* user =get_player( uuid);

	player_unregist( user);
}

void MainStoryServiceImpl::player_unregist( Player* user)
{
	if( user == 0 || user->is_unused())
		return;

	MODULE_LOG_INFO( MODULE_TEMP, "<<<<<<<<<<<<<<<<<<<<< user:%d unregist", user->uuid_.userid_);

	//TODO:其他退出机制

	//复位
	BaseStoryService::unregist_from_onlineplayers( user);
	user->reset();
}
