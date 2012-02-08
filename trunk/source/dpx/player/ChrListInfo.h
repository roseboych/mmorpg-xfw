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
	int			chrid_;
	//角色昵称
	StaticString<CHR_NICKNAME_LEN> name_;
	//种族
	char		race_;
	//职业
	char		profession_;
	//性别 0 男的， 1女的
	char		sex_;
	float		lastposx_;
	float		lastposy_;
	float		lastposz_;
	float		lastfacing_;
	//hp
	int			hp_;
	//mp
	int			mp_;
	//exp
	int			exp_;
	//角色等级
	int			level_;
	//moneys
	int			moneys_;
	//petid
	int			petid_;
	//petcode
	int			petcode_;
	//petname
	StaticString<CHR_PETNAME_LEN> petname_;
	//角色装备id
	short		equiplen_;
	int			equips_[CHRITEMS_MAX];
};

#endif	//__CHRLISTINFO__H__
