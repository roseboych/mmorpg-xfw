/**
* protocol module
*
* 定义了角色类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "prolib/Pro_chr.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_ChrList_req--------------------//
Pro_ChrList_req::Pro_ChrList_req()
:inherit( GAME_CHRLIST_REQ)
{
}

//-----------------------------Pro_ChrList_ack--------------------//
Pro_ChrList_ack::Pro_ChrList_ack()
:inherit( GAME_CHRLIST_ACK)
{
}

//-----------------------------Pro_ChrCreate_req--------------------//
Pro_ChrCreate_req::Pro_ChrCreate_req()
:inherit( GAME_CHRCREATE_REQ)
{
}

//-----------------------------Pro_ChrCreate_ack--------------------//
Pro_ChrCreate_ack::Pro_ChrCreate_ack()
:inherit( GAME_CHRCREATE_ACK)
{
}

//-----------------------------Pro_ChrSel_req--------------------//
Pro_ChrSel_req::Pro_ChrSel_req()
:inherit( GAME_CHRSEL_REQ)
{
}

//-----------------------------Pro_ChrSel_ack--------------------//
Pro_ChrSel_ack::Pro_ChrSel_ack()
:inherit( GAME_CHRSEL_ACK)
{
}

//-----------------------------Pro_ChrDel_req--------------------//
Pro_ChrDel_req::Pro_ChrDel_req()
:inherit( GAME_CHRDEL_REQ)
{
}

//-----------------------------Pro_ChrDel_ack--------------------//
Pro_ChrDel_ack::Pro_ChrDel_ack()
:inherit( GAME_CHRDEL_ACK)
{
}

//----------------------------Pro_ChrRegistToWorld_req------------//
Pro_ChrRegistToWorld_req::Pro_ChrRegistToWorld_req()
:inherit( GAME_CHRREGTOWORLD_REQ),
is_quitinst_( 0)
{
}

//-----------------------------Pro_ChrLoad_req--------------------//
Pro_ChrLoad_req::Pro_ChrLoad_req()
:inherit( GAME_CHRLOAD_REQ)
{
}

//-----------------------------Pro_ChrLoad_ack--------------------//
Pro_ChrLoad_ack::Pro_ChrLoad_ack()
:inherit( GAME_CHRLOAD_ACK)
{
}

//-----------------------------Pro_ChrFin_NTF--------------------//
Pro_ChrFin_NTF::Pro_ChrFin_NTF()
:inherit( GAME_CHRFIN_NTF)
{
}

PROTOCOL_NAMESPACE_END
