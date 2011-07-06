/**
* protocol module
*
* 定义了技能使用协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_SKILL__H__
#define __PRO_SKILL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/BasicProtocol.h"
#include "prolib/SerializeFactoryContainer.h"
#include "prolib/datastore/skill_define.h"

#include <list>

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_SkillInit_ntf
* 
* @brief player技能数据初始化
**/
class Pro_SkillInit_ntf : public AppProtocol<Pro_SkillInit_ntf>
{
	typedef AppProtocol<Pro_SkillInit_ntf> inherit;
public:
	typedef NS_STL::list< SerializeObject*>	SKILLPKGDATA_LIST;

public:
	Pro_SkillInit_ntf();
	~Pro_SkillInit_ntf();

	void add_skillinfo( SerializeObject* obj){
		if( obj != 0)
			pkgs_.push_back( obj);
	}

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_SERIALIZEOBJ_LIST_BEGIN( pdata, totlen)
			LOAD_OBJECT_PRO( pdata, totlen, _item)
		LOAD_SERIALIZEOBJ_LIST_END( pkgs_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_SERIALIZEOBJ_LIST_BEGIN( pdata, buflen, len, pkgs_)
			SAVE_OBJECT_PRO( pdata, buflen, len, _item)
		SAVE_SERIALIZEOBJ_LIST_END()
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillInit_ntf, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_SERIALIZEOBJ_LIST_PRO( proo, pkgs_)
	END_CLONE_PRO()

public:
	//技能所属的角色id
	S_INT_32	chrid_;
	//技能数据包内容
	SKILLPKGDATA_LIST	pkgs_;
};

/**
* @class Pro_SkillStateChg_ntf
* 
* @brief 技能数据变化通知
**/
class Pro_SkillStateChg_ntf : public AppProtocol<Pro_SkillStateChg_ntf>
{
	typedef AppProtocol<Pro_SkillStateChg_ntf> inherit;
public:
	typedef NS_STL::list< SerializeObject*>	SKILLPKGDATA_LIST;

public:
	Pro_SkillStateChg_ntf();
	~Pro_SkillStateChg_ntf();

	void add_skillinfo( SerializeObject* obj){
		if( obj != 0)
			pkgs_.push_back( obj);
	}

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_SERIALIZEOBJ_LIST_BEGIN( pdata, totlen)
			LOAD_OBJECT_PRO( pdata, totlen, _item)
		LOAD_SERIALIZEOBJ_LIST_END( pkgs_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_SERIALIZEOBJ_LIST_BEGIN( pdata, buflen, len, pkgs_)
			SAVE_OBJECT_PRO( pdata, buflen, len, _item)
		SAVE_SERIALIZEOBJ_LIST_END()
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillStateChg_ntf, proo)
		CLONE_VAR_PRO( proo, chrid_)
		CLONE_SERIALIZEOBJ_LIST_PRO( proo, pkgs_)
	END_CLONE_PRO()

public:
	//技能所属的角色id
	S_INT_32	chrid_;
	//变化的技能数据包内容
	SKILLPKGDATA_LIST	pkgs_;
};

/**
* @class Pro_SkillStudy_req
* 
* @brief 技能学习请求
**/
class Pro_SkillStudy_req : public AppProtocol<Pro_SkillStudy_req>
{
	typedef AppProtocol<Pro_SkillStudy_req> inherit;

public:
	Pro_SkillStudy_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, skillid_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, skillid_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillStudy_req, proo)
		CLONE_VAR_PRO( proo, skillid_)
	END_CLONE_PRO()

public:
	//需要学习的技能信息
	S_INT_32	skillid_;
};

/**
* @class Pro_SkillStudy_ack
* 
* @brief 技能学习请求回复
**/
class Pro_SkillStudy_ack : public AppProtocol<Pro_SkillStudy_ack>
{
	typedef AppProtocol<Pro_SkillStudy_ack> inherit;

public:
	Pro_SkillStudy_ack();
	~Pro_SkillStudy_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, skillid_)
		LOAD_INT8_PRO( pdata, totlen, ret_)
		LOAD_OBJECT_PRO( pdata, totlen, skillobj_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, skillid_)
		SAVE_INT8_PRO( pdata, buflen, len, ret_)
		SAVE_OBJECT_PRO( pdata, buflen, len, skillobj_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillStudy_ack, proo)
		CLONE_VAR_PRO( proo, skillid_)
		CLONE_VAR_PRO( proo, ret_)
		CLONE_OBJECT_PRO( proo, skillobj_)
	END_CLONE_PRO()

public:
	//需要学习的技能信息
	S_INT_32	skillid_;
	//0:成功 1:技能不存在 2:技能已经学会 <0:未达到学习技能的条件
	S_INT_8		ret_;
	//如果成功附带新生成的技能数据
	SerializeObject*	skillobj_;
};

/**
* @class Pro_SkillUsed_req
* 
* @brief 技能使用请求
**/
class Pro_SkillUsed_req : public AppProtocol<Pro_SkillUsed_req>
{
	typedef AppProtocol<Pro_SkillUsed_req> inherit;

public:
	Pro_SkillUsed_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, skillid_)
		LOAD_INT64_PRO( pdata, totlen, target_unit_)
		LOAD_INT8_PRO( pdata, totlen, curstate_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, skillid_)
		SAVE_INT64_PRO( pdata, buflen, len, target_unit_)
		SAVE_INT8_PRO( pdata, buflen, len, curstate_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillUsed_req, proo)
		CLONE_VAR_PRO( proo, skillid_)
		CLONE_VAR_PRO( proo, target_unit_)
		CLONE_VAR_PRO( proo, curstate_)
	END_CLONE_PRO()

public:
	//使用的技能
	S_INT_32	skillid_;
	//攻击对象	-1:如果技能是范围攻击，没有攻击对象.
	S_INT_64	target_unit_;
	//当前的状态
	S_INT_8		curstate_;
};

/**
* @class Pro_SkillUsed_ack
* 
* @brief 技能使用请求
**/
class Pro_SkillUsed_ack : public AppProtocol<Pro_SkillUsed_ack>
{
	typedef AppProtocol<Pro_SkillUsed_ack> inherit;

public:
	Pro_SkillUsed_ack();
	~Pro_SkillUsed_ack();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, skillid_)
		LOAD_INT8_PRO( pdata, totlen, curstate_)
		LOAD_INT64_PRO( pdata, totlen, target_unit_)
		LOAD_INT8_PRO( pdata, totlen, ret_)
		LOAD_OBJECT_PRO( pdata, totlen, skillobj_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, skillid_)
		SAVE_INT8_PRO( pdata, buflen, len, curstate_)
		SAVE_INT64_PRO( pdata, buflen, len, target_unit_)
		SAVE_INT8_PRO( pdata, buflen, len, ret_)
		SAVE_OBJECT_PRO( pdata, buflen, len, skillobj_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_SkillUsed_ack, proo)
		CLONE_VAR_PRO( proo, skillid_)
		CLONE_VAR_PRO( proo, curstate_)
		CLONE_VAR_PRO( proo, target_unit_)
		CLONE_VAR_PRO( proo, ret_)
		CLONE_OBJECT_PRO( proo, skillobj_)
	END_CLONE_PRO()

public:
	S_INT_32	skillid_;
	//当前服务器上的player状态
	S_INT_8		curstate_;
	//包含当前攻击对象。
	S_INT_64	target_unit_;
	//0:成功 1:技能不存在 2:技能处于cooldown状态 3:不能攻击被攻击对象 4:不能切换到指定状态 5:技能在当前状态下不可用 6:不是主动技能
	//<0:技能使用失败的其他原因
	S_INT_8		ret_;
	//如果成功附带新生成的技能状态数据
	SerializeObject*	skillobj_;
};


PROTOCOL_NAMESPACE_END


#endif	//__PRO_SKILL__H__
