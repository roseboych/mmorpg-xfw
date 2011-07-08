/**
* dpx application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DPXSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/db/MustReturnContext.h>
#include <corelib/db/NotReturnContext.h>
#include <corelib/log/logmacro.h>

#include "dbaction/ChrGetListAction.h"
#include "dbaction/ChrCreateAction.h"
#include "dbaction/ChrInfoAction.h"
#include "dbaction/ChrDelAction.h"
#include "dbaction/ItemAddAction.h"
#include "dbaction/ItemDelAction.h"
#include "dbaction/ItemOperAction.h"
#include "dbaction/ItemUpdateAction.h"

#include "DPXModule.h"

USE_PROTOCOL_NAMESPACE;

void DPXSvr::cts_userlost_ntf( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	MODULE_LOG_DEBUG( MODULE_TEMP, "userlost from cts. user:[%d]", p->uuid_.userid_);

	//清空缓冲数据
	user->reset();
}

void DPXSvr::cts_userregist_ntf( BasicProtocol* p, bool& autorelease)
{
	Player* user =get_player( p->get_uuidglobalindex());
	if( user == 0)
		return;

	uuid_session id;
	id.set_uuid( p->get_uuiduserid(), p->get_uuidinitstmp());

	user->regist( id);
}

void DPXSvr::cts_chrlist_req( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrList_req* req =dynamic_cast<Pro_ChrList_req*>(p);

	if( user->chrs_init_)
	{
		this->send_to_cts( user->construct_chrlistpro());
	}
	else
	{
		ChrGetListAction* la =DATABASE_NEW ChrGetListAction( user->uuid_.init_timestamp_);
		la->player_ =user;
		la->uuid_ =user->uuid_;

		MustReturnContext<ChrGetListAction>* rc =TASKCMD_NEW MustReturnContext<ChrGetListAction>( la);
		DPXMODULE->append_returntask_sort( user->uuid_.userid_, rc);
	}
}

void DPXSvr::cts_chradd_req( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	if( !user->chrs_init_)
	{
		Pro_ChrCreate_ack* ack =PROTOCOL_NEW Pro_ChrCreate_ack();
		PRO_UUID_FILL( ack, user->global_index_, user->uuid_);
		ack->result_ =1;

		this->send_to_cts( ack);
		return;
	}

	ChrCreateAction* la =DATABASE_NEW ChrCreateAction( user->uuid_.init_timestamp_);
	la->player_ =user;
	la->uuid_ =user->uuid_;
	la->param_ =dynamic_cast<Pro_ChrCreate_req*>(p);
	autorelease =false;

	MustReturnContext<ChrCreateAction>* rc =TASKCMD_NEW MustReturnContext<ChrCreateAction>( la);
	DPXMODULE->append_returntask_sort( user->uuid_.userid_, rc);
}

void DPXSvr::cts_chrsel_req( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrSel_req* req =dynamic_cast<Pro_ChrSel_req*>(p);

	Pro_ChrSel_ack* ack =PROTOCOL_NEW Pro_ChrSel_ack();
	PRO_UUID_FILL( ack, user->global_index_, user->uuid_);

	ack->chrid_ =req->chrid_;

	if( !user->chrs_init_)
		ack->result_ =1;
	else
	{
		ChrListInfo* cl =user->is_chrinlist( req->chrid_);
		if( cl == 0)
			ack->result_ =2;
		else
		{
			ack->result_ =0;
			ack->lastposx_ =cl->lastposx_;
			ack->lastposy_ =cl->lastposy_;
			ack->lastposz_ =cl->lastposz_;
			ack->lastfacing_ =cl->lastfacing_;
			ack->race_ =cl->race_;
			ack->profession_ =cl->profession_;

			user->set_curchrid( req->chrid_);
		}
	}

	this->send_to_cts( ack);
}

void DPXSvr::cts_chrdel_req( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrDel_req* req =dynamic_cast<Pro_ChrDel_req*>(p);

	if( !user->chrs_init_ )
	{
		Pro_ChrDel_ack* ack =PROTOCOL_NEW Pro_ChrDel_ack();
		PRO_UUID_FILL( ack, user->global_index_, user->uuid_);
		ack->result_ =1;
		ack->chrid_ =req->chrid_;

		this->send_to_cts( ack);
		return;
	}
	else if( !user->is_chrinlist( req->chrid_))
	{
		Pro_ChrDel_ack* ack =PROTOCOL_NEW Pro_ChrDel_ack();
		PRO_UUID_FILL( ack, user->global_index_, user->uuid_);
		ack->result_ =2;
		ack->chrid_ =req->chrid_;

		this->send_to_cts( ack);
		return;
	}

	ChrDelAction* la =PROTOCOL_NEW ChrDelAction( user->uuid_.init_timestamp_);
	la->player_ =user;
	la->uuid_ =user->uuid_;
	la->chrid_ =req->chrid_;

	MustReturnContext<ChrDelAction>* rc =TASKCMD_NEW MustReturnContext<ChrDelAction>( la);
	DPXMODULE->append_returntask_sort( user->uuid_.userid_, rc);
}

void DPXSvr::css_chrload_req( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_ChrLoad_req* req =dynamic_cast<Pro_ChrLoad_req*>(p);

	//保存和css的关联
	user->css_svr_ =this->get_csslink( req->cssindex_);

	MODULE_LOG_DEBUG( MODULE_TEMP, "DPXSvr::cts_chrload_req user:%d recv load request", user->uuid_.userid_);

	if( user->is_chrload_)
	{
		//发送通知协议
		Pro_ChrLoad_ack* ack =PROTOCOL_NEW Pro_ChrLoad_ack();
		PRO_UUID_FILL( ack, user->global_index_, user->uuid_);

		for( Player::EQUIPITEMS_MAP::iterator iter =user->items_.begin(); iter != user->items_.end(); ++iter)
		{
			PRO::equipment_item* pitem =iter->second;
			ack->items_.push_back( *pitem);
		}
		for( Player::PETDATA_MAP::iterator iter =user->pets_.begin(); iter != user->pets_.end(); ++iter)
		{
			PRO::petdata_item* ppet =iter->second;
			ack->pets_.push_back( *ppet);
		}

		user->send_to_css( ack);

		Pro_ChrFin_NTF* ntf =PROTOCOL_NEW Pro_ChrFin_NTF();
		PRO_UUID_FILL( ntf, user->global_index_, user->uuid_);
		ntf->chrid_ =user->chrid_;
		ntf->baseinfo_ =user->baseinfo_;

		user->send_to_css( ntf);
	}
	else
	{
		ChrInfoAction* la =DATABASE_NEW ChrInfoAction( user->uuid_.init_timestamp_);
		la->player_ =user;
		la->uuid_ =user->uuid_;
		la->chrid_ =req->chrid_;

		MustReturnContext<ChrInfoAction>* rc =TASKCMD_NEW MustReturnContext<ChrInfoAction>( la);
		DPXMODULE->append_returntask_sort( user->uuid_.userid_, rc);
	}
}
