/**
* protocol module
*
* 定义了AOI协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_AOI__H__
#define __PRO_AOI__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/boost.h"
#include "prolib/BasicProtocol.h"

#include <list>

#pragma warning(disable:4121)

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_AppUnitEnter_ntf
* 
* @brief 其他玩家进入视野
**/
class Pro_AppUnitEnter_ntf : public AppProtocol<Pro_AppUnitEnter_ntf> 
{ 
    typedef AppProtocol<Pro_AppUnitEnter_ntf> inherit;
public: 
	typedef struct player_info
	{
		S_INT_32 chrid_;				// 角色id
		StaticString<CHR_NICKNAME_LEN>	nickname_;	// 角色名称
		S_INT_8 sex_;					// 性别
		S_INT_8	race_;
		S_INT_8	profession_;
//		S_INT_16 equipments_len; 
//		S_INT_32 equipments[13];		// 角色装备ID数据
		S_INT_32 petid_;				// -1为无效值
		S_INT_32 petcode_;				// 宠物怪物id
		StaticString<CHR_PETNAME_LEN> petname_;			// 宠物名称
		S_FLOAT_32 locationx_;			// 位置
		S_FLOAT_32 locationy_;			// 位置
		S_FLOAT_32 locationz_;			// 位置
		S_FLOAT_32 facing_;				// 旋转
		S_INT_8 state_;
	}player_info;

public: 
    Pro_AppUnitEnter_ntf();

protected: 
    BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_LIST_PRO_BEGIN( pdata, buflen, player_info)
			LOAD_INT32_PRO_LS( pdata, buflen, chrid_)
			LOAD_STR_PRO_LS( pdata, buflen, nickname_)
			LOAD_INT8_PRO_LS( pdata, buflen, sex_)
			LOAD_INT8_PRO_LS( pdata, buflen, race_)
			LOAD_INT8_PRO_LS( pdata, buflen, profession_)
//			LOAD_ARRAY_PRO_LS( pdata, buflen, 4, equipments, equipments_len)
			LOAD_INT32_PRO_LS( pdata, buflen, petid_)
			LOAD_INT32_PRO_LS( pdata, buflen, petcode_)
			LOAD_STR_PRO_LS( pdata, buflen, petname_)
			LOAD_FLOAT32_PRO_LS( pdata, buflen, locationx_)
			LOAD_FLOAT32_PRO_LS( pdata, buflen, locationy_)
			LOAD_FLOAT32_PRO_LS( pdata, buflen, locationz_)
			LOAD_FLOAT32_PRO_LS( pdata, buflen, facing_)
			LOAD_INT8_PRO_LS( pdata, buflen, state_)
		LOAD_LIST_PRO_END( players_ )
    END_LOAD_PRO() 

    BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, players_, player_info)
			SAVE_INT32_PRO_LS( pdata, buflen, len, chrid_)
			SAVE_STR_PRO_LS( pdata, buflen, len, nickname_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, sex_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, race_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, profession_)
//			SAVE_ARRAY_PRO_LS( pdata, buflen, len, 4, equipments, equipments_len)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petid_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petcode_)
			SAVE_STR_PRO_LS( pdata, buflen, len, petname_)
			SAVE_FLOAT32_PRO_LS( pdata, buflen, len, locationx_)
			SAVE_FLOAT32_PRO_LS( pdata, buflen, len, locationy_)
			SAVE_FLOAT32_PRO_LS( pdata, buflen, len, locationz_)
			SAVE_FLOAT32_PRO_LS( pdata, buflen, len, facing_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, state_)
		SAVE_LIST_PRO_END()
    END_SAVE_PRO() 

    BEGIN_CLONE_PRO( Pro_AppUnitEnter_ntf, proo )
		CLONE_LIST_PRO(proo, players_)
    END_CLONE_PRO() 

public:
	bool put_unitinfo( player_info& info) {
		if( players_.size() >= 42) return false;
		else players_.push_back( info);
		return true;
	}

public: 
	S_INT_32 pro_len_;
	std::list<player_info> players_; // 进入玩家视野的列表
};

/**
* @class Pro_AppUnitLeave_ntf
* 
* @brief 其他玩家离开视野
**/
class Pro_AppUnitLeave_ntf : public AppProtocol<Pro_AppUnitLeave_ntf> 
{ 
    typedef AppProtocol<Pro_AppUnitLeave_ntf> inherit;
public: 
	typedef struct player_info
	{
		S_INT_32 chrid_; // 角色id
	}player_info;

public: 
    Pro_AppUnitLeave_ntf();

protected:
    BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_LIST_PRO_BEGIN( pdata, buflen, player_info)
			LOAD_INT32_PRO_LS( pdata, buflen, chrid_)
		LOAD_LIST_PRO_END( players_)
    END_LOAD_PRO()

    BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, players_, player_info)
			SAVE_INT32_PRO_LS( pdata, buflen, len, chrid_)
		SAVE_LIST_PRO_END()
    END_SAVE_PRO()

    BEGIN_CLONE_PRO( Pro_AppUnitLeave_ntf, proo )
		CLONE_LIST_PRO( proo, players_)
    END_CLONE_PRO()

public: 
	S_INT_32 pro_len_;
	
	bool put_player( S_INT_32 chrid) {
		player_info i; i.chrid_=chrid;
		if( players_.size() >= 180) return false;
		else players_.push_back( i);
		return true;
	}

public: 
	std::list<player_info> players_; // 进入离开视野的列表
};

/**
* @class Pro_AppTeleport_req
* 
* @brief 地图切换请求
**/
class Pro_AppTeleport_req : public AppProtocol<Pro_AppTeleport_req> 
{ 
    typedef AppProtocol<Pro_AppTeleport_req> inherit;
public: 
    Pro_AppTeleport_req();

protected: 
    BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT8_PRO( pdata, buflen, telid_)
		LOAD_INT8_PRO( pdata, buflen, teltoid_)
    END_LOAD_PRO()

    BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, telid_)
		SAVE_INT8_PRO( pdata, buflen, len, teltoid_)
    END_SAVE_PRO()

    BEGIN_CLONE_PRO( Pro_AppTeleport_req, proo )
		CLONE_VAR_PRO( proo, telid_)
		CLONE_VAR_PRO( proo, teltoid_)
    END_CLONE_PRO()

public:
	S_INT_8 telid_; // 使用的teleport配置
	S_INT_8 teltoid_; // teleport配置下的to的id
};

/**
* @class Pro_AppTeleport_ack
* 
* @brief 地图切换响应
**/
class Pro_AppTeleport_ack : public AppProtocol<Pro_AppTeleport_ack> 
{ 
    typedef AppProtocol<Pro_AppTeleport_ack> inherit;
public: 
    Pro_AppTeleport_ack();

protected: 
    BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT8_PRO( pdata, buflen, result_)
		LOAD_INT32_PRO( pdata, buflen, cssindex_)
		LOAD_INT32_PRO( pdata, buflen, chrid_)
		LOAD_INT32_PRO( pdata, buflen, mapid_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationx_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationy_)
		LOAD_FLOAT32_PRO( pdata, buflen, locationz_)
		LOAD_FLOAT32_PRO( pdata, buflen, facing_)
    END_LOAD_PRO() 

    BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, cssindex_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT32_PRO( pdata, buflen, len, mapid_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, locationz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, facing_)
    END_SAVE_PRO() 

    BEGIN_CLONE_PRO( Pro_AppTeleport_ack, proo )
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, cssindex_)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, mapid_)
		CLONE_VAR_PRO( proo, locationx_)
		CLONE_VAR_PRO( proo, locationy_)
		CLONE_VAR_PRO( proo, locationz_)
		CLONE_VAR_PRO( proo, facing_)
    END_CLONE_PRO()

public:
	// 0:成功 1:转跳点无效 2:必须先进入世界 3:同一个地图内 
	// 4:开始点不是当前地图 5:目标地图不在服务器组支持中 6:玩家正在进入副本
	S_INT_8		result_;
	S_INT_32	mapid_; // 地图id
	S_FLOAT_32	locationx_; // 位置
	S_FLOAT_32	locationy_; // 位置
	S_FLOAT_32	locationz_; // 位置
	S_FLOAT_32	facing_; // 旋转
	//以下为服务器使用
	S_INT_32	cssindex_;
	S_INT_32	chrid_;
};

/**
* @class Pro_AppEnterIns_req
* 
* @brief 进入副本请求
**/
class Pro_AppEnterIns_req : public AppProtocol<Pro_AppEnterIns_req> 
{ 
	typedef AppProtocol<Pro_AppEnterIns_req> inherit;
public: 
	Pro_AppEnterIns_req();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT8_PRO( pdata, buflen, telid_)
		LOAD_FLOAT32_PRO( pdata, buflen, posx_)
		LOAD_FLOAT32_PRO( pdata, buflen, posy_)
		LOAD_FLOAT32_PRO( pdata, buflen, posz_)
		LOAD_INT32_PRO( pdata, buflen, cellid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, telid_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posz_)
		SAVE_INT32_PRO( pdata, buflen, len, cellid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_AppEnterIns_req, proo )
		CLONE_VAR_PRO( proo, telid_)
		CLONE_VAR_PRO( proo, posx_)
		CLONE_VAR_PRO( proo, posy_)
		CLONE_VAR_PRO( proo, posz_)
		CLONE_VAR_PRO( proo, cellid_)
	END_CLONE_PRO()

public:
	S_INT_8		telid_;		// 副本转跳点配置id
	//服务器使用
	S_FLOAT_32	posx_;		// 位置 css->cts
	S_FLOAT_32	posy_;		// 位置
	S_FLOAT_32	posz_;		// 位置
	S_INT_32	cellid_;	//副本服务器cellid, cts->css
};

/**
* @class Pro_AppEnterIns_ack
* 
* @brief 进入副本回复
**/
class Pro_AppEnterIns_ack : public AppProtocol<Pro_AppEnterIns_ack> 
{ 
	typedef AppProtocol<Pro_AppEnterIns_ack> inherit;
public: 
	Pro_AppEnterIns_ack();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT8_PRO( pdata, buflen, result_)
		LOAD_INT32_PRO( pdata, buflen, cssindex_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, cssindex_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_AppEnterIns_ack, proo )
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, cssindex_)
	END_CLONE_PRO()

public:
	// 0:成功 1:转跳点无效 2:必须先进入世界 3:已经存在一个进入副本请求 4:开始点不是当前地图
	// 5:没有可用的副本 6:不能从副本进入副本
	S_INT_8		result_;
	//服务器使用
	S_INT_32	cssindex_;
};

/**
* @class Pro_AppEnterInsOvertime_ntf
* 
* @brief 进入副本超时通知
**/
class Pro_AppEnterInsOvertime_ntf : public AppProtocol<Pro_AppEnterInsOvertime_ntf> 
{ 
	typedef AppProtocol<Pro_AppEnterInsOvertime_ntf> inherit;
public: 
	Pro_AppEnterInsOvertime_ntf();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
		LOAD_INT32_PRO( pdata, buflen, cellid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, cellid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_AppEnterInsOvertime_ntf, proo)
		CLONE_VAR_PRO( proo, cellid_)
	END_CLONE_PRO()

public:
	//cts -> inst maps时使用
	S_INT_32	cellid_;
};

/**
* @class Pro_AppEnterInsConfirm_ntf
* 
* @brief 进入副本预分配占位确认
**/
class Pro_AppEnterInsConfirm_ntf : public AppProtocol<Pro_AppEnterInsConfirm_ntf> 
{ 
	typedef AppProtocol<Pro_AppEnterInsConfirm_ntf> inherit;
public: 
	Pro_AppEnterInsConfirm_ntf();

protected: 
	BEGIN_LOAD_PRO( pdata, buflen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_AppEnterInsConfirm_ntf, proo )
	END_CLONE_PRO()

public:
};

PROTOCOL_NAMESPACE_END

#endif 
