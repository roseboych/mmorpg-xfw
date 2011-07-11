/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <ace/OS_NS_sys_time.h>
#include <prolib/Pro_all.h>
#include "../GTSLink.h"
#include "../BaseStoryService.h"
#include "../CSSModule.h"

boost::object_pool<Player> Player::player_pools_;
ACE_Thread_Mutex Player::pool_mutex_;

Player::Player():inherit(),
status_( PLAYERSTATUS_NONE),
gts_link_( 0),
fight_target_obj_( 0),
is_entertranscript_( false),
is_in_inst( false)
{
	mybuffers_.set_owner( this);
	myskills_.set_owner( this);
	action_selector_.set_owner( this);
}

Player::~Player()
{
	uuid_.reset();
	chrid_ =NO_INITVALUE;
	gts_link_ =0;
	status_ =PLAYERSTATUS_NONE;
	fight_target_obj_ =0;
	is_in_inst =false;
}

Player* Player::create_player()
{
	ACE_Guard<ACE_Thread_Mutex> mon( pool_mutex_);

	Player* ret =player_pools_.construct();
	ret->reset();

	return ret;
}

void Player::destroy_player( Player* p)
{
	if( p == 0)
		return;

	ACE_Guard<ACE_Thread_Mutex> mon( pool_mutex_);
	player_pools_.destroy( p);
}

void Player::reset()
{
	//通知监听对象，player重置事件
	trigger_unitentityreset();
	set_fighttargetobj( 0);

	inherit::reset();

	PlayerData::reset();

	action_selector_.reset_selector();

	uuid_.reset();
	chrid_ =NO_INITVALUE;
	status_ =PLAYERSTATUS_NONE;
	gts_link_ =0;
	is_entertranscript_ =false;
	is_in_inst =false;
}

S_INT_32 Player::get_id()
{
	return uuid_.userid_;
}

void Player::send_to_gts( BasicProtocol* p)
{
	if( gts_link_ == 0)
		delete p;
	else
		gts_link_->send_protocol( p);
}

void Player::regist( S_INT_32 uid, S_TIMESTAMP stmp, S_INT_32 chrid)
{
	uuid_.set_uuid( uid, stmp);
	this->chrid_ =chrid;
	this->status_ =PLAYERSTATUS_REGIST;
}

void Player::regist( S_INT_32 gid, S_INT_32 uid, S_TIMESTAMP stmp, S_INT_32 chrid)
{
	regist( uid, stmp, chrid);
	this->global_index_ =gid;
}

void Player::props_load( PRO::Pro_ChrLoad_ack* ack)
{
	PlayerData::init_baseinfo( ack);
}

void Player::regist_fin( PRO::Pro_ChrFin_NTF* fin)
{
	//TODO:存储装备信息
	//如果是副本，则保留注册副本时保存的出生点信息
	//同时把ntf协议中的位置信息修改为副本位置信息(副本位置不保存到数据库)
	if( this->is_in_inst)
	{
		fin->baseinfo_.posx_ =baseinfo_.posx_;
		fin->baseinfo_.posy_ =baseinfo_.posy_;
		fin->baseinfo_.posz_ =baseinfo_.posz_;
	}

	PlayerData::init_others( fin);

	this->status_ =PLAYERSTATUS_INSCENE;

	//设置位置保存信息
	this->lastposx_ =fin->baseinfo_.posx_;
	this->lastposy_ =fin->baseinfo_.posy_;
	this->lastposz_ =fin->baseinfo_.posz_;
	this->lastfacing_ =fin->baseinfo_.facing_;
	t_lastupdate_ =ACE_OS::gettimeofday().get_msec();

	//设置初始状态
	action_selector_.regist_finish();
	myskills_.regist_finish();
	cur_state_ =action_selector_.get_default_state();
}

void Player::moveto( coordinate_pointer& pos, S_FLOAT_32& face)
{
	//保存位置
	set_position( pos);
	set_facing( face);
}

void Player::tick( const ACE_UINT64& tnow)
{
	if( this->status_ != PLAYERSTATUS_INSCENE)
		return;

	//buffer tick
	mybuffers_.tick( tnow);
	myskills_.tick( tnow);

	//是否需要更新坐标
	if( is_needupdatepos( tnow))
	{
		this->lastposx_ =baseinfo_.posx_;
		this->lastposy_ =baseinfo_.posy_;
		this->lastposz_ =baseinfo_.posz_;
		this->lastfacing_ =baseinfo_.facing_;
		this->t_lastupdate_ =tnow;

		USE_PROTOCOL_NAMESPACE;

		Pro_DBPosRotSave_ntf* ntf =PROTOCOL_NEW Pro_DBPosRotSave_ntf();
		PRO_UUID_FILL( ntf, this->global_index_, this->uuid_);

		ntf->lastposx_ =lastposx_;
		ntf->lastposy_ =lastposy_;
		ntf->lastposz_ =lastposz_;
		ntf->lastfacing_ =lastfacing_;
		
		CONTENTSERVICE_INS->get_scenemanage()->local_to_world( ntf->lastposx_, ntf->lastposy_);

		CSSMODULE->send_to_dpx( ntf);
	}
}
