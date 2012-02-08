/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "SceneManagePaneImpl.h"

#include <reslib/world/WorldInfoConfig.h>
#include "../../BaseStoryService.h"
#include "ActorSceneExtPane.h"

SceneManagePaneImpl::SceneManagePaneImpl( BaseStoryService* pService):
inherit( pService)
{

}

SceneManagePaneImpl::~SceneManagePaneImpl()
{

}

bool SceneManagePaneImpl::init_scenemanage()
{
	inherit::init_scenemanage();

	//初始化格子
	StoryMapOption* minfo =owner_service_->get_storyoption();

	this->map_width_ =minfo->width_;
	this->map_height_ =minfo->height_;
	//计算行/列
	this->pane_column_min_ =::abs(minfo->leftx_)/MAP_PLANE_DIAMETER;
	if( pane_column_min_*MAP_PLANE_DIAMETER != ::abs( minfo->leftx_))
		++pane_column_min_;

	if( minfo->leftx_ < 0)
	{
		pane_column_min_ *= -1;
		pane_column_min_ = pane_column_min_-1;
	}

	this->pane_column_max_ =::abs(minfo->bottomx_)/MAP_PLANE_DIAMETER;
	if( pane_column_max_*MAP_PLANE_DIAMETER != ::abs( minfo->bottomx_))
		++pane_column_max_;

	if( minfo->bottomx_ < 0)
	{
		pane_column_max_ *= -1;
		pane_column_max_ =pane_column_max_ -1;
	}

	this->pane_row_min_ =::abs(minfo->lefty_)/MAP_PLANE_DIAMETER;
	if( pane_row_min_*MAP_PLANE_DIAMETER != ::abs( minfo->lefty_))
		++pane_row_min_;

	if( minfo->lefty_ < 0)
	{
		pane_row_min_ *= -1;
		pane_row_min_ =pane_row_min_ -1;
	}

	this->pane_row_max_ =::abs(minfo->bottomy_)/MAP_PLANE_DIAMETER;
	if( pane_row_max_*MAP_PLANE_DIAMETER != ::abs( minfo->bottomy_))
		++pane_row_max_;

	if( minfo->bottomy_ < 0)
	{
		pane_row_max_ *= -1;
		pane_row_max_ =pane_row_max_ -1;
	}

	this->pane_columns_ =pane_column_max_ - pane_column_min_ + 1;
	this->pane_rows_ =pane_row_max_ - pane_row_min_ + 1;

	panes_.reset( FRAMEWK_NEW MapPane[pane_columns_*pane_rows_]);
	for( int ii =pane_row_min_; ii <= pane_row_max_; ++ii)
	{
		for( int jj =pane_column_min_; jj <= pane_column_max_; ++jj)
		{
			MapPane* p =get_mappane( jj, ii);
			p->pane_x_ =jj;
			p->pane_y_ =ii;
		}
	}

	return true;
}

void SceneManagePaneImpl::uninit_scenemanage()
{
	//释放资源
	for( int ii =0; ii < pane_rows_; ++ii)
	{
		for( int jj =0; jj < pane_columns_; ++jj)
		{
			panes_[ii*pane_columns_ + jj].reset();
		}
	}

	inherit::uninit_scenemanage();
}

void SceneManagePaneImpl::scene_aoi_ticker( const ACE_UINT64& tnow)
{
	//调整活动对象所在的位置
	calcuate_unitobj_inoutview( tnow);

	//计算进入视野的玩家
	calcuate_unit_inview_tmp();
	//计算走出视野的玩家
	calcuate_unit_outview_tmp();
	//发送通知协议
	broadcast_inout_notify();
}

void SceneManagePaneImpl::calcuate_unitobj_inoutview( const ACE_UINT64& tnow)
{
	//计算在线玩家
	for( BaseStoryService::ONLINEPLAYER_MAP::iterator iter =owner_service_->get_onlineplayers().begin(), eiter =owner_service_->get_onlineplayers().end();
		iter != eiter; ++iter)
	{
		Player* user =iter->second;
		if( !user->is_initfinish())
			continue;
		
		user->get_scene_ext<ActorSceneExtPane>()->viewinout_calculate();
	}
}

void SceneManagePaneImpl::broadcast_inout_notify()
{
	USE_PROTOCOL_NAMESPACE;

	//构造进入和退出协议
	for( BaseStoryService::ONLINEPLAYER_MAP::iterator iter =owner_service_->get_onlineplayers().begin(), eiter =owner_service_->get_onlineplayers().end();
		iter != eiter; ++iter)
	{
		Player* user =iter->second;
		if( !user->is_initfinish())
			continue;

		//进入协议
		if( user->get_scene_ext<ActorSceneExtPane>()->in_view_objs_.size() > 0)
		{
			Pro_AppUnitEnter_ntf *ntf =PROTOCOL_NEW Pro_AppUnitEnter_ntf();
			PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);

			for( ActorSceneExtPane::UNITOBJECT_MAP::iterator iter2 =
				user->get_scene_ext<ActorSceneExtPane>()->in_view_objs_.begin(); 
				iter2 != user->get_scene_ext<ActorSceneExtPane>()->in_view_objs_.end(); ++iter2)
			{
				UnitBase* pb =iter2->second;

				ACTOR_TYPE_ENUM t =pb->get_actortype();
				//TODO:in out
				if( t == ACTOR_TYPE_PLAYER)
				{
					Pro_AppUnitEnter_ntf::player_info info;
					((Player*)pb)->fill_inview_data( info);

					//分包发送
					if( !ntf->put_unitinfo( info))
					{
						user->send_to_gts( ntf);

						ntf =PROTOCOL_NEW Pro_AppUnitEnter_ntf();
						PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);
						ntf->put_unitinfo( info);
					}
				}
			}

			//MODULE_LOG_DEBUG(MODULE_TEMP, "broadcast_inout_notify send_to_gts size:%d", ntf->players_.size());
			user->send_to_gts( ntf);
		}

		//退出协议
		if( user->get_scene_ext<ActorSceneExtPane>()->out_view_objs_.size() > 0)
		{
			Pro_AppUnitLeave_ntf* ntf =PROTOCOL_NEW Pro_AppUnitLeave_ntf();
			PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);

			for( ActorSceneExtPane::UNITOBJECT_MAP::iterator iter2 =user->get_scene_ext<ActorSceneExtPane>()->out_view_objs_.begin(); 
				iter2 != user->get_scene_ext<ActorSceneExtPane>()->out_view_objs_.end(); ++iter2)
			{
				UnitBase* pb =iter2->second;
				ACTOR_TYPE_ENUM t =pb->get_actortype();
				//TODO:in out
				if( t == ACTOR_TYPE_PLAYER)
				{
					//分包发送
					if( !ntf->put_player( pb->get_id()))
					{
						user->send_to_gts( ntf);

						ntf =PROTOCOL_NEW Pro_AppUnitLeave_ntf();
						PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);
						ntf->put_player( pb->get_id());
					}
				}
			}

			user->send_to_gts( ntf);
		}
	}
}

void SceneManagePaneImpl::unitobj_entermap( UnitBase* pobj)
{
	ACE_ASSERT( pobj != 0);

	//活动对象加入到地图中
	MapPane* pane =get_mappanebyxy( pobj->get_positionx(), pobj->get_positiony());
	ACE_ASSERT( pane != 0);
	pobj->get_scene_ext<ActorSceneExtPane>()->init_unitobj( pane);
}

void SceneManagePaneImpl::unitobj_leavemap( UnitBase* pobj)
{
	ACE_ASSERT( pobj);
}

void SceneManagePaneImpl::unitobj_moveto( Player* pobj, coordinate_pointer& pos, float& face)
{
	USE_PROTOCOL_NAMESPACE;
	//TODO:移动检测
	//玩家状态检测
	//S_INT_8 ret =pobj->move_avaliabe_check();
	S_INT_8 ret =0;
	//物理系统移动检测
	//map_phys_->move_check( pobj->get_positionx(), pobj->get_positiony(), pobj->get_positionz(), pos.x_, pos.y_, pos.z_);
	/*
	if( map_phys_->test_position_validate( pos.x_, pos.y_, pos.z_))
		ret =0;
	else
		ret =1;
	*/

	if( ret != 0)
	{
		PRO::Pro_PlayerMove_ack *ack =PROTOCOL_NEW PRO::Pro_PlayerMove_ack();
		PRO_UUID_FILL( ack, pobj->global_index_, pobj->uuid_);
		ack->result_ =1;

		pobj->send_to_gts( ack);
		return;
	}

	//移动位置
	pobj->moveto( pos, face);

	MapPane* pane =get_mappanebyxy( pobj->get_positionx(), pobj->get_positiony());
	ACE_ASSERT( pane != 0);
	pobj->get_scene_ext<ActorSceneExtPane>()->change_inpane( pane);

	//转发移动协议
	PRO::Pro_PlayerMove_ntf *ntf =PROTOCOL_NEW PRO::Pro_PlayerMove_ntf();
	ntf->chrid_ =pobj->chrid_;
	ntf->locationx_ =pobj->get_positionx();
	ntf->locationy_ =pobj->get_positiony();
	ntf->locationz_ =pobj->get_positionz();
	ntf->facing_ =pobj->get_facing();

	this->broadcast_inview( pobj, ntf, false);
}

void SceneManagePaneImpl::calcuate_unit_inview_tmp()
{
	//计算在线玩家
	for( BaseStoryService::ONLINEPLAYER_MAP::iterator iter =owner_service_->get_onlineplayers().begin(), 
		eiter =owner_service_->get_onlineplayers().end(); iter != eiter; ++iter)
	{
		Player* user =iter->second;

		if( !user->is_initfinish())
			continue;

		//获取当前所在的pane(x,y)
		int panex =POSX2PANEX( user->get_positionx());
		int paney =POSY2PANEY( user->get_positiony());

		//没有变化
		ActorSceneExtPane *ext =user->get_scene_ext<ActorSceneExtPane>();
		if( panex == ext->oldpanex_ && paney == ext->oldpaney_)
			continue;

		if( ext->oldpanex_ == INVALIDATE_PANEINDEX || ext->oldpaney_ == INVALIDATE_PANEINDEX)
		{
			//玩家初始化,刚进入地图
			NS_STL::vector<MapPane*> pss;
			get_playerviewpanes( panex, paney, pss);
			for( size_t ii =0; ii < pss.size(); ++ii)
				pss[ii]->unit_inout_fill( user, true);
		}
		else if( ::abs( panex - ext->oldpanex_) >= VIEW_PLANE_NUM || ::abs( paney - ext->oldpaney_) >= VIEW_PLANE_NUM)
		{
			//跳跃,不需要考虑重叠情况
			NS_STL::vector<MapPane*> pss;
			get_playerviewpanes( panex, paney, pss);
			for( size_t ii =0; ii < pss.size(); ++ii)
				pss[ii]->unit_inout_fill( user, true);
		}
		else
		{
			//获取重叠区域
			pane_region	r_enter[2];
			if( ext->get_moveinregion( &r_enter[0]))
			{
				for( int ii =0; ii < 2; ++ii)
				{
					if( r_enter[ii].is_empty())
						continue;

					NS_STL::vector<MapPane*> pss;
					get_playerviewpanes( r_enter[ii].x1, r_enter[ii].y1, r_enter[ii].x2, r_enter[ii].y2, pss);
					for( size_t kk =0; kk < pss.size(); ++kk)
						pss[kk]->unit_inout_fill( user, true);
				}
			}
		}
	}
}

void SceneManagePaneImpl::calcuate_unit_outview_tmp()
{
	for( BaseStoryService::ONLINEPLAYER_MAP::iterator iter =owner_service_->get_onlineplayers().begin(), 
		eiter =owner_service_->get_onlineplayers().end(); iter != eiter; ++iter)
	{
		Player* user =iter->second;
		if( !user->is_initfinish())
			continue;

		//获取当前所在的pane(x,y)
		int panex =POSX2PANEX( user->get_positionx());
		int paney =POSY2PANEY( user->get_positiony());

		ActorSceneExtPane* ext =user->get_scene_ext<ActorSceneExtPane>();
		//没有变化
		if( panex == ext->oldpanex_ && paney == ext->oldpaney_)
			continue;

		if( ext->oldpanex_ == INVALIDATE_PANEINDEX || ext->oldpaney_ == INVALIDATE_PANEINDEX)
		{
			//玩家初始化,刚进入地图
		}
		else if( ::abs( panex - ext->oldpanex_) >= VIEW_PLANE_NUM || ::abs( paney - ext->oldpaney_) >= VIEW_PLANE_NUM)
		{
			//跳跃,不需要考虑重叠情况
			NS_STL::vector<MapPane*> pss;
			get_playerviewpanes( ext->oldpanex_, ext->oldpaney_, pss);
			for( size_t ii =0; ii < pss.size(); ++ii)
				pss[ii]->unit_inout_fill( user, false);
		}
		else
		{
			//获取重叠区域
			pane_region	r_exit[2];
			if( ext->get_moveoutregion( &r_exit[0]))
			{
				for( int ii =0; ii < 2; ++ii)
				{
					if( r_exit[ii].is_empty())
						continue;

					NS_STL::vector<MapPane*> pss;
					get_playerviewpanes( r_exit[ii].x1, r_exit[ii].y1, r_exit[ii].x2, r_exit[ii].y2, pss);
					for( size_t kk =0; kk < pss.size(); ++kk)
						pss[kk]->unit_inout_fill( user, false);
				}
			}
		}

		//修改oldpane
		ext->change_oldpane();
	}
}

void SceneManagePaneImpl::broadcast_inview( UnitBase* sender, BasicProtocol* p, bool includesender)
{
	ActorSceneExtPane* ext =sender->get_scene_ext<ActorSceneExtPane>();

	//必须初始化完成，并且进入到pane中
	if( !sender->is_initfinish() || ext->oldpanex_ == INVALIDATE_PANEINDEX)
	{
		delete p;
		return;
	}

	NS_STL::vector<MapPane*> pss;
	get_playerviewpanes( ext->oldpanex_, ext->oldpaney_, pss);
	if( pss.size() == 0)
		delete p;
	else
	{
		for( size_t ii =1; ii < pss.size(); ++ii)
		{
			pss[ii]->broadcast( sender, p, includesender, false);
		}

		pss[0]->broadcast( sender, p, includesender, true);
	}
}

void SceneManagePaneImpl::notify_allobjs_inview( Player* pl)
{
	int panex =POSX2PANEX( pl->get_positionx());
	int paney =POSY2PANEY( pl->get_positiony());

	NS_STL::vector<MapPane*> pss;
	get_playerviewpanes( panex, paney, pss);

	PRO::Pro_AppUnitEnter_ntf *ntf =PROTOCOL_NEW PRO::Pro_AppUnitEnter_ntf();
	PRO_UUID_FILL( ntf, pl->global_index_, pl->uuid_);

	for( size_t kk =0; kk < pss.size(); ++kk)
	{
		MapPane* mp =pss[kk];

		//构造进入和退出协议
		for( MapPane::UNITOBJ_MAP::iterator iter =mp->players_.begin(); iter != mp->players_.end(); ++iter)
		{
			UnitBase* pb =iter->second;
			if( !pb->is_initfinish() || pb == pl)
				continue;

			ACTOR_TYPE_ENUM t =pb->get_actortype();
			//TODO:可以增加其他类型
			if( t == ACTOR_TYPE_PLAYER)
			{
				PRO::Pro_AppUnitEnter_ntf::player_info info;
				((Player*)pb)->fill_inview_data( info);

				//分包发送
				if( !ntf->put_unitinfo( info))
				{
					pl->send_to_gts( ntf);

					ntf =PROTOCOL_NEW PRO::Pro_AppUnitEnter_ntf();
					PRO_UUID_FILL( ntf, pl->global_index_, pl->uuid_);
					ntf->put_unitinfo( info);
				}
			}
		}
	}

	//发送最后一个包
	pl->send_to_gts( ntf);
}
