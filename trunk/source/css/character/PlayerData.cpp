/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PlayerData.h"

#include <ace/OS_NS_string.h>
#include <reslib/items/ItemsConfig.h>

#include "BaseActionState.h"

PlayerData::PlayerData():
cur_state_( 0)
{
}

PlayerData::~PlayerData()
{
	reset();
}

void PlayerData::reset()
{
	cur_state_ =0;

	for( MYPETINFO_VECTOR::iterator iter =mypets_.begin(); iter != mypets_.end(); ++iter)
		delete (*iter);
	mypets_.clear();

	for( MYITEMINFO_MAP::iterator iter =myitems_.begin(); iter != myitems_.end(); ++iter)
		delete iter->second;
	myitems_.clear();

	ACE_OS::memset( (void*)&avatar_items_[0], 0, sizeof( MyItemInfo*)*AVATAR_SLOT_MAX);

	mybuffers_.reset();
}

void PlayerData::init_baseinfo( PRO::Pro_ChrLoad_ack* pack)
{
	ACE_OS::memset( (void*)&avatar_items_[0], 0, sizeof( MyItemInfo*)*AVATAR_SLOT_MAX);

	for( NS_STL::list<PRO::equipment_item>::iterator iter =pack->items_.begin(); iter != pack->items_.end(); ++iter)
	{
		PRO::equipment_item &info =(*iter);

		//获取物品配置
		ItemInfoBase* pitem =ITEMRES_CFG->get_iteminfobyid( info.itemcode_);
		ACE_ASSERT( pitem != 0);
		if( pitem == 0)
			continue;

		MyItemInfo* pinfo =FRAMEWK_NEW MyItemInfo( info, pitem);
		this->myitems_[pinfo->get_itemid()] =pinfo;

		//avatar信息
		if( pinfo->in_avatar())
			avatar_items_[info.inpart_] =pinfo;
	}

	//buffer初始化
	mybuffers_.init_baseinfo( pack);

	//宠物信息
	for( NS_STL::list<PRO::petdata_item>::iterator iter =pack->pets_.begin(); iter != pack->pets_.end(); ++iter)
	{
		PRO::petdata_item& pt =(*iter);

		MyPetInfo* myp =FRAMEWK_NEW MyPetInfo( pt);
		//加载对应的配置
		this->mypets_.push_back( myp);
	}
}

void PlayerData::init_others( PRO::Pro_ChrFin_NTF* pfin)
{
	this->baseinfo_ =pfin->baseinfo_;
}

MyPetInfo* PlayerData::get_petinfo( int petid)
{
	for( size_t ii =0; ii < mypets_.size(); ++ii)
	{
		if( mypets_[ii]->get_petid() == petid)
			return mypets_[ii];
	}

	return 0;
}

MyPetInfo* PlayerData::pet_followme( int petid)
{
	MyPetInfo* ret =get_petinfo( petid);
	if( ret == 0)
		return 0;

	baseinfo_.petid_ =ret->get_petid();

	return ret;
}

MyPetInfo* PlayerData::pet_back()
{
	if( baseinfo_.petid_ == NO_INITVALUE)
		return 0;

	MyPetInfo* ret =get_petinfo( baseinfo_.petid_);
	if( ret == 0)
	{
		baseinfo_.petid_ =NO_INITVALUE;
		return 0;
	}

	baseinfo_.petid_ =NO_INITVALUE;

	return ret;
}

void PlayerData::fill_inview_data( PRO::Pro_AppUnitEnter_ntf::player_info& info)
{
	info.chrid_ =this->chrid_;
	info.nickname_ =baseinfo_.name_;

	info.sex_ =baseinfo_.sex_;
	info.race_ =baseinfo_.race_;
	info.profession_ =baseinfo_.profession_;
	info.petid_ =baseinfo_.petid_;

	MyPetInfo* p =get_petinfo( baseinfo_.petid_);
	if( p == 0)
		info.petid_ =NO_INITVALUE;
	else
	{
		info.petcode_ =p->get_petcode();
		info.petname_ =p->get_petname();
	}
	
	info.locationx_ =baseinfo_.posx_;
	info.locationy_ =baseinfo_.posy_;
	info.locationz_ =baseinfo_.posz_;
	info.facing_ =baseinfo_.facing_;

	info.state_ =cur_state_->get_statetype();
}
