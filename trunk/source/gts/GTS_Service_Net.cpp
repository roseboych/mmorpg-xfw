/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTS_Service.h"

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "GTSModule.h"

void GTS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == SVR_SELSERVERCONFIRM_REQ)
	{
		NETCMD_FUN_MAP3 fun3 =boost::bind( &GTSSvr::net_proxyconfirm_req, GTSMODULE, _1, _2, (void*)player_);
		NetCommandV* pcmd =TASKCMD_NEW NetCommandV( p_pro.release(), fun3, true);
		GTSMODULE->regist_netcmd( pcmd);

		return;
	}
	else if( p->iid_ == GAME_LOGOUT_REQ)
	{
		fun =boost::bind( &GTSSvr::net_userlogout_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == SVR_QUITSERVER_REQ)
	{
		fun =boost::bind( &GTSSvr::net_quitteam_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRLIST_REQ)
	{
		fun =boost::bind( &GTSSvr::net_chrlist_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRSEL_REQ)
	{
		fun =boost::bind( &GTSSvr::net_chrsel_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRDEL_REQ)
	{
		fun =boost::bind( &GTSSvr::net_chrdel_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRCREATE_REQ)
	{
		fun =boost::bind( &GTSSvr::net_chradd_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == AOI_TELEPORT_REQ)
	{
		fun =boost::bind( &GTSSvr::net_teleport_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == AOI_ENTERINS_REQ)
	{
		fun =boost::bind( &GTSSvr::net_instenter_req, GTSMODULE, _1, _2);
	}
	else if(
		//宠物相关
		p->iid_ == APP_PETSUMMON_REQ ||
		//装备相关
		p->iid_ == APP_EQUIPITEM_REQ ||
		//聊天相关
		p->iid_ == APP_TEAMCHAT_REQ	||
		p->iid_ == APP_P2PCHAT_REQ	||
		//状态相关
		p->iid_ == APP_PLAYERMOVE_REQ	||
		//技能相关
		p->iid_ == APP_SKILLSTUDY_REQ	||
		p->iid_ == APP_SKILLUSED_REQ
		)
	{
		fun =boost::bind( &GTSSvr::transfer_to_css_ingame, GTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		GTSMODULE->regist_netcmd( pcmd);
	}
}
