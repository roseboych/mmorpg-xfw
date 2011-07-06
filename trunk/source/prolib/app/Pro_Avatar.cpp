/**
* protocol module
*
* 定义了player的avatar管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_Avatar.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_EquipItem_req--------------------//
Pro_EquipItem_req::Pro_EquipItem_req()
:inherit( APP_EQUIPITEM_REQ)
{
}

//-----------------------------Pro_EquipItem_ack--------------------//
Pro_EquipItem_ack::Pro_EquipItem_ack()
:inherit( APP_EQUIPITEM_ACK)
{
}

//-----------------------------Pro_EquipItem_ntf--------------------//
Pro_EquipItem_ntf::Pro_EquipItem_ntf()
:inherit( APP_EQUIPITEM_NTF)
{
}

PROTOCOL_NAMESPACE_END
