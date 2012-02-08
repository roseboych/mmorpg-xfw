/**
* cts application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <prolib/Pro_all.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/world/StoryMapOption.h>

#include "../TeamProxySession.h"
#include "../CSSLink.h"
#include "../CTSModule.h"
#include "../InstCellState.h"

Player::Player():
gts_link_( 0),
css_link_( 0)
{
	enter_instcell_ctrl_.set_owner( this);
}

void Player::reset()
{
	css_link_ =0;

	uuid_.reset();
	proxy_session_ =0;

	chrid_ =NO_INITVALUE;
	lastposx_ =lastposy_ =lastposz_ =NO_INITVALUE;
	is_lnkkeep_ =false;
}

void Player::selteamconfirm( TeamProxySession* proxy, S_TIMESTAMP st)
{
	uuid_.set_uuid( proxy->uuid_.userid_, st);

	css_link_ =0;

	proxy_session_ =proxy;

	proxy->selteam_confirm( this);
}

void Player::chrsel( int chrid, float posx, float posy, float posz)
{
	this->chrid_ =chrid;
	this->lastposx_ =posx;
	this->lastposy_ =posy;
	this->lastposz_ =posz;
}

void Player::tick( const ACE_UINT64& t)
{
	enter_instcell_ctrl_.tick( t);
}

void Player::instcell_proxy_overtime()
{
	//向主地图服务器发送超时提醒
	PRO::Pro_AppEnterInsOvertime_ntf *ntf =PROTOCOL_NEW PRO::Pro_AppEnterInsOvertime_ntf();
	PRO_UUID_FILL( ntf, this->global_index_, this->uuid_);
	ntf->cellid_ =enter_instcell_ctrl_.inst_cell_->get_cellid();
	send_to_css( ntf->clone());

	//向副本地图发送超时提醒
	ACE_ASSERT( enter_instcell_ctrl_.css_svr_ != 0);
	enter_instcell_ctrl_.css_svr_->send_protocol( ntf);

	enter_instcell_ctrl_.reset();
}

void Player::instcell_confirm_overtime()
{
	//向副本地图发送超时提醒
	PRO::Pro_AppEnterInsOvertime_ntf *ntf =PROTOCOL_NEW PRO::Pro_AppEnterInsOvertime_ntf();
	PRO_UUID_FILL( ntf, this->global_index_, this->uuid_);
	ntf->cellid_ =enter_instcell_ctrl_.inst_cell_->get_cellid();
	send_to_css( ntf);

	instcell_quit();
}

void Player::instcell_quit()
{
	//此时玩家已经从主世界退出，所以需要重新注册到主世界
	StoryMapOption* opt =WORLDINFO->get_mainstorybyxy( this->lastposx_, this->lastposy_);
	ACE_ASSERT( opt != 0);

	//重新把和css的关联设置为和主世界地图的关联
	this->css_link_ =CTSMODULE->get_csslink( opt->owner_css_->server_index_);

	//发送注册请求
	PRO::Pro_ChrRegistToWorld_req* loadreq =PROTOCOL_NEW PRO::Pro_ChrRegistToWorld_req();
	PRO_UUID_FILL( loadreq, this->global_index_, this->uuid_);

	loadreq->chrid_ =this->chrid_;
	loadreq->mapid_ =opt->get_mapid();
	loadreq->cssindex_ =opt->owner_css_->server_index_;
	loadreq->is_quitinst_ =1;

	send_to_css( loadreq);

	enter_instcell_ctrl_.reset();
}
