/**
* skill module
*
* @category		data package define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __DTPKG_SKILL__H__
#define __DTPKG_SKILL__H__

#include "prolib/SerializeObject.h"
#include "prolib/datastore/skill_define.h"

PROTOCOL_NAMESPACE_BEGIN

/**
* @class skpkg_base
* 
* @brief 技能包基类
**/
template<class _T>
class skpkg_base : public AppSerializeObject<_T>
{
	typedef AppSerializeObject<_T> inherit;
public:
	skpkg_base( S_INT_8 fctype, S_INT_16 siid):inherit(fctype,siid){}
	skpkg_base();

	BEGIN_LOAD_SERIALIZEOBJ( _pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, skillid_)
		LOAD_TIMESTAMP_PRO( pdata, totlen, last_runtime_)
	END_LOAD_SERIALIZEOBJ()

	BEGIN_SAVE_SERIALIZEOBJ( _pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, skillid_)
		SAVE_TIMESTAMP_PRO( pdata, buflen, len, last_runtime_)
	END_SAVE_SERIALIZEOBJ()

	BEGIN_CLONE_SERIALIZEOBJ( skpkg_base, proo)
		CLONE_VAR_PRO( proo, skillid_)
		CLONE_VAR_PRO( proo, last_runtime_)
	END_CLONE_SERIALIZEOBJ() 
public:
	//技能id
	S_INT_32	skillid_;
	//最后一次使用时间
	S_TIMESTAMP	last_runtime_;
};

/**
* @class skpkg_nearfight
* 
* @brief 近身攻击技能包	- SKILLDATAPKG_NEARFIGHT
**/
class skpkg_nearfight : public skpkg_base<skpkg_nearfight>
{
	typedef skpkg_base<skpkg_nearfight> inherit;
public:
	skpkg_nearfight();

	BEGIN_LOAD_SERIALIZEOBJ( _pdata, totlen, ext)
		INHERIT_LOAD_SERIALIZEOBJ( inherit, _pdata, totlen, ext)
	END_LOAD_SERIALIZEOBJ()

	BEGIN_SAVE_SERIALIZEOBJ( _pdata, buflen, len, ext)
		INHERIT_SAVE_SERIALIZEOBJ( inherit, _pdata, buflen, len, ext)
	END_SAVE_SERIALIZEOBJ()

	BEGIN_CLONE_SERIALIZEOBJ( skpkg_nearfight, proo)
		INHERIT_CLONE_SERIALIZEOBJ( inherit, proo)
	END_CLONE_SERIALIZEOBJ() 

public:
};

/**
* @class skpkg_farfight
* 
* @brief 远程攻击技能包	- SKILLDATAPKG_FARFIGHT
**/
class skpkg_farfight : public skpkg_base<skpkg_farfight>
{
	typedef skpkg_base<skpkg_farfight> inherit;
public:
	skpkg_farfight();

	BEGIN_LOAD_SERIALIZEOBJ( _pdata, totlen, ext)
		INHERIT_LOAD_SERIALIZEOBJ( inherit, _pdata, totlen, ext)
		LOAD_INT32_PRO( pdata, totlen, remain_cool_time_)
	END_LOAD_SERIALIZEOBJ()

	BEGIN_SAVE_SERIALIZEOBJ( _pdata, buflen, len, ext)
		INHERIT_SAVE_SERIALIZEOBJ( inherit, _pdata, buflen, len, ext)
		SAVE_INT32_PRO( pdata, buflen, len, remain_cool_time_)
	END_SAVE_SERIALIZEOBJ()

	BEGIN_CLONE_SERIALIZEOBJ( skpkg_farfight, proo)
		INHERIT_CLONE_SERIALIZEOBJ( inherit, proo)
		CLONE_VAR_PRO( proo, remain_cool_time_)
	END_CLONE_SERIALIZEOBJ() 

public:
	//剩余的冷却时间
	S_INT_32	remain_cool_time_;
};

PROTOCOL_NAMESPACE_END

#endif	//__DTPKG_SKILL__H__
