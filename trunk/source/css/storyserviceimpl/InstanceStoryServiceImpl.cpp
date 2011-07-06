/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "InstanceStoryServiceImpl.h"

#include <reslib/deploy/INSCellOption.h>

#include "../CSSModule.h"
#include <prolib/Pro_all.h>

InstanceStoryServiceImpl::InstanceStoryServiceImpl():
inherit(),
inscell_opt_( 0),
svr_state_( TRANSCRIPT_SVRST_UNREGIST),
state_ctrl_( 0)
{
}

InstanceStoryServiceImpl::~InstanceStoryServiceImpl()
{
}

S_INT_32 InstanceStoryServiceImpl::get_cellid()
{
	return inscell_opt_->get_cellid();
}

bool InstanceStoryServiceImpl::init_service( StoryMapOption* opt, INSCellOption* cellopt)
{
	inscell_opt_ =cellopt;

	svr_state_ =TRANSCRIPT_SVRST_UNREGIST;

	return init_service( opt);
}

bool InstanceStoryServiceImpl::init_service( StoryMapOption* opt)
{
	if( !inherit::init_service( opt))
		return false;

	add_contenttimer( 1000, boost::bind( &InstanceStoryServiceImpl::transcript_state_ticker, this, _1));

	//构造状态管理器

	return true;
}

void InstanceStoryServiceImpl::uninit_service()
{
	inherit::uninit_service();

	for( ONLINEPLAYER_MAP::iterator iter =players_.begin(); iter != players_.end(); ++iter)
	{
		Player* user =iter->second;
		//复位
		user->reset();
		//释放
		Player::destroy_player( user);
	}

	players_.clear();

	svr_state_ =TRANSCRIPT_SVRST_UNREGIST;

	if( state_ctrl_)
	{
		delete state_ctrl_;
		state_ctrl_ =0;
	}
}

void InstanceStoryServiceImpl::init_instservice( void*)
{
	if( state_ctrl_)
	{
		delete state_ctrl_;
		state_ctrl_ =0;
	}

	StoryMapOption* mapopt =get_storyoption();
	ACE_ASSERT( mapopt != 0);

	state_ctrl_ =InstCtrlBase::create( mapopt->instmap_opt_.ctrl_classname_.c_str(), this);
	ACE_ASSERT( state_ctrl_ != 0);
	state_ctrl_->init_ctrl();
}

void InstanceStoryServiceImpl::transcript_state_ticker( ACE_UINT64 tnow)
{
	if( state_ctrl_ == 0)
		return;

	//控制器心跳
	state_ctrl_->tick( tnow);
}

Player* InstanceStoryServiceImpl::get_player( const BasicProtocol::internal_uuid& uuid)
{
	ONLINEPLAYER_MAP::iterator fiter =players_.find( uuid.userid_);
	if( fiter == players_.end())
		return 0;
	return fiter->second;
}

Player* InstanceStoryServiceImpl::player_regist( const BasicProtocol::internal_uuid& uuid, S_INT_32 chrid)
{
	Player* user =Player::create_player();
	ACE_ASSERT( user != 0);

	//初始化玩家基本信息
	user->regist( uuid.global_index_, uuid.userid_, uuid.init_stmp_, chrid);

	GTSLink* l =CSSMODULE->get_gtslinkbyuserglobal( user->global_index_);
	ACE_ASSERT( l != 0);
	user->gts_link_ =l;

	//注册到在线列表
	players_[user->uuid_.userid_] =user;

	MODULE_LOG_INFO( MODULE_TEMP, ">>>>>>>>>>>>>>>>>>>>> user:%d regist", user->uuid_.userid_);

	return user;
}

void InstanceStoryServiceImpl::player_unregist( const BasicProtocol::internal_uuid& uuid)
{
	Player* user =get_player( uuid);
	
	player_unregist( user);
}

void InstanceStoryServiceImpl::player_unregist( Player* user)
{
	if( user == 0 || user->is_unused())
		return;

	//从列表中清除
	players_.erase( user->uuid_.userid_);

	MODULE_LOG_INFO( MODULE_TEMP, "<<<<<<<<<<<<<<<<<<<<< user:%d unregist", user->uuid_.userid_);

	//TODO:其他退出机制

	//复位
	user->reset();

	//释放
	Player::destroy_player( user);
}

void InstanceStoryServiceImpl::set_serverstate( TRANSCRIPT_SERVER_STATE ns)
{
	if( ns == svr_state_)
		return;

	svr_state_ =ns;

	//发送状态更新
	PRO::Pro_SvrInstUpdate_ntf *ntf =PROTOCOL_NEW PRO::Pro_SvrInstUpdate_ntf();
	ntf->cellid_ =inscell_opt_->get_cellid();
	ntf->state_ =( S_INT_8)svr_state_;
	CSSMODULE->send_to_cts( ntf);
}
