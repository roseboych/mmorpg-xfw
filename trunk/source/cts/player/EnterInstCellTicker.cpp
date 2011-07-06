/**
* cts application
*
* @category		玩家进入副本的状态节点
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "EnterInstCellTicker.h"

#include <ace/OS_NS_sys_time.h>
#include "Player.h"

EnterInstCellTicker::EnterInstCellTicker():
owner_( 0)
{
	reset();
}

void EnterInstCellTicker::set_owner( Player* p)
{
	owner_ =p;	
}

void EnterInstCellTicker::reset()
{
	tick_state_ =EINSTCELL_ST_NONE;
}

void EnterInstCellTicker::tick( const ACE_UINT64& t)
{
	if( tick_state_ == EINSTCELL_ST_NONE)
		return;

	if( tick_state_ == EINSTCELL_ST_WAITCELLPROXY)
	{
		//在1秒内返回
		if( tick_time_ + 1000 < t)
			owner_->instcell_proxy_overtime();
	}
	else if( tick_state_ == EINSTCELL_ST_WAITCELLCONFIRM)
	{
		if( tick_time_ + 1500 < t)
			owner_->instcell_confirm_overtime();
	}
}

void EnterInstCellTicker::wait_cellproxy( PRO::Pro_AppEnterIns_req* req, CSSLink* svr)
{
	if( tick_state_ != EINSTCELL_ST_NONE)
		return;

	tick_state_ =EINSTCELL_ST_WAITCELLPROXY;
	tick_time_ =ACE_OS::gettimeofday().get_msec();

	posx_ =req->posx_;
	posy_ =req->posy_;
	posz_ =req->posz_;
	css_svr_ =svr;
}

void EnterInstCellTicker::wait_cellconfirm()
{
	if( tick_state_ != EINSTCELL_ST_WAITCELLPROXY)
		return;

	tick_state_ =EINSTCELL_ST_WAITCELLCONFIRM;
	tick_time_ =ACE_OS::gettimeofday().get_msec();
}
