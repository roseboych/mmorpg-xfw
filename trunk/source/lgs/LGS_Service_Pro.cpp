/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGS_Service.h"

#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "LGSModule.h"

void LGS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;

	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	SESSION_UUID_FILL( p, player_->server_index_, player_->uuid_)

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == GAME_LOGIN_REQ)
	{
		//玩家登陆请求
		fun =boost::bind( &LGSSvr::net_user_loginreq, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_SELSERVER_REQ)
	{
		//选服请求
		fun =boost::bind( &LGSSvr::net_team_selreq, LGSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVERRECONN_REQ)
	{
		fun =boost::bind( &LGSSvr::net_quitteamreconn_req, LGSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		LGSMODULE->regist_netcmd( pcmd);
	}
}
