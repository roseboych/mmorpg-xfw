/**
* protocol module
*
* 定义了player的宠物管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_Pet.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_PetSummon_ntf--------------------//
Pro_PetSummon_ntf::Pro_PetSummon_ntf()
:inherit( APP_PETSUMMON_NTF)
{
}

//-----------------------------Pro_PetSummon_req--------------------//
Pro_PetSummon_req::Pro_PetSummon_req()
:inherit( APP_PETSUMMON_REQ)
{
}

//-----------------------------Pro_PetSummon_ack--------------------//
Pro_PetSummon_ack::Pro_PetSummon_ack()
:inherit( APP_PETSUMMON_ACK)
{
}

PROTOCOL_NAMESPACE_END
