#include "CSSLocalLink.h"

#include <ace/Auto_Ptr.h>

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma pack( pop)

/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include <memory>

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "CSSModule.h"

CSSLocalLink::CSSLocalLink()
{
}

int CSSLocalLink::get_serverindex()
{
	return opt_->server_index_;
}

void CSSLocalLink::send_protocol( BasicProtocol* p)
{
	if( p == 0)
		return;

	NETCMD_FUN_MAP fun =boost::bind( &CSSSvr::css_netadapter, CSSMODULE, _1, _2);

	NetCommand* pcmd =TASKCMD_NEW NetCommand( p, fun, true);
	CSSMODULE->regist_netcmd( pcmd);
}

void CSSLocalLink::set_option( CSSOption* s)
{
	this->opt_ =s;
}
