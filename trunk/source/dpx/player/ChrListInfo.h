/**
* dpx application
*
* @category		player data
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CHRLISTINFO__H__
#define __CHRLISTINFO__H__

#include "dpx/dpx_pre.h"
#include <prolib/datastore/character_define.h>

/**
* @class ChrListInfo
* 
* @brief 角色列表信息
**/
class ChrListInfo
{
public:
	ChrListInfo();

public:
	//角色id
	S_INT_32	chrid_;
	//角色昵称
	StaticString<CHR_NICKNAME_LEN> name_;
	//种族
	S_INT_8		race_;
	//职业
	S_INT_8		profession_;
	//性别 0 男的， 1女的
	S_INT_8		sex_;
	S_FLOAT_32	lastposx_;
	S_FLOAT_32	lastposy_;
	S_FLOAT_32	lastposz_;
	S_FLOAT_32	lastfacing_;
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
	//petid
	S_INT_32	petid_;
	//petcode
	S_INT_32	petcode_;
	//petname
	StaticString<CHR_PETNAME_LEN> petname_;
	//角色装备id
	S_INT_16	equiplen_;
	S_INT_32	equips_[CHRITEMS_MAX];
};

#endif	//__CHRLISTINFO__H__
