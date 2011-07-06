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
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/CTSOption.h>
#include <reslib/deploy/DPXOption.h>

#include <algorithm>
#include <memory>

DPXSvr::DPXSvr(void)
{
}

DPXSvr::~DPXSvr(void)
{
}

void DPXSvr::init_svr()
{
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());

	//css
	csssvr_nums_ =st->get_cssnums();
	csssvr_.reset( FRAMEWK_NEW CSSLink[csssvr_nums_]);

	NS_STL::list< CSSOption*> css;
	st->get_cssoptions( css);
	for( NS_STL::list< CSSOption*>::iterator iter =css.begin(); iter != css.end(); ++iter)
	{
		CSSOption* opt =(*iter);
		CSSLink* l =&csssvr_[opt->server_index_];
		l->set_option( opt);
	}

	//cts
	ctssvr_.set_option( st->get_ctsoption());

	//players
	player_nums_ =st->dpxopt_.maxuser_;
	players_.reset( FRAMEWK_NEW Player[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		players_[ii].reset();
		players_[ii].global_index_ =ii;
	}
}

void DPXSvr::uninit_svr()
{

}

void DPXSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	DPXAPP_SERVICE_ENUM stype =( DPXAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == DPXAPP_SERVICE_DPX)
	{
		DPX_Service* ps =dynamic_cast<DPX_Service*>(s);
		if( ps->type_ == NETSVR_CSS)
		{
			CSSLink* l =get_csslink( ps->server_index_);
			if( l == 0)
				return;

			l->set_linkservice( ps);

			autorelease =false;

			MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] regist to me....!", l->opt_->sn_);
		}
		else
			return;

		//发送回复
		PRO::Pro_SvrRegist_ack* ack =PROTOCOL_NEW PRO::Pro_SvrRegist_ack();
		ack->result_ =0;
		s->append_write_msg( ack);
	}
	else if( stype == DPXAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>( s);

		CTSLink* l =get_ctslink();
		if( l && l->is_waitregistack())
		{
			l->set_status( SVRREGIST_ST_SUCC);
			l->set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to cts success.....");
		}

		autorelease =false;
	}

}

void DPXSvr::service_disconnected( BasicService* s)
{
	DPXAPP_SERVICE_ENUM stype =( DPXAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == DPXAPP_SERVICE_DPX)
	{
		DPX_Service* ps =dynamic_cast<DPX_Service*>(s);

		if( ps->type_ == NETSVR_CSS)
		{
			CSSLink* l =get_csslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] disconnected with me....!", l->opt_->sn_);

				if( l->is_connected())
					svr_cssdisconnect( l);

				l->reset();
			}
		}
	}
	else if( stype == DPXAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>( s);

		CTSLink* l =get_ctslink();

		MODULE_LOG_WARN( MODULE_BOOT, "++++cts disconnected with me....!");

		if( l->is_connected())
			svr_ctsdisconnect( l);

		l->reset();
	}
}

void DPXSvr::svr_cssdisconnect( CSSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to css. now, clear system resource......");
}

void DPXSvr::svr_ctsdisconnect( CTSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to cts. now, clear system resource......");
}

void DPXSvr::close_all_service()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

void DPXSvr::send_to_cts( BasicProtocol* p)
{
	ctssvr_.send_protocol( p);
}

void DPXSvr::send_to_css( BasicProtocol* p, int ind)
{
	CSSLink* cl =this->get_csslink( ind);
	if( cl == 0)
		delete p;
	else
		cl->send_protocol( p);
}
