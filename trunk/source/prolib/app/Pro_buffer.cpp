/**
* protocol module
*
* 定义了buffer使用协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_buffer.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_BuffChange_ntf--------------------//
Pro_BuffChange_ntf::Pro_BuffChange_ntf()
:inherit( APP_BUFFCHANGE_NTF)
{
}

PROTOCOL_NAMESPACE_END
