/**
* character module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
* 定义了和角色相关的常量
*/
#ifndef __CHARACTER_DEFINE__H__
#define __CHARACTER_DEFINE__H__

#include <prolib/core_type.h>
#include <string>

//定义了unit对象的类型
enum ACTOR_TYPE_ENUM{
	ACTOR_TYPE_NONE =0,
	//玩家
	ACTOR_TYPE_PLAYER =0x10,
	//NPC
	ACTOR_TYPE_NPC =0x20,
	//怪物
	ACTOR_TYPE_MONSTER =0x30,
};

#pragma pack(push,1)

union entity_uuid
{
	S_INT_64	uuid_;
	S_INT_32	parts_[2];
};

#pragma pack(pop)

//每秒速度	4m
#define SPEED_ONESECOND			400	//400cm

//玩家支持的角色数
#define CHRNUMS_MAX		5

//玩家最大装备数
#define CHRITEMS_MAX	13

//玩家游戏状态定义
enum CHRSTATE_TYPE_ENUM
{
	CHRSTATE_NONE =-1,
	//非战斗
	//坐
	CHRSTATE_NOFIGHT_SITDOWN =0,
	//站立
	CHRSTATE_NOFIGHT_STAND,
	//走动
	CHRSTATE_NOFIGHT_WALK,
	//跑
	CHRSTATE_NOFIGHT_RUN,
	//骑乘
	CHRSTATE_NOFIGHT_RIDE,
	//飞行
	CHRSTATE_NOFIGHT_FLY,
	//战斗
	//站立
	CHRSTATE_FIGHT_STAND,
	//走动
	CHRSTATE_FIGHT_WALK,
	//跑
	CHRSTATE_FIGHT_RUN,
	//骑乘
	CHRSTATE_FIGHT_RIDE,
	//飞行
	CHRSTATE_FIGHT_FLY,
	//导航
	//飞行导航
	CHRSTATE_NAV_FLY,
	//死亡
	//站立
	CHRSTATE_DEATH_STAND,
	//走动
	CHRSTATE_DEATH_WALK,
	//跑
	CHRSTATE_DEATH_RUN,
	//
	CHRSTATE_MAX,
};

extern const NS_STL::string g_chrstate_desc[];

//玩家种族定义
enum CHR_RACE_TYPE_ENUM
{
	//人族
	CHR_RACE_HUMAN =0,
	CHR_RACE_MAX,
};

extern const NS_STL::string g_chr_race_desc[];

//职业定义
enum PROFESSION_HUMAN_TYPE_ENUM
{
	//战士
	PROFESSION_HUMAN_SOLDIER,
	//法师
	PROFESSION_HUMAN_MASTER,
	PROFESSION_HUMAN_MAX,
};

extern const NS_STL::string g_profession_human_desc[];

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

PROTOCOL_NAMESPACE_END

#endif	//__CHARACTER_DEFINE__H__
