/**
* dpx application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DPX_Service.h"

#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "DPXModule.h"

void DPX_Service::from_css( BasicProtocol *p)
{
	USE_PROTOCOL_NAMESPACE;
	ACE_Auto_Ptr<BasicProtocol> p_pro( p);

	NETCMD_FUN_MAP fun =0;
	if( p->iid_ == GAME_CHRLOAD_REQ)
	{
		fun =boost::bind( &DPXSvr::css_chrload_req, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == APP_BUFFCHANGE_NTF)
	{
		fun =boost::bind( &DPXSvr::css_buffchange_ntf, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == DB_POSROTSAVE_NTF)
	{
		fun =boost::bind( &DPXSvr::css_posrotsave_ntf, DPXMODULE, _1, _2);
	}
	else if( p->iid_ == DB_PLAYERBASEINFO_SAVE)
	{
		fun =boost::bind( &DPXSvr::css_playerbaseinfo_sav, DPXMODULE, _1, _2);
	}

	if( fun)
	{
		NetCommand* pcmd =TASKCMD_NEW NetCommand( p_pro.release(), fun, true);
		DPXMODULE->regist_netcmd( pcmd);
	}
}
