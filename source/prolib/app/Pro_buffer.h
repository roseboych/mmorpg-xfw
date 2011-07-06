/**
* protocol module
*
* 定义了buffer使用协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_BUFFER__H__
#define __PRO_BUFFER__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/boost.h"
#include "prolib/pre.h"
#include "prolib/BasicProtocol.h"
#include "prolib/datastore/buffer_define.h"

#include <list>

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_BuffChange_ntf
* 
* @brief buffer变化通知
**/
class Pro_BuffChange_ntf : public AppProtocol<Pro_BuffChange_ntf>
{
	typedef AppProtocol<Pro_BuffChange_ntf> inherit;

public:
	Pro_BuffChange_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT8_PRO( pdata, totlen, ntf_type_)
		LOAD_INT32_PRO( pdata, totlen, change_buff_.bufferid_)
		LOAD_INT8_PRO( pdata, totlen, change_buff_.types_)
		LOAD_INT32_PRO( pdata, totlen, change_buff_.createtime_)
		LOAD_INT32_PRO( pdata, totlen, change_buff_.scopetime_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT8_PRO( pdata, buflen, len, ntf_type_)
		SAVE_INT32_PRO( pdata, buflen, len, change_buff_.bufferid_)
		SAVE_INT8_PRO( pdata, buflen, len, change_buff_.types_)
		SAVE_INT32_PRO( pdata, buflen, len, change_buff_.createtime_)
		SAVE_INT32_PRO( pdata, buflen, len, change_buff_.scopetime_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_BuffChange_ntf, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, ntf_type_)
		CLONE_VAR_PRO( proo, change_buff_)
	END_CLONE_PRO()

public:
	//buffer所属的角色id
	S_INT_32	chrid_;
	//0:新增的buffer 1:buffer失效 2:buffer更新
	S_INT_8		ntf_type_;
	bufferdata_item	change_buff_;
};


PROTOCOL_NAMESPACE_END

#endif	//__PRO_BUFFER__H__
