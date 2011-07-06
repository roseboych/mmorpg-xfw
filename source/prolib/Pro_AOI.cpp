/**
* protocol module
*
* 定义了AOI协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/Pro_AOI.h"

PROTOCOL_NAMESPACE_BEGIN

Pro_AppUnitEnter_ntf::Pro_AppUnitEnter_ntf()
:inherit( AOI_UNITENTER_NTF)
{
}

Pro_AppUnitLeave_ntf::Pro_AppUnitLeave_ntf()
:inherit( AOI_UNITLEAVE_NTF)
{
}

Pro_AppTeleport_req::Pro_AppTeleport_req()
:inherit( AOI_TELEPORT_REQ)
{
}

Pro_AppTeleport_ack::Pro_AppTeleport_ack()
:inherit( AOI_TELEPORT_ACK)
{
}

Pro_AppEnterIns_req::Pro_AppEnterIns_req()
:inherit( AOI_ENTERINS_REQ)
{
}

Pro_AppEnterIns_ack::Pro_AppEnterIns_ack()
:inherit( AOI_ENTERINS_ACK)
{
}

Pro_AppEnterInsOvertime_ntf::Pro_AppEnterInsOvertime_ntf()
:inherit( AOI_ENTERINSOT_NTF)
{
}

Pro_AppEnterInsConfirm_ntf::Pro_AppEnterInsConfirm_ntf()
:inherit( AOI_ENTERINSCONFIRM_NTF)
{
}

PROTOCOL_NAMESPACE_END
