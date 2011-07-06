/**
* protocol module
* 
* @category		预定义的结构，在协议中使用
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PRO_STRUCT_DEF__H__
#define __PRO_STRUCT_DEF__H__

#include "prolib/core_type.h"

#include <string>
#include <list>

PROTOCOL_NAMESPACE_BEGIN

//角色的基本信息
struct character_baseinfo
{
	//角色昵称
	NS_STL::string name_;
	//性别 0 男的， 1女的
	S_INT_8		sex_;
	//种族
	S_INT_8		race_;
	//职业
	S_INT_8		profession_;
	//moneys
	S_INT_32	moneys_;
	//角色等级
	S_INT_32	level_;
	//背包空格数
	S_INT_8		bagslots_;
	//技能
	S_INT_32	skill_[2];
	//petid
	S_INT_32	petid_;
	//hp
	S_INT_32	hp_;
	//mp
	S_INT_32	mp_;
	//额外附加的点数
	S_INT_32	power_;
	S_INT_32	agile_;
	S_INT_32	endurance_;
	S_INT_32	intellect_;
	S_INT_32	spirit_;
	S_INT_32	armor_;
	//坐标
	S_FLOAT_32	posx_, posy_, posz_;
	//面向
	S_FLOAT_32	facing_;
	//是否死亡
	S_INT_8		died_;
	//exp
	S_INT_32	exp_;
};

//玩家装备信息项
struct equipment_item
{
	S_INT_32	itemid_;
	//是否装备	0:在背包中 1:普通装 2:属于时装
	S_INT_16	inavatar_;
	//装备位置	inavatar=1,2 有效 0-12
	S_INT_16	inpart_;
	//物品编号
	S_INT_32	itemcode_;
	//有效期计算方式 0:按次计 1:按绝对时间计 2:永久
	S_INT_16	validatetype_;
	//开始时间 validatetype=0 表示剩余次数 =3保留 =1剩余的在线时间 =2开始时间yyyymmddhh
	S_INT_32	starttime_;
	//结束时间 =2时有效，表示结束时间  =0保存堆跌次数
	S_INT_32	endtime_;
};

enum AVATAR_SLOT_ENUM
{
	//头
	AVATAR_SLOT_HEAD =0,
	//手
	AVATAR_SLOT_LEFTHANDE,
	AVATAR_SLOT_RIGHTHANDE,
	//上身
	AVATAR_SLOT_UPPERBODY,
	//下身
	AVATAR_SLOT_LOWERBODY,
	//腰
	AVATAR_SLOT_WAIST,
	//脚
	AVATAR_SLOT_LEFTFOOT,
	AVATAR_SLOT_RIGHTFOOT,
	AVATAR_SLOT_MAX
};

//宠物信息
struct petdata_item
{
	//宠物id
	S_INT_32	petid_;
	//宠物编号 配置ID
	S_INT_32	petcode_;
	//宠物昵称
	NS_STL::string petname_;
};

//buffer信息
struct bufferdata_item
{
	//buffer编号
	S_INT_32	bufferid_;
	//类型 0:系统buffer 1:应用buffer
	S_INT_8		types_;
	S_INT_32	createtime_;
	S_INT_32	scopetime_;
};

//玩家技能信息
struct skilldata_item
{
	//id
	S_INT_32	skillid_;
	//上一次执行时间
	S_INT_32	lastruntime_;
};

PROTOCOL_NAMESPACE_END

#endif	//__PRO_STRUCT_DEF__H__
