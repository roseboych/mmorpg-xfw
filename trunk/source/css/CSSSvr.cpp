/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSSvr.h"

#include <prolib/Pro_all.h>

#include <corelib/log/logmacro.h>

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/deploy/CTSOption.h>
#include <reslib/deploy/DPXOption.h>
#include <reslib/deploy/INSCellOption.h>
#include <reslib/world/WorldInfoConfig.h>

#include "storyserviceimpl/MainStoryServiceImpl.h"
#include "storyserviceimpl/InstanceStoryServiceImpl.h"

#include <algorithm>
#include <memory>

CSSSvr::CSSSvr(void)
{
}

CSSSvr::~CSSSvr(void)
{
}

CSSOption* CSSSvr::get_owneropt()
{
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());
	CSSOption* css =st->get_cssoption( GLOBALCONFIG_INS->get_appsn());

	return css;
}

void CSSSvr::init_svr()
{
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());

	//cts
	ctssvr_.set_option( st->get_ctsoption());

	//dpx
	dpxsvr_.set_option( st->get_dpxoption());

	//gts
	gtssvr_nums_ =st->get_gtsnums();
	gtssvr_.reset( FRAMEWK_NEW GTSLink[gtssvr_nums_]);

	NS_STL::list<GTSOption*> gts;
	st->get_gtsoptions( gts);
	for( NS_STL::list<GTSOption*>::iterator iter =gts.begin(); iter != gts.end(); ++iter)
	{
		GTSOption* opt =(*iter);
		gtssvr_[opt->server_index_].set_option( opt);
	}

	//players
	player_nums_ =st->maxuser_;
	players_.reset( FRAMEWK_NEW AdapterPlayer[player_nums_]);
	for( int ii =0; ii < player_nums_; ++ii)
	{
		players_[ii].reset();
		players_[ii].global_index_ =ii;
	}

	//css之间的相互连接
	csssvr_.init_linkwrap();

	//构造支持的服务
	CSSOption* css =st->get_cssoption( GLOBALCONFIG_INS->get_appsn());
	
	NS_STL::vector<StoryMapOption*> ress;
	if( css->get_mapsize() > 0)
		WORLDINFO->get_mainstorymapres( css->mapids_, ress);

	mainstory_nums_ =(S_INT_32)ress.size();
	mainstorys_.reset( FRAMEWK_NEW MainStoryServiceImpl[mainstory_nums_]);
	for( int ii =0; ii < mainstory_nums_; ++ii)
	{
		if( !mainstorys_[ii].init_service( ress[ii]))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, ".......initialize main story service failed! map:%s ", ress[ii]->get_mapdefpath().c_str());
		}
		else
		{
			dosvrs_.push_back( &mainstorys_[ii]);
		}
	}

	NS_STL::list<INSOption*> insopts;
	css->get_insoptions( insopts);

	inststory_nums_ =css->get_instancesize();
	inststorys_.reset( FRAMEWK_NEW InstanceStoryServiceImpl[inststory_nums_]);

	int insttotle =0;
	for( NS_STL::list<INSOption*>::iterator iter =insopts.begin(); iter != insopts.end(); ++iter)
	{
		INSOption* opt =(*iter);
		for( INSOption::INSCELL_LIST::iterator iter2 =opt->inscells_.begin(); iter2 != opt->inscells_.end(); ++iter2, ++insttotle)
		{
			StoryMapOption* mapopt =WORLDINFO->get_instancemapres( opt->sn_);
			INSCellOption* cellopt =(*iter2);
			if( !inststorys_[insttotle].init_service( mapopt, cellopt))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, ".......initialize instance story service failed! map:%s ", mapopt->get_mapdefpath().c_str());
			}
			else
				dosvrs_.push_back( &inststorys_[insttotle]);
		}
	}

	//启动线程池
	//TODO:单线程调试内容
	dothreads_.set_threadnum( 1);
	if( dothreads_.start_threads())
	{
		MODULE_LOG_WARN( MODULE_BOOT, "story service thread pool start.......");
	}
	else
	{
		MODULE_LOG_ERROR( MODULE_BOOT, ".........story service thread pool start failed.......");
	}
}

void CSSSvr::uninit_svr()
{
	dothreads_.end_threads();
	dosvrs_.clear();

	for( int ii =0; ii < mainstory_nums_; ++ii)
		mainstorys_[ii].uninit_service();
	for( int ii =0; ii < inststory_nums_; ++ii)
		inststorys_[ii].uninit_service();
}

void CSSSvr::fin_regist_service( BasicService* s, bool& autorelease)
{
	CSSAPP_SERVICE_ENUM stype =( CSSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == CSSAPP_SERVICE_DPX)
	{
		DPX_Service* ps =dynamic_cast<DPX_Service*>(s);
		if( dpxsvr_.is_waitregistack())
		{
			dpxsvr_.set_status( SVRREGIST_ST_SUCC);
			dpxsvr_.set_linkservice( ps);

			MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to dpx success.....");
		}

		autorelease =false;
	}
	else if( stype == CSSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>(s);

		CTSLink* l =get_ctslink();
		l->set_status( SVRREGIST_ST_SUCC);
		l->set_linkservice( ps);

		autorelease =false;

		notify_instinitctrl();

		MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to cts success.....");
	}
	else if( stype == CSSAPP_SERVICE_CSS)
	{
		CSS_Service* ps =dynamic_cast<CSS_Service*>(s);

		if( ps->type_ == NETSVR_CSS)
			csssvr_.fin_regist_service( ps, autorelease);
		else if( ps->type_ == NETSVR_GTS)
		{
			GTSLink* l =get_gtslink( ps->server_index_);
			if( l)
			{
				l->set_linkservice( ps);
				autorelease =false;

				MODULE_LOG_WARN( MODULE_BOOT, "++++gts:[%d] regist to me....!", l->opt_->sn_);

			}
			//发送回复
			PRO::Pro_SvrRegist_ack* ack =PROTOCOL_NEW PRO::Pro_SvrRegist_ack();
			if( l)
				ack->result_ =0;
			else
				ack->result_ =1;
			ps->append_write_msg( ack);
		}
	}
	else if( stype == CSSAPP_SERVICE_CSS2)
	{
		CSS2_Service* ps =dynamic_cast<CSS2_Service*>(s);

		csssvr_.fin_regist_service( ps, autorelease);
	}
}

void CSSSvr::service_disconnected( BasicService* s)
{
	CSSAPP_SERVICE_ENUM stype =( CSSAPP_SERVICE_ENUM)s->get_serviceimpl();

	if( stype == CSSAPP_SERVICE_DPX)
	{
		DPX_Service* ps =dynamic_cast<DPX_Service*>(s);
		if( dpxsvr_.is_connected())
			svr_dpxdisconnect();
		dpxsvr_.reset();

		MODULE_LOG_WARN( MODULE_BOOT, "++++dpx disconnected with me....!");
	}
	else if( stype == CSSAPP_SERVICE_CTS)
	{
		CTS_Service* ps =dynamic_cast<CTS_Service*>(s);

		MODULE_LOG_WARN( MODULE_BOOT, "++++cts disconnected with me....!");

		if( ctssvr_.is_connected())
			svr_ctsdisconnect();
		ctssvr_.reset();
	}
	else if( stype == CSSAPP_SERVICE_CSS)
	{
		CSS_Service* ps =dynamic_cast<CSS_Service*>(s);

		if( ps->type_ == NETSVR_CSS)
			csssvr_.service_disconnected( ps);
		else if( ps->type_ == NETSVR_GTS)
		{
			GTSLink* l =get_gtslink( ps->server_index_);
			if( l)
			{
				MODULE_LOG_WARN( MODULE_BOOT, "++++gts:[%d] disconnected with me....!", l->opt_->sn_);

				if( l->is_connected())
					svr_gtsdisconnect( l);

				l->reset();
			}
		}
	}
	else if( stype == CSSAPP_SERVICE_CSS2)
	{
		CSS2_Service* ps =dynamic_cast<CSS2_Service*>(s);
		csssvr_.service_disconnected( ps);
	}
}

void CSSSvr::svr_dpxdisconnect()
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to dpx. now, clear system resource......");
}

void CSSSvr::svr_gtsdisconnect( GTSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to gts. now, clear system resource......");
}

void CSSSvr::svr_ctsdisconnect()
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to cts. now, clear system resource......");
}

void CSSSvr::svr_cssdisconnect( CSSLink* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to css. now, clear system resource......");
}

void CSSSvr::svr_css2disconnect( CSS2Link* l)
{
	//TODO:jeffery
	MODULE_LOG_DEBUG( MODULE_BOOT, "me disconnect to css2. now, clear system resource......");
}

void CSSSvr::close_all_service()
{
	ACE_Guard<ACE_Thread_Mutex> mon( get_mutex());
}

BaseStoryService* CSSSvr::pop_storyservice()
{
	ACE_Guard<ACE_Thread_Mutex> mon( svr_mutex_);

	if( dosvrs_.size() == 0)
		return 0;

	BaseStoryService* ret =dosvrs_.front();
	dosvrs_.pop_front();

	return ret;
}

void CSSSvr::push_storyservice( BaseStoryService* svr)
{
	if( svr == 0)
		return;

	ACE_Guard<ACE_Thread_Mutex> mon( svr_mutex_);

	dosvrs_.push_back( svr);
}

void CSSSvr::send_to_cts( BasicProtocol* p)
{
	ctssvr_.send_protocol( p);
}

void CSSSvr::send_to_dpx( BasicProtocol* p)
{
	dpxsvr_.send_protocol( p);
}

void CSSSvr::broadcast_to_gts( BasicProtocol* p)
{
	if( p == 0)
		return;

	for( int ii =1; ii < gtssvr_nums_; ++ii)
	{
		if( !gtssvr_[ii].is_connected())
			continue;

		gtssvr_[ii].send_protocol( p->clone());
	}

	gtssvr_[0].send_protocol( p);
}

BaseStoryService* CSSSvr::get_storyservicebymap( S_INT_32 mapid)
{
	StoryMapOption* pm =WORLDINFO->get_mainstorymapres( mapid);
	if( pm == 0)
		return 0;

	for( int ii =0; ii < mainstory_nums_; ++ii)
	{
		if( mainstorys_[ii].is_mapsupported( pm))
			return &(mainstorys_[ii]);
	}

	return 0;
}

InstanceStoryServiceImpl* CSSSvr::get_inststorysvrbycellid( S_INT_32 cellid)
{
	for( int ii =0; ii < inststory_nums_; ++ii)
	{
		if( inststorys_[ii].get_cellid() == cellid)
			return &(inststorys_[ii]);
	}

	return 0;
}

void CSSSvr::notify_instinitctrl()
{
	for( S_INT_32 ind =0; ind < inststory_nums_; ++ind)
	{
		SystemCommand<void*>* pcmd =TASKCMD_NEW SystemCommand<void*>( boost::bind( &InstanceStoryServiceImpl::init_instservice, &(inststorys_[ind]), (void*)0));
		inststorys_[ind].regist_netcmd( pcmd);
	}
}
