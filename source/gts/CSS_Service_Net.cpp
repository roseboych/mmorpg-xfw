/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSS_Service.h"

#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "GTSModule.h"

void CSS_Service::assort_protocol( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == GAME_CHRREGTOWORLD_REQ)
	{
		fun =boost::bind( &GTSSvr::css_chrreg2world_req, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRLOAD_ACK)
	{
		fun =boost::bind( &GTSSvr::css_chrload_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == GAME_CHRFIN_NTF)
	{
		fun =boost::bind( &GTSSvr::css_chrfin_ntf, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == AOI_TELEPORT_ACK)
	{
		fun =boost::bind( &GTSSvr::css_teleport_ack, GTSMODULE, _1, _2);
	}
	else if( p->iid_ == AOI_ENTERINS_ACK)
	{
		fun =boost::bind( &GTSSvr::css_instenter_ack, GTSMODULE, _1, _2);
	}
	else if( 
		   p->iid_ == GAME_LOGOUT_NTF
		|| p->iid_ == AOI_UNITENTER_NTF
		|| p->iid_ == AOI_UNITLEAVE_NTF
		//buffer相关
		|| p->iid_ == APP_BUFFCHANGE_NTF
		//状态相关
		|| p->iid_ == APP_PLAYERMOVE_ACK
		|| p->iid_ == APP_PLAYERMOVE_NTF
		//技能相关
		|| p->iid_ == APP_SKILLINIT_NTF
		|| p->iid_ == APP_SKILLSTATECHG_NTF
		|| p->iid_ == APP_SKILLSTUDY_ACK
		|| p->iid_ == APP_SKILLUSED_ACK
		//宠物相关
		|| p->iid_ == APP_PETSUMMON_ACK
		|| p->iid_ == APP_PETSUMMON_NTF
		//聊天相关
		|| p->iid_ == APP_TEAMCHAT_NTF
		|| p->iid_ == APP_P2PCHAT_NTF
		)
	{
		fun =boost::bind( &GTSSvr::transfer_to_net, GTSMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		GTSMODULE->regist_netcmd( pcmd);
	}
}
