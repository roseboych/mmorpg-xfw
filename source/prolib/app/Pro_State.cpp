/**
* protocol module
*
* 定义了player的状态管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_State.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_PlayerMove_req--------------------//
Pro_PlayerMove_req::Pro_PlayerMove_req()
:inherit( APP_PLAYERMOVE_REQ)
{
}

//-----------------------------Pro_PlayerMove_ack--------------------//
Pro_PlayerMove_ack::Pro_PlayerMove_ack()
:inherit( APP_PLAYERMOVE_ACK)
{
}

//-----------------------------Pro_PlayerMove_ntf--------------------//
Pro_PlayerMove_ntf::Pro_PlayerMove_ntf()
:inherit( APP_PLAYERMOVE_NTF)
{
}

//-----------------------------Pro_DBPosRotSave_ntf--------------------//
Pro_DBPosRotSave_ntf::Pro_DBPosRotSave_ntf()
:inherit( DB_POSROTSAVE_NTF)
{
}

//-----------------------------Pro_DBPlayerBaseInfo_sav--------------------//
Pro_DBPlayerBaseInfo_sav::Pro_DBPlayerBaseInfo_sav()
:inherit( DB_PLAYERBASEINFO_SAVE)
{
}

PROTOCOL_NAMESPACE_END
