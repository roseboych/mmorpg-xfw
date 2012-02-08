/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __TELEPORT__H__
#define __TELEPORT__H__

#include "reslib/world/res_struct_def.h"
#include <map>

/**
* @class MapTeleport
* 
* @brief 地图转跳点信息
* 
**/
class Teleport
{
public:
	typedef NS_STL::map< int, teleport_info>	TELEPORTINFO_MAP;

public:
	Teleport();

	/**
	* 根据id获取teleport信息
	* @param id
	* @return teleport_info*
	**/
	teleport_info* get_teleportinfo( int id){
		TELEPORTINFO_MAP::iterator fiter =teleports_.find( id);
		if( fiter == teleports_.end())
			return 0;
		return &(fiter->second);
	}

	void set_teleportinfo( teleport_info& ti){ teleports_[ti.id_] =ti;}

	int get_mapid(){ return mapid_;}
	void set_mapid( int id){ mapid_ =id;}

private:
	//包含的转跳点信息
	TELEPORTINFO_MAP	teleports_;
	int					mapid_;
};

/**
* @class TeleportPair
* 
* @brief 地图转跳点配对
* 
**/
class TeleportPair
{
	typedef NS_STL::map< int, teleport_info*>	TOTELEPORT_MAP;

public:
	TeleportPair():iid_( NO_INITVALUE), from_( 0){}

	int get_iid(){ return iid_;}
	void set_iid( int id){ iid_ =id;}

	teleport_info* get_from(){ return from_;}
	void set_from( teleport_info* t){ this->from_ =t;}

	teleport_info* get_tobyid( int id){
		TOTELEPORT_MAP::iterator fiter =to_teleports_.find( id);
		if( fiter == to_teleports_.end())
			return 0;
		return fiter->second;
	}

	void add_to( int id, teleport_info* t){
		to_teleports_[id] =t;
	}

private:
	//编号
	int				iid_;

	//转跳开始点
	teleport_info*	from_;
	//目标点
	TOTELEPORT_MAP	to_teleports_;
};

#endif	//__TELEPORT__H__
