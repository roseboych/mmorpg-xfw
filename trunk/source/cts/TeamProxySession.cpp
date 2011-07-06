/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "TeamProxySession.h"

#include <ace/OS_NS_sys_time.h>
#include "player/Player.h"

TeamProxySession::TeamProxySession()
{
	reset();
}

void TeamProxySession::reset()
{
	uuid_.reset();
	server_index_ =NO_INITVALUE;

	proxy_timestamp_ =0;

	player_ =0;
}

void TeamProxySession::proxy( S_INT_32 uid, S_TIMESTAMP st, S_INT_32 si)
{
	uuid_.set_uuid( uid, st);
	this->server_index_ =si;

	proxy_timestamp_ =0;

	player_ =0;

	check_stmp_ =ACE_OS::gettimeofday().get_msec();
}

void TeamProxySession::proxy_ack( S_TIMESTAMP st)
{
	this->proxy_timestamp_ =st;
}

void TeamProxySession::selteam_confirm( Player* p)
{
	this->player_ =p;
}

bool TeamProxySession::proxy_timeover( ACE_UINT64& now)
{
	//10s没有完成登陆验证
	if( check_stmp_ + 10*1000 < now)
		return true;
	return false;
}
