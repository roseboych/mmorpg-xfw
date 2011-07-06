/**
* gts application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <ace/OS_NS_sys_time.h>

#include "../GTSModule.h"
#include "../TeamProxySession.h"

Player::Player():
#ifdef CORELIB_WIN32
service_( 0)
#else
service_( 0)
#endif
{
	service_.player_ =this;
	css_svr_ =0;
	have_force_close_ =false;
	is_instsvr_ =false;
}

Player::~Player()
{
}

void Player::send_to_css( BasicProtocol* p)
{
	if( css_svr_ == 0)
		delete p;
	else
		css_svr_->send_protocol( p);
}

void Player::reset()
{
	global_index_ =NO_INITVALUE;

	service_.reset();

	status_ =PLAYERSTATUS_NONE;
	uuid_.reset();
	curchrid_ =NO_INITVALUE;
	css_svr_ =0;

	team_proxy_ =0;

	have_force_close_ =false;
	is_instsvr_ =false;
}

void Player::prepare()
{
	status_ =PLAYERSTATUS_PROXYW;

	uuid_.reset();
	team_proxy_ =0;

	curchrid_ =NO_INITVALUE;
	css_svr_ =0;
	selteam_tmpstamp_ =ACE_OS::gettimeofday().get_msec();

	have_force_close_ =false;
	is_instsvr_ =false;
}

void Player::proxy_confirm( TeamProxySession* proxy)
{
	if( proxy->is_lnkkeep())
	{
		//断线保持，直接进入线组
		status_ =PLAYERSTATUS_INGAME;
		team_proxy_ =proxy;
		uuid_.set_uuid( proxy->uuid_.userid_, proxy->uuid_.init_timestamp_);
		this->css_svr_ =proxy->lnkkeep_csslink_;
		this->curchrid_ =proxy->lnkkeep_chrid_;
		this->global_index_ =proxy->proxy_index_;
		//设置代理
		proxy->proxy_confirm( this);
	}
	else
	{
		status_ =PLAYERSTATUS_INTEAM;
		team_proxy_ =proxy;
		proxy->token_ =proxy->uuid_.init_timestamp_;
		uuid_.set_uuid( proxy->uuid_.userid_, proxy->uuid_.init_timestamp_);
		//关联全局索引
		this->global_index_ =proxy->proxy_index_;
		//设置代理
		proxy->proxy_confirm( this);
	}
}

void Player::set_curchrid( S_INT_32 cid)
{
	this->curchrid_ =cid;

	if( curchrid_ != NO_INITVALUE)
	{
		status_ =PLAYERSTATUS_INGAME;
		selteam_tmpstamp_ =ACE_OS::gettimeofday().get_msec();
	}
	else
		status_ =PLAYERSTATUS_INTEAM;
}

void Player::main_tick( ACE_UINT64& t)
{
	if( status_ == PLAYERSTATUS_NONE || have_force_close_)
		return;

	bool blost =false;
	if( status_ == PLAYERSTATUS_PROXYW)
	{
		//保证不会有无用的socket占用资源 8s
		if( selteam_tmpstamp_ + 8*1000 < t)
			blost =true;
	}
	else if( status_ == PLAYERSTATUS_OUTTEAM_FIN)
	{
		//发送后断开
		if( selteam_tmpstamp_ + 500 < t)
			blost =true;
	}
	else if( status_ == PLAYERSTATUS_LOGOUT)
	{
		//注销请求
		blost =true;
	}

	if( blost)
	{
		have_force_close_ =true;
		service_.conn_error();
		return;
	}

	service_.online_tickit( t);
}
