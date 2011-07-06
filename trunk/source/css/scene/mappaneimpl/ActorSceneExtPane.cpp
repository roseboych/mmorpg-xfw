/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "ActorSceneExtPane.h"

#include "../../character/UnitBase.h"
#include "MapPane.h"

ActorSceneExtPane::ActorSceneExtPane( UnitBase* pOwner):
inherit( pOwner),
cur_pane_( 0),
oldpanex_( INVALIDATE_PANEINDEX),
oldpaney_( INVALIDATE_PANEINDEX)
{

}

void ActorSceneExtPane::reset()
{
	if( cur_pane_)
		cur_pane_->unregist_unitobj( owner_actor_);
	cur_pane_ =0;

	oldpanex_ =INVALIDATE_PANEINDEX;
	oldpaney_ =INVALIDATE_PANEINDEX;
	clear_viewinout_tmp();
}

bool ActorSceneExtPane::init_unitobj( MapPane* pane)
{
	//注册到对应的pane中
	this->cur_pane_ =pane;

	if( this->cur_pane_ == 0)
		return false;

	cur_pane_->regist_unitobj( owner_actor_);

	return true;
}

void ActorSceneExtPane::change_inpane( MapPane* newpane)
{
	if( cur_pane_ == newpane)
		return;

	if( cur_pane_)
		cur_pane_->unregist_unitobj( owner_actor_);
	cur_pane_ =newpane;

	if( cur_pane_)
		cur_pane_->regist_unitobj( owner_actor_);
}

void ActorSceneExtPane::viewinout_calculate()
{
	this->clear_viewinout_tmp();
}
