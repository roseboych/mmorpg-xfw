/**
* css application
*
* @category		cmsvr module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "MapPane.h"

#include "SceneManagePaneImpl.h"
#include "ActorSceneExtPane.h"

MapPane::MapPane()
{
}

void MapPane::reset()
{
	players_.clear();
}

void MapPane::broadcast( UnitBase* sender, BasicProtocol* p, bool includeme, bool freepro)
{
	//广播原则
	//尽可能的减少p->clone()的调用，优化算法
	size_t totles =players_.size();

	//没有可以转播的对象
	if( totles == 0)
	{
		if( freepro)
			delete p;
		return;
	}

	//处理玩家
	size_t cnts =players_.size();
	totles -= cnts;

	if( cnts > 0)
	{
		if( cnts > 1)
		{
			UNITOBJ_MAP::iterator iter =players_.begin();
			++iter;
			for( ; iter != players_.end(); ++iter)
			{
				--cnts;
				Player* pu =(Player*)iter->second;
				if( pu == sender && !includeme)
					continue;

				PRO_UUID_FILL( p, pu->global_index_, pu->uuid_);
				pu->send_to_gts( p->clone());
			}
		}

		//处理单个
		if( cnts > 0)
		{
			Player* pu =(Player*)players_.begin()->second;
			if( pu != sender || includeme)
			{
				PRO_UUID_FILL( p, pu->global_index_, pu->uuid_);
				if( totles <= 0 && freepro)
				{
					pu->send_to_gts( p);
					return;
				}
				else
					pu->send_to_gts( p->clone());
			}
			else if( totles <= 0 && freepro)
			{
				delete p;
				return;
			}
		}
	}
}

void MapPane::regist_unitobj( UnitBase* pobj)
{
	ACE_ASSERT( pobj != 0);

	ACTOR_TYPE_ENUM tp =pobj->get_actortype();
	if( tp == ACTOR_TYPE_PLAYER)
	{
		players_[pobj->get_id()] =pobj;
	}
}

void MapPane::unregist_unitobj( UnitBase* pobj)
{
	ACE_ASSERT( pobj != 0);

	S_INT_32 uid =pobj->get_id();
	ACTOR_TYPE_ENUM tp =pobj->get_actortype();
	if( tp == ACTOR_TYPE_PLAYER)
	{
		players_.erase( uid);
	}
}

void MapPane::unit_inout_fill( UnitBase* punit, bool bin)
{
	//玩家填充
	for( UNITOBJ_MAP::iterator iter =players_.begin(); iter != players_.end(); ++iter)
	{
		UnitBase* pobj =iter->second;
		//必须加载数据完成
		if( pobj == punit || !pobj->is_initfinish())
			continue;

		ActorSceneExtPane* ext1 =punit->get_scene_ext<ActorSceneExtPane>();
		ActorSceneExtPane* ext2 =pobj->get_scene_ext<ActorSceneExtPane>();

		if( bin)
		{
			ext1->join_inview( pobj);
			ext2->join_inview( punit);
		}
		else
		{
			ext1->join_outview( pobj);
			ext2->join_outview( punit);
		}
	}
}
