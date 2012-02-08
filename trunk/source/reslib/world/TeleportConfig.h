/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __TELEPORTCONFIG__H__
#define __TELEPORTCONFIG__H__

#include <corelib/corelibdef.h>

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <map>

#include "reslib/world/res_struct_def.h"
#include "reslib/world/Teleport.h"

class TiXmlElement;

/**
* @class TeleportConfig
* 
* @brief 地图转跳点管理
* 
**/
class TeleportConfig
{
	typedef NS_STL::map< int, Teleport>		ALLTELEPORT_MAP;
	typedef NS_STL::map< int, TeleportPair>	TELEPORTPAIR_MAP;
public:
	TeleportConfig();

	/**
	* 初始化转跳点信息
	* @return
	**/
	bool init_teleports( TiXmlElement* root);

	TeleportPair* get_teleportpair( int iid);

protected:
	/**
	* 根据mapid获取转跳点配置
	* @param mapid
	* @return 
	**/
	Teleport* get_mapteleport( int mapid);

private:
	//所有的转跳点
	ALLTELEPORT_MAP		teleports_;
	TELEPORTPAIR_MAP	teleportpairs_;
};

#endif	//__TELEPORTCONFIG__H__
