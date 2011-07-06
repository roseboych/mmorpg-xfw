/**
* protocol module
*
* 定义了player的avatar管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_AVATAR__H__
#define __PRO_AVATAR__H__

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
* @class Pro_EquipItem_req
* 
* @brief 换装请求
**/
class Pro_EquipItem_req : public AppProtocol<Pro_EquipItem_req>
{ 
	typedef AppProtocol<Pro_EquipItem_req> inherit;

public:
	Pro_EquipItem_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, itemid_)
		LOAD_INT16_PRO( pdata, totlen, bagslot_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, itemid_)
		SAVE_INT16_PRO( pdata, buflen, len, bagslot_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_EquipItem_req, proo)
		CLONE_VAR_PRO(proo, itemid_)
		CLONE_VAR_PRO(proo, bagslot_)
	END_CLONE_PRO()

public:
	//物品id
	S_INT_32	itemid_;
	//>=0:卸下物品，并放到指定的bagslot -1:装上物品
	S_INT_16	bagslot_;
};

/**
* @class Pro_EquipItem_ack
* 
* @brief 换装请求回复
**/
class Pro_EquipItem_ack : public AppProtocol<Pro_EquipItem_ack>
{ 
	typedef AppProtocol<Pro_EquipItem_ack> inherit;

public:
	Pro_EquipItem_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, ret_)
		LOAD_INT32_PRO( pdata, totlen, itemid_)
		LOAD_INT16_PRO( pdata, totlen, bagslot_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, ret_)
		SAVE_INT32_PRO( pdata, buflen, len, itemid_)
		SAVE_INT16_PRO( pdata, buflen, len, bagslot_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_EquipItem_ack, proo)
		CLONE_VAR_PRO(proo, ret_)
		CLONE_VAR_PRO(proo, itemid_)
		CLONE_VAR_PRO(proo, bagslot_)
	END_CLONE_PRO()

public:
	//0:成功 1:物品不能被装备 2:卸下的位置不为空或者不存在
	S_INT_8		ret_;
	//物品id
	S_INT_32	itemid_;
	//>=0:卸下物品，并放到指定的bagslot -1:装上物品
	S_INT_16	bagslot_;
};

/**
* @class Pro_EquipItem_ntf
* 
* @brief 换装通知
**/
class Pro_EquipItem_ntf : public AppProtocol<Pro_EquipItem_ntf> 
{ 
	typedef AppProtocol<Pro_EquipItem_ntf> inherit;

public:
	Pro_EquipItem_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, charid_)
		LOAD_INT32_PRO( pdata, totlen, itemcode_)
		LOAD_INT8_PRO( pdata, totlen, operate_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, charid_)
		SAVE_INT32_PRO( pdata, buflen, len, itemcode_)
		SAVE_INT8_PRO( pdata, buflen, len, operate_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_EquipItem_ntf, proo)
		CLONE_VAR_PRO(proo, charid_)
		CLONE_VAR_PRO(proo, itemcode_)
		CLONE_VAR_PRO(proo, operate_)
	END_CLONE_PRO()

public:
	// 角色id
	S_INT_32	charid_;
	//物品code
	S_INT_32	itemcode_;
	//0:装上 1:卸下
	S_INT_8		operate_;
}; 

PROTOCOL_NAMESPACE_END

#endif	//__PRO_AVATAR__H__
