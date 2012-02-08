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
	typedef NS_STL::map< int, MyItemInfo*>	MYITEMINFO_MAP;

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
	MyPetInfo* get_petinfo( int petid);
	/**
	* 召唤宠物
	* @param petid
	**/
	MyPetInfo* pet_followme( int petid);
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
	BufferRuntime* attach_newbuffer( int buffcode){ return mybuffers_.attach_newbuffer( buffcode);}

	/**
	* 消除一个buffer
	* @param buffcode
	**/
	void cancel_onebuffer( int buffcode){ return mybuffers_.cancel_onebuffer( buffcode);}

public:
	inline void set_facing( float& face);
	inline float get_facing();

public:
	//------------------------------implement IPlayerProperty interface----------------------
	inline virtual int get_hp();
	inline virtual void set_hp( int v);
	inline virtual int inc_hp( int v);
	inline virtual int get_mp();
	inline virtual void set_mp( int v);
	inline virtual int inc_mp( int v);
	inline virtual int get_exp();
	inline virtual void set_exp( int v);
	inline virtual int inc_exp( int v);

	inline virtual int get_force();
	inline virtual void set_force( int v);
	inline virtual int inc_force( int v);
	inline virtual int get_agile();
	inline virtual void set_agile( int v);
	inline virtual int inc_agile( int v);
	inline virtual int get_endurance();
	inline virtual void set_endurance( int v);
	inline virtual int inc_endurance( int v);
	inline virtual int get_intellect();
	inline virtual void set_intellect( int v);
	inline virtual int inc_intellect( int v);
	inline virtual int get_spirit();
	inline virtual void set_spirit( int v);
	inline virtual int inc_spirit( int v);
	inline virtual int get_armor();
	inline virtual void set_armor( int v);
	inline virtual int inc_armor( int v);

	inline char get_sex();
	inline char get_race();
	inline char get_profession();
	inline int get_money();
	inline void set_money( int v);
	inline int get_level();
	inline char get_bagslots();
	inline char get_died();
	inline void set_died( char v);

public:
	//角色id
	int						chrid_;

	//基础信息
	PRO::character_baseinfo	baseinfo_;
	//属性信息
	//力量force，敏捷agile，耐力endurance，智力intellect，精神spirit，护甲armor
	int	prop_force_;
	int	prop_agile_;
	int	prop_endurance_;
	int	prop_intellect_;
	int	prop_spirit_;
	int	prop_armor_;

	//当前等级最大值(hp, mp, experience)
	int	max_hp_;
	int	max_mp_;
	int	max_exp_;

	//宠物信息
	MYPETINFO_VECTOR	mypets_;
	//背包物品信息
	MYITEMINFO_MAP		myitems_;
	//avatar信息
	MyItemInfo*			avatar_items_[AVATAR_SLOT_MAX];
	//buffer信息
	MyBufferContainer	mybuffers_;
	//技能信息
	MySkillContainer	myskills_;

	//当前的游戏状态
	BaseActionState*	cur_state_;
};

#include "PlayerData.inl"

#endif	//__PLAYERDATA__H__
