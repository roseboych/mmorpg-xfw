/**
* protocol module
*
* 定义了player的宠物管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_PET__H__
#define __PRO_PET__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/BasicProtocol.h"
#include "prolib/SerializeFactoryContainer.h"

#include <list>

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_PetSummon_ntf
* 
* @brief 召唤宠物通知
**/
class Pro_PetSummon_ntf : public AppProtocol<Pro_PetSummon_ntf>
{ 
	typedef AppProtocol<Pro_PetSummon_ntf> inherit;

public:
	Pro_PetSummon_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT32_PRO( pdata, totlen, petcode_)
		LOAD_STR_PRO( pdata, totlen, petname_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT32_PRO( pdata, buflen, len, petcode_)
		SAVE_STR_PRO( pdata, buflen, len, petname_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PetSummon_ntf, proo)
		CLONE_VAR_PRO(proo, chrid_)
		CLONE_VAR_PRO(proo, petcode_)
		CLONE_VAR_PRO(proo, petname_)
	END_CLONE_PRO()

public:
	// 角色id
	S_INT_32	chrid_;
	//宠物petcode_ >0: 放出宠物, -1:收起宠物
	S_INT_32	petcode_;
	// 宠物名称
	StaticString<CHR_PETNAME_LEN>	petname_;
};

/**
* @class Pro_PetSummon_req
* 
* @brief 召唤宠物请求
**/
class Pro_PetSummon_req : public AppProtocol<Pro_PetSummon_req>
{ 
	typedef AppProtocol<Pro_PetSummon_req> inherit;

public:
	Pro_PetSummon_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, petid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, petid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PetSummon_req, proo)
		CLONE_VAR_PRO(proo, petid_)
	END_CLONE_PRO()

public:
	// 宠物实例id,-1表示收宠
	S_INT_32	petid_;
};

/**
* @class Pro_PetSummon_ack
* 
* @brief 召唤宠物请求回复
**/
class Pro_PetSummon_ack : public AppProtocol<Pro_PetSummon_ack>
{ 
	typedef AppProtocol<Pro_PetSummon_ack> inherit;

public:
	Pro_PetSummon_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, petid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, petid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PetSummon_ack, proo)
		CLONE_VAR_PRO(proo, result_)
		CLONE_VAR_PRO(proo, petid_)
	END_CLONE_PRO()

public:
	// 0成功，1:招宠失败，宠物不存在 2:收宠失败，没有活动宠物
	S_INT_8		result_;
	// 宠物实例id,-1表示收宠回复
	S_INT_32	petid_;
};

PROTOCOL_NAMESPACE_END

#endif	//__PRO_PET__H__
