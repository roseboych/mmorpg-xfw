/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PlayerInfo.h"

#include "RGSModule.h"

PlayerInfo::PlayerInfo(void)
:cts_link_( 0),
lgs_link_( 0)
{
	clear();
}

PlayerInfo::~PlayerInfo(void)
{
	clear();
}

void PlayerInfo::clear()
{
	status_ =PLAYERSTATUS_NONE;

	uuid_.reset();
	server_index_ =NO_INITVALUE;

	lgs_link_ =0;
	cts_link_ =0;
}

void PlayerInfo::prepare()
{
	status_ =PLAYERSTATUS_WAITLOGIN;
	uuid_.reset();
	server_index_ =NO_INITVALUE;

	lgs_link_ =0;
	cts_link_ =0;
}

void PlayerInfo::login_prepare( int serverindex, S_TIMESTAMP session, LGSLink* l)
{
	status_ =PLAYERSTATUS_LOGINING;
	uuid_.set_timestamp( session);
	this->lgs_link_ =l;

	this->server_index_ =serverindex;
}

void PlayerInfo::login_succ()
{
	status_ =PLAYERSTATUS_SELTEAM;
}

void PlayerInfo::selteam_begin( CTSLink* l)
{
	status_ =PLAYERSTATUS_SELTEAMING;
	this->cts_link_ =l;
}

void PlayerInfo::selteam_end( bool succ)
{
	if( succ)
	{
		status_ =PLAYERSTATUS_GTSVALIDATE;
	}
	else
	{
		status_ =PLAYERSTATUS_SELTEAM;
		this->cts_link_ =0;
	}
}

void PlayerInfo::selteam_confirm()
{
	if( status_ != PLAYERSTATUS_GTSVALIDATE)
		return;

	status_ =PLAYERSTATUS_FIN;
	//复位和lgs的连接
	this->lgs_link_ =0;
}

void PlayerInfo::lnkkeep_confirm()
{
	if( status_ != PLAYERSTATUS_LNKKEEP_SELTEAMING)
		return;

	status_ =PLAYERSTATUS_FIN;
	//复位和lgs的连接
	this->lgs_link_ =0;
}

void PlayerInfo::selteam_timeout()
{
	status_ =PLAYERSTATUS_SELTEAM;
	this->cts_link_ =0;
}

void PlayerInfo::quit_team()
{
	status_ =PLAYERSTATUS_QUITTEAM;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
	qt_token_ =tstamp_;
	this->cts_link_ =0;
}

void PlayerInfo::quitteam_reconn( int serverindex, S_TIMESTAMP session, LGSLink* l)
{
	status_ =PLAYERSTATUS_SELTEAM;
	uuid_.set_timestamp( session);
	this->lgs_link_ =l;

	this->server_index_ =serverindex;
}

void PlayerInfo::link_keep( S_TIMESTAMP& token, int teamid, int gtsid, int proxyindex)
{
	status_ =PLAYERSTATUS_LNKKEEP;
	qt_token_ =token;
	lnkkeep_teamid_ =teamid;
	lnkkeep_gtsid_ =gtsid;
	lnkkeep_proxyindex_ =proxyindex;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
}

void PlayerInfo::login_succ_of_lnkkeep( int serverindex, LGSLink* l)
{
	status_ =PLAYERSTATUS_LNKKEEP_SELTEAMING;
	this->server_index_ =serverindex;
	this->lgs_link_ =l;
}

bool PlayerInfo::quitteamreconn_timeout( ACE_UINT64& tnow)
{
	if( status_ != PLAYERSTATUS_QUITTEAM)
		return false;

	//30s重连
	if( tstamp_ + 30*1000 < tnow)
		return true;
	else
		return false;
}

bool PlayerInfo::lnkkeep_timeout( ACE_UINT64& tnow)
{
	if( status_ != PLAYERSTATUS_LNKKEEP)
		return false;

	//5m重连
	//if( tstamp_ + 5*60000 < tnow)
	if( tstamp_ + 5000 < tnow)
		return true;
	else
		return false;
}
