/**
* rgs application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "../RGSSvr.h"

#include <prolib/Pro_all.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/RGSOption.h>
#include <reslib/deploy/LGSOption.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/CTSOption.h>

#include <corelib/profiler/ProfilerAnalysis.h>

#include "../RGSModule.h"
#include "../LGSLink.h"
#include "../CTSLink.h"

USE_PROTOCOL_NAMESPACE

void RGSSvr::controller_login_req( BasicProtocol* p, bool& autorelease)
{
	Pro_CtrlLogin_req* req =dynamic_cast<Pro_CtrlLogin_req*>( p);

	RGSOption* opt =GLOBALCONFIG_INS->get_rgsoption();
	S_INT_8 ret =opt->has_privilege( req->name_, req->pwd_)?0:1;

	if( !controller_svr_.finish_login( ret == 0))
		ret =1;

	Pro_CtrlLogin_ack* ack =PROTOCOL_NEW Pro_CtrlLogin_ack();
	ack->result_ =ret;

	controller_svr_.send_protocol( ack);
}

void RGSSvr::controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease)
{
	if( !controller_svr_.is_login())
		return;

	Pro_CtrlProfilerFilterCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerFilterCtrl*>( p);

	if( ctrl->machine_type_ == 1)
	{
		//rgs
#ifdef ENABLE_PROFILING
		G_PROFILEANALYSIS->set_activeprofilethreadtype( (EProfileThreadType)ctrl->profiler_thread_);
		G_PROFILEANALYSIS->set_activeprofilescope( (EProfileScope)ctrl->profiler_scope_);
		G_PROFILEANALYSIS->set_maxlevel( ctrl->level_);
#endif

	}
	else if( ctrl->machine_type_ == 2)
	{
		//lgs
		LGSOption* lgs =GLOBALCONFIG_INS->get_lgsoption( ctrl->machine_id1_);
		if( lgs)
		{
			LGSLink* plink =this->get_lgslink( lgs->server_index_);
			
			plink->send_protocol( p);
			autorelease =false;
		}
	}
	else
	{
		//cts,dpx,css,gts
		SvrTeamOption* psvr =GLOBALCONFIG_INS->get_svrteamoption( ctrl->machine_id1_);
		if( psvr)
		{
			CTSLink* plink =this->get_ctslink( psvr->server_index_);
			if( plink)
			{
				plink->send_protocol( p);
				autorelease =false;
			}
		}
	}
}

void RGSSvr::controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease)
{
	if( !controller_svr_.is_login())
		return;

	Pro_CtrlProfilerAnalysisCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerAnalysisCtrl*>( p);

	if( ctrl->machine_type_ == 1)
	{
		//rgs
#ifdef ENABLE_PROFILING
		if( ctrl->ctrl_ == 1)
		{
			G_PROFILEANALYSIS->start_analysis( 
				(ctrl->transfer_ & 1) > 0, (ctrl->transfer_ & 2) > 0, 
				ctrl->transfer_net_ip_, ctrl->transfer_net_port_);
		}
		else
			G_PROFILEANALYSIS->stop_analysis();
#endif

	}
	else if( ctrl->machine_type_ == 2)
	{
		//lgs
		LGSOption* lgs =GLOBALCONFIG_INS->get_lgsoption( ctrl->machine_id1_);
		if( lgs)
		{
			LGSLink* plink =this->get_lgslink( lgs->server_index_);

			plink->send_protocol( p);
			autorelease =false;
		}
	}
	else
	{
		//cts,dpx,css,gts
		SvrTeamOption* psvr =GLOBALCONFIG_INS->get_svrteamoption( ctrl->machine_id1_);
		if( psvr)
		{
			CTSLink* plink =this->get_ctslink( psvr->server_index_);
			if( plink)
			{
				plink->send_protocol( p);
				autorelease =false;
			}
		}
	}
}
