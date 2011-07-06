/**
* css application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "CSSLinkWrap.h"

#include <prolib/Pro_all.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CSSOption.h>

#include "CSSModule.h"

CSSLinkWrap::CSSLinkWrap():
owner_server_index_( NO_INITVALUE)
{
}

void CSSLinkWrap::init_linkwrap()
{
	SvrTeamOption* st =GLOBALCONFIG_INS->get_svrteamoption( GLOBALCONFIG_INS->get_regionsn());
	CSSOption* css =st->get_cssoption( GLOBALCONFIG_INS->get_appsn());

	this->owner_server_index_ =css->server_index_;
	this->csslocal_.set_option( css);

	css_nums_ =0;
	css2_nums_ =0;

	NS_STL::list<CSSOption*> svr;
	css->svrteamopt_->get_cssoptions( svr);

	for( NS_STL::list<CSSOption*>::iterator iter =svr.begin(); iter != svr.end(); ++iter)
	{
		CSSOption* opt =(*iter);
		if( opt == css)
			continue;

		//sn小的主动连接sn大的
		if( css->sn_ < opt->sn_)
			++css2_nums_;
		else
			++css_nums_;
	}

	if( css_nums_ > 0)
		css_.reset( FRAMEWK_NEW CSSLink[css_nums_]);
	if( css2_nums_ > 0)
		css2_.reset( FRAMEWK_NEW CSS2Link[css2_nums_]);

	int ind =0, ind2 =0;
	for( NS_STL::list<CSSOption*>::iterator iter =svr.begin(); iter != svr.end(); ++iter)
	{
		CSSOption* opt =(*iter);
		if( opt == css)
			continue;

		if( css->sn_ < opt->sn_)
		{
			css2_[ind2].set_option( opt);
			++ind2;
		}
		else
		{
			css_[ind2].set_option( opt);
			++ind;
		}
	}
}

void CSSLinkWrap::css2link_tick( ACE_UINT64 t)
{
	for( int ii =0; ii < css2_nums_; ++ii)
	{
		if( css2_[ii].need_connected())
			css2_[ii].connect_to_server();
	}
}

INetLink* CSSLinkWrap::get_csslinkwrap( int server_index)
{
	if( server_index > owner_server_index_)
		return get_css2link( server_index);
	else if( server_index < owner_server_index_)
		return get_csslink( server_index);
	else
		return &csslocal_;
}

void CSSLinkWrap::fin_regist_service( CSS_Service* s, bool& autorelease)
{
	CSSLink* l =get_csslink( s->server_index_);
	if( l == 0)
		return;

	l->set_linkservice( s);
	autorelease =false;

	//发送回复
	PRO::Pro_SvrRegist_ack* ack =PROTOCOL_NEW PRO::Pro_SvrRegist_ack();
	ack->result_ =0;
	s->append_write_msg( ack);

	MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] regist to me....!", l->opt_->sn_);
}

void CSSLinkWrap::fin_regist_service( CSS2_Service* s, bool& autorelease)
{
	CSS2Link* l =get_css2link( s->server_index_);
	if( l && l->is_waitregistack())
	{
		l->set_status( SVRREGIST_ST_SUCC);
		l->set_linkservice( s);

		autorelease =false;
	}

	MODULE_LOG_WARN( MODULE_BOOT, "++++me connect to css:[%d] success.....", l->opt_->sn_);
}

void CSSLinkWrap::service_disconnected( CSS_Service* s)
{
	CSSLink* l =get_csslink( s->server_index_);
	if( l)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] disconnected with me....!", l->opt_->sn_);

		if( l->is_connected())
			CSSMODULE->svr_cssdisconnect( l);

		l->reset();
	}
}

void CSSLinkWrap::service_disconnected( CSS2_Service* s)
{
	CSS2Link* l =get_css2link( s->server_index_);
	if( l)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "++++css:[%d] disconnected with me....!", l->opt_->sn_);

		if( l->is_connected())
			CSSMODULE->svr_css2disconnect( l);

		l->reset();
	}
}
