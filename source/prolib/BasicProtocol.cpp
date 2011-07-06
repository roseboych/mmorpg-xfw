/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "prolib/BasicProtocol.h"

BasicProtocol::BasicProtocol( S_INT_32 id)
:iid_( id)
{
}

BasicProtocol::~BasicProtocol()
{
}

bool BasicProtocol::format( char* data, S_INT_32 len, void* ext, bool internals)
{
	char* pdata =data;
	S_INT_32 llen =len;

	//iid_
	LOAD_INT32_PRO( pdata, llen, iid_);

	if( internals)
	{
		//global_index_
		LOAD_INT32_PRO( pdata, llen, uuid_.global_index_);
		//userid_
		LOAD_INT32_PRO( pdata, llen, uuid_.userid_);
		//init_stmp_
		LOAD_TIMESTAMP_PRO( pdata, llen, uuid_.init_stmp_);
	}

	//用户扩展协议
	return to_protocol( pdata, llen, ext);
}

bool BasicProtocol::data( char* buf, S_INT_32 buflen, S_INT_32& len, void* ext, bool internals)
{
	char* pdata =buf;

	//iid_
	SAVE_INT32_PRO( pdata, buflen, len, iid_);

	if( internals)
	{
		//global_index_
		SAVE_INT32_PRO( pdata, buflen, len, uuid_.global_index_);
		//userid_
		SAVE_INT32_PRO( pdata, buflen, len, uuid_.userid_);
		//init_stmp_
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, uuid_.init_stmp_);
	}

	//用户扩展协议
	return to_buffer( pdata, buflen, len, ext);
}
