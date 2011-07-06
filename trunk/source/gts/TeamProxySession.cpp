/**
* gts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "TeamProxySession.h"

#include <ace/OS_NS_sys_time.h>
#include "player/Player.h"
#include "CSSLink.h"

TeamProxySession::TeamProxySession()
{
	reset();
}

void TeamProxySession::reset()
{
	status_ =PROXYSTATUS_NONE;
	uuid_.reset();
	token_ =0;
	player_ =0;
	lnkkeep_csslink_ =0;
	lnkkeep_chrid_ =NO_INITVALUE;
}

void TeamProxySession::lnkkeep_reset()
{
	status_ =PROXYSTATUS_LNKKEEP;
	lnkkeep_csslink_ =player_->css_svr_;
	lnkkeep_chrid_ =player_->curchrid_;

	player_ =0;
}

void TeamProxySession::proxy( S_INT_32 uid, S_TIMESTAMP st)
{
	status_ =PROXYSTATUS_CONFIRM;

	uuid_.set_uuid( uid, st);
	
	token_ =ACE_OS::gettimeofday().get_msec();

	player_ =0;
}

void TeamProxySession::proxy_confirm( Player* p)
{
	status_ =PROXYSTATUS_FIN;
	player_ =p;
	lnkkeep_csslink_ =0;
	lnkkeep_chrid_ =NO_INITVALUE;
}
