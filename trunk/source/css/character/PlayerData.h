/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYERDATA__H__
#define __PLAYERDATA__H__

#include "../css_pre.h"

#include <prolib/datastore/character_define.h>
#include <prolib/Pro_all.h>
#include <reslib/IPlayerProperty.h>

#include "MyPetInfo.h"
#include "MyItemInfo.h"
#include "playerbuffer/MyBufferContainer.h"
#include "playerskills/MySkillContainer.h"

class BaseActionState;

/**
* @class PlayerData
* 
* @brief 玩家数据
**/
class PlayerData : public IPlayerProperty
{
public:
	typedef NS_STL::vector<MyPetInfo*>	MYPETINFO_VECTOR;
	typedef NS_STL::map< S_INT_32, MyItemInfo*>	MYITEMINFO_MAP;

public:
	PlayerData();
	virtual ~PlayerData();

	/**
	* 重置玩家数据
	**/
	void reset();

	/**
	* 初始化角色信息
	**/
	void init_baseinfo( PRO::Pro_ChrLoad_ack* pack);
	void init_others( PRO::Pro_ChrFin_NTF* pfin);

	/**
	* 获取宠物信息
	* @param petid
	* @return
	**/
	MyPetInfo* get_petinfo( S_INT_32 petid);
	/**
	* 召唤宠物
	* @param petid
	**/
	MyPetInfo* pet_followme( S_INT_32 petid);
	MyPetInfo* pet_back();

	/**
	* 填充进入视野是提供的玩家数据
	* @param info
	**/
	void fill_inview_data( PRO::Pro_AppUnitEnter_ntf::player_info& info);

public:
	//-----------------------------------state machine对外接口封装函数-----------------------------------
	BaseActionState* get_curstate(){ return cur_state_;}
	void set_curstate( BaseActionState* p){ cur_state_ =p;}

public:
	//-----------------------------------buffer对外接口封装函数-----------------------------------

	/**
	* 附加一个新的buffer
	* @param buffcode
	* @return
	**/
	BufferRuntime* attach_newbuffer( S_INT_32 buffcode){ return mybuffers_.attach_newbuffer( buffcode);}

	/**
	* 消除一个buffer
	* @param buffcode
	**/
	void cancel_onebuffer( S_INT_32 buffcode){ return mybuffers_.cancel_onebuffer( buffcode);}

public:
	inline void set_facing( S_FLOAT_32& face);
	inline S_FLOAT_32 get_facing();

public:
	//------------------------------implement IPlayerProperty interface----------------------
	inline virtual S_INT_32 get_hp();
	inline virtual void set_hp( S_INT_32 v);
	inline virtual S_INT_32 inc_hp( S_INT_32 v);
	inline virtual S_INT_32 get_mp();
	inline virtual void set_mp( S_INT_32 v);
	inline virtual S_INT_32 inc_mp( S_INT_32 v);
	inline virtual S_INT_32 get_force();
	inline virtual void set_force( S_INT_32 v);
	inline virtual S_INT_32 inc_force( S_INT_32 v);
	inline virtual S_INT_32 get_agile();
	inline virtual void set_agile( S_INT_32 v);
	inline virtual S_INT_32 inc_agile( S_INT_32 v);
	inline virtual S_INT_32 get_endurance();
	inline virtual void set_endurance( S_INT_32 v);
	inline virtual S_INT_32 inc_endurance( S_INT_32 v);
	inline virtual S_INT_32 get_intellect();
	inline virtual void set_intellect( S_INT_32 v);
	inline virtual S_INT_32 inc_intellect( S_INT_32 v);
	inline virtual S_INT_32 get_spirit();
	inline virtual void set_spirit( S_INT_32 v);
	inline virtual S_INT_32 inc_spirit( S_INT_32 v);
	inline virtual S_INT_32 get_armor();
	inline virtual void set_armor( S_INT_32 v);
	inline virtual S_INT_32 inc_armor( S_INT_32 v);

	inline S_INT_8 get_sex();
	inline S_INT_8 get_race();
	inline S_INT_8 get_profession();
	inline S_INT_32 get_money();
	inline void set_money( S_INT_32 v);
	inline S_INT_32 get_level();
	inline S_INT_8 get_bagslots();
	inline S_INT_8 get_died();
	inline void set_died( S_INT_8 v);

public:
	//角色id
	S_INT_32	chrid_;

	//基础信息
	PRO::character_baseinfo	baseinfo_;
	//属性信息
	//力量force，敏捷agile，耐力endurance，智力intellect，精神spirit，护甲armor
	S_INT_32	prop_force_;
	S_INT_32	prop_agile_;
	S_INT_32	prop_endurance_;
	S_INT_32	prop_intellect_;
	S_INT_32	prop_spirit_;
	S_INT_32	prop_armor_;

	//当前等级最大值(hp, mp, experience)
	S_INT_32	max_hp_;
	S_INT_32	max_mp_;
	S_INT_32	max_exp_;

	//宠物信息
	MYPETINFO_VECTOR	mypets_;
	//背包物品信息
	MYITEMINFO_MAP		myitems_;
	//avatar信息
	MyItemInfo*			avatar_items_[PRO::AVATAR_SLOT_MAX];
	//buffer信息
	MyBufferContainer	mybuffers_;
	//技能信息
	MySkillContainer	myskills_;

	//当前的游戏状态
	BaseActionState*	cur_state_;
};

#include "PlayerData.inl"

#endif	//__PLAYERDATA__H__
