/**
* protocol module
*
* 定义了所有的聊天协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_Chat.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_TeamChat_req--------------------//
Pro_TeamChat_req::Pro_TeamChat_req()
:inherit( APP_TEAMCHAT_REQ)
{
}

//-----------------------------Pro_TeamChat_ntf--------------------//
Pro_TeamChat_ntf::Pro_TeamChat_ntf()
:inherit( APP_TEAMCHAT_NTF)
{
}

//-----------------------------Pro_P2PChat_req--------------------//
Pro_P2PChat_req::Pro_P2PChat_req()
:inherit( APP_P2PCHAT_REQ)
{
}

//-----------------------------Pro_P2PChat_ntf--------------------//
Pro_P2PChat_ntf::Pro_P2PChat_ntf()
:inherit( APP_P2PCHAT_NTF)
{
}

PROTOCOL_NAMESPACE_END
