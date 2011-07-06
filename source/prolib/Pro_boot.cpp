/**
* protocol module
*
* 定义了服务器组启动类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "prolib/Pro_boot.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_SvrRegist_req--------------------//
Pro_SvrRegist_req::Pro_SvrRegist_req()
:inherit( SVR_REGIST_REQ)
{
}

//-----------------------------Pro_SvrRegist_ack--------------------//
Pro_SvrRegist_ack::Pro_SvrRegist_ack()
:inherit( SVR_REGIST_ACK)
{
}

//-----------------------------Pro_SvrTeamInfo_NTF----------------//
Pro_SvrTeamInfo_NTF::Pro_SvrTeamInfo_NTF()
:inherit( SVR_SVRTEAMINFO_NTF)
{
}

//-----------------------------Pro_SvrSelTeamTimeout_NTF----------------//
Pro_SvrSelTeamTimeout_NTF::Pro_SvrSelTeamTimeout_NTF()
:inherit( SVR_SELTEAMTIMEOUT_NTF)
{
}

//-----------------------------Pro_SvrSelTeam_req----------------//
Pro_SvrSelTeam_req::Pro_SvrSelTeam_req()
:inherit( SVR_SELSERVER_REQ)
{
}

//-----------------------------Pro_SvrSelTeam_ack----------------//
Pro_SvrSelTeam_ack::Pro_SvrSelTeam_ack()
:inherit( SVR_SELSERVER_ACK)
{
}

//-----------------------------Pro_SvrSelTeamProxy_req----------------//
Pro_SvrSelTeamProxy_req::Pro_SvrSelTeamProxy_req()
:inherit( SVR_SELSERVERPROXY_REQ)
{
}

//-----------------------------Pro_SvrSelTeamProxy_req----------------//
Pro_SvrSelTeamProxy_ack::Pro_SvrSelTeamProxy_ack()
:inherit( SVR_SELSERVERPROXY_ACK)
{
}

//-----------------------------Pro_SvrSelTeamConfirm_req----------------//
Pro_SvrSelTeamConfirm_req::Pro_SvrSelTeamConfirm_req()
:inherit( SVR_SELSERVERCONFIRM_REQ)
{
}

//-----------------------------Pro_SvrSelTeamConfirm_ack----------------//
Pro_SvrSelTeamConfirm_ack::Pro_SvrSelTeamConfirm_ack()
:inherit( SVR_SELSERVERCONFIRM_ACK)
{
}

//-----------------------------Pro_SvrQuitTeam_req----------------//
Pro_SvrQuitTeam_req::Pro_SvrQuitTeam_req()
:inherit( SVR_QUITSERVER_REQ)
{
}

//-----------------------------Pro_SvrQuitTeam_ack----------------//
Pro_SvrQuitTeam_ack::Pro_SvrQuitTeam_ack()
:inherit( SVR_QUITSERVER_ACK)
{
}

//-----------------------------Pro_SvrQuitTeamReconn_req----------------//
Pro_SvrQuitTeamReconn_req::Pro_SvrQuitTeamReconn_req()
:inherit( SVR_QUITSERVERRECONN_REQ)
{
}

//-----------------------------Pro_SvrQuitTeamReconn_ack----------------//
Pro_SvrQuitTeamReconn_ack::Pro_SvrQuitTeamReconn_ack()
:inherit( SVR_QUITSERVERRECONN_ACK)
{
}

//-----------------------------Pro_SvrInstUpdate_ntf-------------------//
Pro_SvrInstUpdate_ntf::Pro_SvrInstUpdate_ntf()
:inherit( SVR_INSTUPDATE_NTF)
{}

//-----------------------------Pro_SvrOnlineTick_NTF----------------//
Pro_SvrOnlineTick_NTF::Pro_SvrOnlineTick_NTF()
:inherit( SVR_ONLINETICK_NTF)
{
}

//-----------------------------Pro_SvrUserLost_NTF----------------//
Pro_SvrUserLost_NTF::Pro_SvrUserLost_NTF()
:inherit( SVR_USERLOST_NTF)
{
}

//-----------------------------Pro_Login_req--------------------//
Pro_Login_req::Pro_Login_req()
:inherit( GAME_LOGIN_REQ)
{
}

//-----------------------------Pro_Login_ack--------------------//
Pro_Login_ack::Pro_Login_ack()
:inherit( GAME_LOGIN_ACK)
{
	is_lnkkeep_ =0;
}

//-----------------------------Pro_UserRegist_ntf----------------//
Pro_UserRegist_ntf::Pro_UserRegist_ntf()
:inherit( GAME_USERREGIST_NTF)
{
}

//-----------------------------Pro_Logout_req--------------------//
Pro_Logout_req::Pro_Logout_req()
:inherit( GAME_LOGOUT_REQ)
{
}

//-----------------------------Pro_Logout_ntf--------------------//
Pro_Logout_ntf::Pro_Logout_ntf()
:inherit( GAME_LOGOUT_NTF)
{
}

//-----------------------------Pro_SvrDisconnKeep_req--------------------//
Pro_SvrDisconnKeep_req::Pro_SvrDisconnKeep_req()
:inherit( SVR_DISCONNKEEP_NTF)
{
}

//-----------------------------Pro_SvrLnkkeepLoad_req--------------------//
Pro_SvrLnkkeepLoad_req::Pro_SvrLnkkeepLoad_req()
:inherit( SVR_LNKEEPLOAD_NTF)
{}

PROTOCOL_NAMESPACE_END
