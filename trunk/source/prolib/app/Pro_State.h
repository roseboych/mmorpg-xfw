/**
* protocol module
*
* 定义了player的状态管理协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_STATE__H__
#define __PRO_STATE__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/BasicProtocol.h"
#include "prolib/SerializeFactoryContainer.h"
#include "prolib/datastore/character_define.h"

#include <list>

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_PlayerMove_req
* 
* @brief 玩家移动请求
**/
class Pro_PlayerMove_req : public AppProtocol<Pro_PlayerMove_req> 
{ 
	typedef AppProtocol<Pro_PlayerMove_req> inherit;
public: 
	Pro_PlayerMove_req();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_FLOAT32_PRO( pdata, buflen, locationx_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationy_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationz_)
		LOAD_FLOAT32_PRO( pdata, buflen, facing_)
		LOAD_INT8_PRO( pdata, buflen, curstate_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, facing_)
		SAVE_INT8_PRO( pdata, buflen, len, curstate_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PlayerMove_req, proo)
		CLONE_VAR_PRO(proo, locationx_)
		CLONE_VAR_PRO(proo, locationy_)
		CLONE_VAR_PRO(proo, locationz_)
		CLONE_VAR_PRO(proo, facing_)
		CLONE_VAR_PRO(proo, curstate_)
	END_CLONE_PRO()

public: 
	S_FLOAT_32	locationx_;
	S_FLOAT_32	locationy_;
	S_FLOAT_32	locationz_;
	S_FLOAT_32	facing_;
	//当前状态
	S_INT_8		curstate_;
}; 

/**
* @class Pro_PlayerMove_ack
* 
* @brief 玩家移动回复，只在失败的情况下回复
**/
class Pro_PlayerMove_ack : public AppProtocol<Pro_PlayerMove_ack> 
{ 
	typedef AppProtocol<Pro_PlayerMove_ack> inherit;
public: 
	Pro_PlayerMove_ack();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT8_PRO( pdata, buflen, result_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationx_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationy_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationz_)
		LOAD_FLOAT32_PRO( pdata, buflen, facing_)
		LOAD_INT8_PRO( pdata, buflen, curstate_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, facing_)
		SAVE_INT8_PRO( pdata, buflen, len, curstate_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PlayerMove_ack, proo)
		CLONE_VAR_PRO(proo, result_)
		CLONE_VAR_PRO(proo, locationx_)
		CLONE_VAR_PRO(proo, locationy_)
		CLONE_VAR_PRO(proo, locationz_)
		CLONE_VAR_PRO(proo, facing_)
		CLONE_VAR_PRO(proo, curstate_)
	END_CLONE_PRO()

public: 
	S_INT_8		result_; // 1:非法移动 2:非法状态
	S_FLOAT_32	locationx_;
	S_FLOAT_32	locationy_;
	S_FLOAT_32	locationz_;
	S_FLOAT_32	facing_;
	S_INT_8		curstate_;
}; 

/**
* @class Pro_PlayerMove_ntf
* 
* @brief 玩家移动通知
**/
class Pro_PlayerMove_ntf : public AppProtocol<Pro_PlayerMove_ntf> 
{ 
	typedef AppProtocol<Pro_PlayerMove_ntf> inherit;
public: 
	Pro_PlayerMove_ntf();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT32_PRO( pdata, buflen, chrid_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationx_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationy_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationz_)
		LOAD_FLOAT32_PRO( pdata, buflen, facing_)
		LOAD_INT8_PRO( pdata, buflen, curstate_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, facing_)
		SAVE_INT8_PRO( pdata, buflen, len, curstate_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_PlayerMove_ntf, proo )
		CLONE_VAR_PRO(proo, chrid_)
		CLONE_VAR_PRO(proo, locationx_)
		CLONE_VAR_PRO(proo, locationy_)
		CLONE_VAR_PRO(proo, locationz_)
		CLONE_VAR_PRO(proo, facing_)
		CLONE_VAR_PRO(proo, curstate_)
	END_CLONE_PRO()

public: 
	S_INT_32	chrid_;
	S_FLOAT_32	locationx_;
	S_FLOAT_32	locationy_;
	S_FLOAT_32	locationz_;
	S_FLOAT_32	facing_;
	S_INT_8		curstate_;
}; 

/**
* @class Pro_DBPosRotSave_ntf
* 
* @brief 普通数据保存通知
**/
class Pro_DBPosRotSave_ntf : public AppProtocol<Pro_DBPosRotSave_ntf>
{
	typedef AppProtocol<Pro_DBPosRotSave_ntf> inherit;

public:
	Pro_DBPosRotSave_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposx_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposy_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposz_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastfacing_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastfacing_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_DBPosRotSave_ntf, proo)
		CLONE_VAR_PRO( proo, lastposx_)
		CLONE_VAR_PRO( proo, lastposy_)
		CLONE_VAR_PRO( proo, lastposz_)
		CLONE_VAR_PRO( proo, lastfacing_)
	END_CLONE_PRO()

public:
	//坐标
	S_FLOAT_32	lastposx_, lastposy_, lastposz_;
	//面向
	S_FLOAT_32	lastfacing_;
};

/**
* @class Pro_DBPlayerBaseInfo_sav
* 
* @brief 普通数据保存通知
**/
class Pro_DBPlayerBaseInfo_sav : public AppProtocol<Pro_DBPlayerBaseInfo_sav>
{
	typedef AppProtocol<Pro_DBPlayerBaseInfo_sav> inherit;

public:
	Pro_DBPlayerBaseInfo_sav();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.moneys_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.level_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.bagslots_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.skill_[0])
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.skill_[1])
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.hp_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.mp_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.power_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.agile_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.endurance_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.intellect_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.spirit_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.armor_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.died_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.exp_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.moneys_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.level_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.bagslots_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.skill_[0])
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.skill_[1])
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.hp_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.mp_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.power_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.agile_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.endurance_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.intellect_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.spirit_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.armor_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.died_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.exp_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_DBPlayerBaseInfo_sav, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, baseinfo_)
	END_CLONE_PRO()

public:
	//角色id
	S_INT_32	chrid_;
	//玩家基本信息
	character_baseinfo	baseinfo_;
};

PROTOCOL_NAMESPACE_END

#endif	//__PRO_STATE__H__
