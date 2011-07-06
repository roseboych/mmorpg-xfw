/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ACTOR_DEF__H__
#define __ACTOR_DEF__H__

//定义了移动对象的类型
enum ACTOR_TYPE_ENUM{
	ACTOR_TYPE_NONE =0,
	//玩家
	ACTOR_TYPE_PLAYER =0x0010FFFF,
	//组队对象
	ACTOR_TYPE_TEAM =0x0020FFFF,
	//战斗对象
	ACTOR_TYPE_FIREOBJ =0x0030FFFF,
	//npc对象
	ACTOR_TYPE_NPC =0x0040FFFF
};

#endif	//__ACTOR_DEF__H__