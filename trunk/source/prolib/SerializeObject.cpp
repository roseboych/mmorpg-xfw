/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "prolib/SerializeObject.h"

SerializeObject::SerializeObject( S_INT_8 fctype, S_INT_16 siid):
factory_( fctype),
serialize_obj_iid_( siid)
{
}

SerializeObject::~SerializeObject()
{
}

bool SerializeObject::format( char** data, S_INT_32& llen, void* ext)
{
	char* pdata =*data;

	//serialize_obj_iid_
	LOAD_INT16_PRO( pdata, llen, serialize_obj_iid_);

	//移动指针位置
	*data =pdata;

	//用户扩展协议
	return to_protocol( data, llen, ext);
}

bool SerializeObject::data( char** buf, S_INT_32 buflen, S_INT_32& len, void* ext)
{
	char* pdata =*buf;

	//serialize_obj_iid_
	SAVE_INT16_PRO( pdata, buflen, len, serialize_obj_iid_);

	*buf =pdata;

	//用户扩展协议
	return to_buffer( buf, buflen, len, ext);
}
