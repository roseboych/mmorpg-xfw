/**
* protocol module
*
* 定义了角色类协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PRO_CHR__H__
#define __PRO_CHR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/boost.h"
#include "prolib/BasicProtocol.h"
#include "prolib/datastore/character_define.h"
#include "prolib/datastore/buffer_define.h"

#include <list>

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_ChrList_req
* 
* @brief 角色列表请求
**/
class Pro_ChrList_req : public AppProtocol<Pro_ChrList_req>
{
	typedef AppProtocol<Pro_ChrList_req> inherit;

public:
	Pro_ChrList_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrList_req, proo)
	END_CLONE_PRO()

public:
};

/**
* @class Pro_ChrList_ack
* 
* @brief 角色列表请求
**/
class Pro_ChrList_ack : public AppProtocol<Pro_ChrList_ack>
{
	typedef AppProtocol<Pro_ChrList_ack> inherit;

public:
	struct chrinfo
	{
		//角色id
		S_INT_32	chrid_;
		//角色昵称
		StaticString<CHR_NICKNAME_LEN> name_;
		//种族
		S_INT_8	race_;
		//职业
		S_INT_8	profession_;
		//性别 0 男的， 1女的
		S_INT_8	sex_;
		//hp
		S_INT_32	hp_;
		//mp
		S_INT_32	mp_;
		//exp
		S_INT_32	exp_;
		//角色等级
		S_INT_32	level_;
		//moneys
		S_INT_32	moneys_;
		//petid    宠物实例的ID
		S_INT_32	petid_;
		//petcode  宠物配置值ID
		S_INT_32	petcode_;
		//petname
		StaticString<CHR_PETNAME_LEN>	petname_;
		//角色装备id
		S_INT_16	equiplen_;
		S_INT_32	equips_[CHRITEMS_MAX];
	};

	typedef NS_STL::list< chrinfo> CHR_LIST;

public:
	Pro_ChrList_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_LIST_PRO_BEGIN( pdata, totlen, chrinfo)
			LOAD_INT32_PRO_LS( pdata, totlen, chrid_)
			LOAD_STR_PRO_LS( pdata, totlen, name_)
			LOAD_INT8_PRO_LS( pdata, totlen, race_)
			LOAD_INT8_PRO_LS( pdata, totlen, profession_)
			LOAD_INT8_PRO_LS( pdata, totlen, sex_)
			LOAD_INT32_PRO_LS( pdata, totlen, hp_)
			LOAD_INT32_PRO_LS( pdata, totlen, mp_)
			LOAD_INT32_PRO_LS( pdata, totlen, exp_)
			LOAD_INT32_PRO_LS( pdata, totlen, level_)
			LOAD_INT32_PRO_LS( pdata, totlen, moneys_)
			LOAD_INT32_PRO_LS( pdata, totlen, petid_)
			LOAD_INT32_PRO_LS( pdata, totlen, petcode_)
			LOAD_STR_PRO_LS( pdata, totlen, petname_)
			LOAD_ARRAY_PRO_LS( pdata, totlen, 4, equips_, equiplen_)
		LOAD_LIST_PRO_END( chrs_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, chrs_, chrinfo)
			SAVE_INT32_PRO_LS( pdata, buflen, len, chrid_)
			SAVE_STR_PRO_LS( pdata, buflen, len, name_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, race_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, profession_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, sex_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, hp_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, mp_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, exp_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, level_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, moneys_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petid_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petcode_)
			SAVE_STR_PRO_LS( pdata, buflen, len, petname_)
			SAVE_ARRAY_PRO_LS( pdata, buflen, len, 4, equips_, equiplen_)
		SAVE_LIST_PRO_END()
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrList_ack, proo)
		CLONE_LIST_PRO( proo, chrs_)
	END_CLONE_PRO()

public:
	CHR_LIST	chrs_;
};

/**
* @class Pro_ChrCreate_req
* 
* @brief 角色创建请求
**/
class Pro_ChrCreate_req : public AppProtocol<Pro_ChrCreate_req>
{
	typedef AppProtocol<Pro_ChrCreate_req> inherit;

public:
	Pro_ChrCreate_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_STR_PRO( pdata, totlen, name_)
		LOAD_INT8_PRO( pdata, totlen, race_)
		LOAD_INT8_PRO( pdata, totlen, profession_)
		LOAD_INT8_PRO( pdata, totlen, sex_)
		LOAD_FLOAT32_PRO( pdata, totlen, posx_)
		LOAD_FLOAT32_PRO( pdata, totlen, posy_)
		LOAD_FLOAT32_PRO( pdata, totlen, posz_)
		LOAD_FLOAT32_PRO( pdata, totlen, facing_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_STR_PRO( pdata, buflen, len, name_)
		SAVE_INT8_PRO( pdata, buflen, len, race_)
		SAVE_INT8_PRO( pdata, buflen, len, profession_)
		SAVE_INT8_PRO( pdata, buflen, len, sex_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, posz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, facing_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrCreate_req, proo)
		CLONE_VAR_PRO( proo, name_)
		CLONE_VAR_PRO( proo, race_)
		CLONE_VAR_PRO( proo, profession_)
		CLONE_VAR_PRO( proo, sex_)
		CLONE_VAR_PRO( proo, posx_)
		CLONE_VAR_PRO( proo, posy_)
		CLONE_VAR_PRO( proo, posz_)
		CLONE_VAR_PRO( proo, facing_)
	END_CLONE_PRO()

public:
	//角色昵称
	StaticString<CHR_NICKNAME_LEN>	name_;
	//种族 1:人类
	S_INT_8	race_;
	//职业 1:战士 2:法师
	S_INT_8 profession_;
	//性别 0 男的， 1女的
	S_INT_8	sex_;
	//服务器使用
	S_FLOAT_32 posx_, posy_, posz_, facing_;
};

/**
* @class Pro_ChrCreate_ack
* 
* @brief 角色创建请求
**/
class Pro_ChrCreate_ack : public AppProtocol<Pro_ChrCreate_ack>
{
	typedef AppProtocol<Pro_ChrCreate_ack> inherit;

public:
	Pro_ChrCreate_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_STR_PRO( pdata, totlen, name_)
		LOAD_INT8_PRO( pdata, totlen, race_)
		LOAD_INT8_PRO( pdata, totlen, profession_)
		LOAD_INT8_PRO( pdata, totlen, sex_)
		LOAD_INT32_PRO( pdata, totlen, hp_)
		LOAD_INT32_PRO( pdata, totlen, mp_)
		LOAD_INT32_PRO( pdata, totlen, exp_)
		LOAD_INT32_PRO( pdata, totlen, level_)
		LOAD_INT32_PRO( pdata, totlen, moneys_)
		LOAD_INT32_PRO( pdata, totlen, petid_)
		LOAD_INT32_PRO( pdata, totlen, petcode_)
		LOAD_STR_PRO( pdata, totlen, petname_)
		LOAD_ARRAY_PRO( pdata, totlen, 4, equips_, equiplen_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_STR_PRO( pdata, buflen, len, name_)
		SAVE_INT8_PRO( pdata, buflen, len, race_)
		SAVE_INT8_PRO( pdata, buflen, len, profession_)
		SAVE_INT8_PRO( pdata, buflen, len, sex_)
		SAVE_INT32_PRO( pdata, buflen, len, hp_)
		SAVE_INT32_PRO( pdata, buflen, len, mp_)
		SAVE_INT32_PRO( pdata, buflen, len, exp_)
		SAVE_INT32_PRO( pdata, buflen, len, level_)
		SAVE_INT32_PRO( pdata, buflen, len, moneys_)
		SAVE_INT32_PRO( pdata, buflen, len, petid_)
		SAVE_INT32_PRO( pdata, buflen, len, petcode_)
		SAVE_STR_PRO( pdata, buflen, len, petname_)
		SAVE_ARRAY_PRO( pdata, buflen, len, 4, equips_, equiplen_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrCreate_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, name_)
		CLONE_VAR_PRO( proo, race_)
		CLONE_VAR_PRO( proo, profession_)
		CLONE_VAR_PRO( proo, sex_)
		CLONE_VAR_PRO( proo, hp_)
		CLONE_VAR_PRO( proo, mp_)
		CLONE_VAR_PRO( proo, exp_)
		CLONE_VAR_PRO( proo, level_)
		CLONE_VAR_PRO( proo, moneys_)
		CLONE_VAR_PRO( proo, petid_)
		CLONE_VAR_PRO( proo, petcode_)
		CLONE_VAR_PRO( proo, petname_)
		CLONE_ARRAY_PRO( proo, 4, equips_, equiplen_)
	END_CLONE_PRO()

public:
	//0成功，1:必须先获取角色列表 2:超过最大角色数 3:其他错误 4:昵称重复
	S_INT_8	result_;
	//角色id
	S_INT_32	chrid_;
	//角色昵称
	StaticString<CHR_NICKNAME_LEN>	name_;
	//种族
	S_INT_8	race_;
	//职业
	S_INT_8	profession_;
	//性别 0 男的， 1女的
	S_INT_8	sex_;
	//hp
	S_INT_32	hp_;
	//mp
	S_INT_32	mp_;
	//exp
	S_INT_32	exp_;
	//角色等级
	S_INT_32	level_;
	//moneys
	S_INT_32	moneys_;
	//petid    宠物实例的ID
	S_INT_32	petid_;
	//petcode  宠物配置值ID
	S_INT_32	petcode_;
	//petname
	StaticString<CHR_PETNAME_LEN>	petname_;
	//角色装备id 
	S_INT_16	equiplen_;
	S_INT_32	equips_[CHRITEMS_MAX];
};

/**
* @class Pro_ChrSel_req
* 
* @brief 角色选择请求
**/
class Pro_ChrSel_req : public AppProtocol<Pro_ChrSel_req>
{
	typedef AppProtocol<Pro_ChrSel_req> inherit;

public:
	Pro_ChrSel_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrSel_req, proo)
		CLONE_VAR_PRO( proo, chrid_)
	END_CLONE_PRO()

public:
	//角色id
	S_INT_32	chrid_;
};

/**
* @class Pro_ChrSel_ack
* 
* @brief 角色选择请求
**/
class Pro_ChrSel_ack : public AppProtocol<Pro_ChrSel_ack>
{
	typedef AppProtocol<Pro_ChrSel_ack> inherit;

public:
	Pro_ChrSel_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT8_PRO( pdata, totlen, race_)
		LOAD_INT8_PRO( pdata, totlen, profession_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposx_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposy_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastposz_)
		LOAD_FLOAT32_PRO( pdata, totlen, lastfacing_)
		LOAD_INT32_PRO( pdata, totlen, lastmap_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT8_PRO( pdata, buflen, len, race_)
		SAVE_INT8_PRO( pdata, buflen, len, profession_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastposz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, lastfacing_)
		SAVE_INT32_PRO( pdata, buflen, len, lastmap_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrSel_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, race_)
		CLONE_VAR_PRO( proo, profession_)
		CLONE_VAR_PRO( proo, lastposx_)
		CLONE_VAR_PRO( proo, lastposy_)
		CLONE_VAR_PRO( proo, lastposz_)
		CLONE_VAR_PRO( proo, lastfacing_)
		CLONE_VAR_PRO( proo, lastmap_)
	END_CLONE_PRO()

public:
	//0成功，1:先获取角色列表 2:选择的角色不存在 3:其他错误
	S_INT_8		result_;
	//角色id
	S_INT_32	chrid_;
	//种族
	S_INT_8		race_;
	//职业
	S_INT_8		profession_;
	//[服务器使用]最后使用的位置
	S_FLOAT_32	lastposx_, lastposy_, lastposz_;
	S_FLOAT_32	lastfacing_;
	//最后的地图
	S_INT_32	lastmap_;
};

/**
* @class Pro_ChrDel_req
* 
* @brief 角色删除请求
**/
class Pro_ChrDel_req : public AppProtocol<Pro_ChrDel_req>
{
	typedef AppProtocol<Pro_ChrDel_req> inherit;

public:
	Pro_ChrDel_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrDel_req, proo)
		CLONE_VAR_PRO( proo, chrid_)
	END_CLONE_PRO()

public:
	//角色id
	S_INT_32	chrid_;
};

/**
* @class Pro_ChrDel_ack
* 
* @brief 角色删除请求
**/
class Pro_ChrDel_ack : public AppProtocol<Pro_ChrDel_ack>
{
	typedef AppProtocol<Pro_ChrDel_ack> inherit;

public:
	Pro_ChrDel_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, result_)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, result_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrDel_ack, proo)
		CLONE_VAR_PRO( proo, result_)
		CLONE_VAR_PRO( proo, chrid_)
	END_CLONE_PRO()

public:
	//0成功，1:先获取角色列表 2:删除的角色不存在 3:其他错误
	S_INT_8		result_;
	//角色id
	S_INT_32	chrid_;
};

/**
* @class Pro_ChrRegistToWorld_req
* 
* @brief 玩家注册到世界地图 cts to css, css to gts
**/
class Pro_ChrRegistToWorld_req : public AppProtocol<Pro_ChrRegistToWorld_req>
{
	typedef AppProtocol<Pro_ChrRegistToWorld_req> inherit;

public:
	Pro_ChrRegistToWorld_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT32_PRO( pdata, totlen, mapid_)
		LOAD_INT32_PRO( pdata, totlen, cssindex_)
		LOAD_INT8_PRO( pdata, totlen, is_quitinst_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT32_PRO( pdata, buflen, len, mapid_)
		SAVE_INT32_PRO( pdata, buflen, len, cssindex_)
		SAVE_INT8_PRO( pdata, buflen, len, is_quitinst_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrRegistToWorld_req, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, mapid_)
		CLONE_VAR_PRO( proo, cssindex_)
		CLONE_VAR_PRO( proo, is_quitinst_)
	END_CLONE_PRO()

public:
	//角色id
	S_INT_32	chrid_;
	//地图id
	S_INT_32	mapid_;
	//gts使用
	S_INT_32	cssindex_;
	//是否是退出副本时触发的事件 0:no 1:yes
	S_INT_8		is_quitinst_;
};

/**
* @class Pro_ChrLoad_req
* 
* @brief 玩家角色加载请求 css to dpx
**/
class Pro_ChrLoad_req : public AppProtocol<Pro_ChrLoad_req>
{
	typedef AppProtocol<Pro_ChrLoad_req> inherit;

public:
	Pro_ChrLoad_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_INT32_PRO( pdata, totlen, cssindex_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_INT32_PRO( pdata, buflen, len, cssindex_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrLoad_req, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_VAR_PRO( proo, cssindex_)
	END_CLONE_PRO()

public:
	//角色id
	S_INT_32	chrid_;
	S_INT_32	cssindex_;
};

/**
* @class Pro_ChrLoad_ack
* 
* @brief 玩家角色加载请求 dpx to css
**/
class Pro_ChrLoad_ack : public AppProtocol<Pro_ChrLoad_ack>
{
	typedef AppProtocol<Pro_ChrLoad_ack> inherit;
public:
	typedef	NS_STL::list<equipment_item>	EQUIPITEM_LIST;
	typedef	NS_STL::list<petdata_item>		PETDATA_LIST;
	typedef	NS_STL::list<bufferdata_item>	BUFFERDATA_LIST;

public:
	Pro_ChrLoad_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_LIST_PRO_BEGIN( pdata, totlen, equipment_item)
			LOAD_INT32_PRO_LS( pdata, totlen, itemid_)
			LOAD_INT16_PRO_LS( pdata, totlen, inavatar_)
			LOAD_INT16_PRO_LS( pdata, totlen, inpart_)
			LOAD_INT32_PRO_LS( pdata, totlen, itemcode_)
			LOAD_INT16_PRO_LS( pdata, totlen, validatetype_)
			LOAD_INT32_PRO_LS( pdata, totlen, starttime_)
			LOAD_INT32_PRO_LS( pdata, totlen, endtime_)
		LOAD_LIST_PRO_END( items_)
		LOAD_LIST_PRO_BEGIN( pdata, totlen, petdata_item)
			LOAD_INT32_PRO_LS( pdata, totlen, petid_)
			LOAD_INT32_PRO_LS( pdata, totlen, petcode_)
			LOAD_STR_PRO_LS( pdata, totlen, petname_)
		LOAD_LIST_PRO_END( pets_)
		LOAD_LIST_PRO_BEGIN( pdata, totlen, bufferdata_item)
			LOAD_INT32_PRO_LS( pdata, totlen, bufferid_)
			LOAD_INT8_PRO_LS( pdata, totlen, types_)
			LOAD_INT32_PRO_LS( pdata, totlen, createtime_)
			LOAD_INT32_PRO_LS( pdata, totlen, scopetime_)
		LOAD_LIST_PRO_END( buffers_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, items_, equipment_item)
			SAVE_INT32_PRO_LS( pdata, buflen, len, itemid_)
			SAVE_INT16_PRO_LS( pdata, buflen, len, inavatar_)
			SAVE_INT16_PRO_LS( pdata, buflen, len, inpart_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, itemcode_)
			SAVE_INT16_PRO_LS( pdata, buflen, len, validatetype_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, starttime_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, endtime_)
		SAVE_LIST_PRO_END()
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, pets_, petdata_item)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petid_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, petcode_)
			SAVE_STR_PRO_LS( pdata, buflen, len, petname_)
		SAVE_LIST_PRO_END()
		SAVE_LIST_PRO_BEGIN( pdata, buflen, len, buffers_, bufferdata_item)
			SAVE_INT32_PRO_LS( pdata, buflen, len, bufferid_)
			SAVE_INT8_PRO_LS( pdata, buflen, len, types_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, createtime_)
			SAVE_INT32_PRO_LS( pdata, buflen, len, scopetime_)
		SAVE_LIST_PRO_END()
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrLoad_ack, proo)
		CLONE_LIST_PRO( proo, items_)
		CLONE_LIST_PRO( proo, pets_)
		CLONE_LIST_PRO( proo, buffers_)
	END_CLONE_PRO()

public:
	EQUIPITEM_LIST	items_;
	PETDATA_LIST	pets_;
	BUFFERDATA_LIST	buffers_;
};

/**
* @class Pro_ChrFin_NTF
* 
* @brief 选服结束后的玩家信息初始化完成通知
**/
class Pro_ChrFin_NTF : public AppProtocol<Pro_ChrFin_NTF>
{
	typedef AppProtocol<Pro_ChrFin_NTF> inherit;

public:
	Pro_ChrFin_NTF();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_STR_PRO( pdata, totlen, baseinfo_.name_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.race_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.profession_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.sex_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.moneys_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.level_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.bagslots_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.skill_[0])
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.skill_[1])
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.petid_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.hp_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.mp_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.power_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.agile_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.endurance_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.intellect_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.spirit_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.armor_)
		LOAD_FLOAT32_PRO( pdata, totlen, baseinfo_.posx_)
		LOAD_FLOAT32_PRO( pdata, totlen, baseinfo_.posy_)
		LOAD_FLOAT32_PRO( pdata, totlen, baseinfo_.posz_)
		LOAD_FLOAT32_PRO( pdata, totlen, baseinfo_.facing_)
		LOAD_INT8_PRO( pdata, totlen, baseinfo_.died_)
		LOAD_INT32_PRO( pdata, totlen, baseinfo_.exp_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_STR_PRO( pdata, buflen, len, baseinfo_.name_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.race_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.profession_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.sex_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.moneys_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.level_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.bagslots_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.skill_[0])
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.skill_[1])
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.petid_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.hp_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.mp_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.power_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.agile_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.endurance_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.intellect_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.spirit_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.armor_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, baseinfo_.posx_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, baseinfo_.posy_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, baseinfo_.posz_)
		SAVE_FLOAT32_PRO( pdata, buflen, len, baseinfo_.facing_)
		SAVE_INT8_PRO( pdata, buflen, len, baseinfo_.died_)
		SAVE_INT32_PRO( pdata, buflen, len, baseinfo_.exp_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_ChrFin_NTF, proo)
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

#endif //__PRO_CHR__H__
