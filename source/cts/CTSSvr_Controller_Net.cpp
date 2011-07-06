/**
* cts application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "CTSSvr.h"

#include <prolib/Pro_all.h>

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/GTSOption.h>
#include <reslib/deploy/CSSOption.h>
#include <corelib/log/logmacro.h>

#include <corelib/profiler/ProfilerAnalysis.h>

USE_PROTOCOL_NAMESPACE

void CTSSvr::controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease)
{
	Pro_CtrlProfilerFilterCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerFilterCtrl*>( p);

	if( ctrl->machine_type_ == 3)
	{
		//cts
#ifdef ENABLE_PROFILING
		G_PROFILEANALYSIS->set_activeprofilethreadtype( (EProfileThreadType)ctrl->profiler_thread_);
		G_PROFILEANALYSIS->set_activeprofilescope( (EProfileScope)ctrl->profiler_scope_);
		G_PROFILEANALYSIS->set_maxlevel( ctrl->level_);
#endif
	}
	else if( ctrl->machine_type_ == 4)
	{
		//dpx
		send_to_dpx( p);
		autorelease =false;
	}
	else if( ctrl->machine_type_ == 5)
	{
		//css
		CSSOption* pcss =owner_svrteam_->get_cssoption( ctrl->machine_id2_);
		if( pcss)
		{
			CSSLink* plink =this->get_csslink( pcss->server_index_);
			plink->send_protocol( p);
			autorelease =false;
		}
	}
	else if( ctrl->machine_type_ == 6)
	{
		//gts
		GTSOption* pgts =owner_svrteam_->get_gtsoption( ctrl->machine_id2_);
		if( pgts)
		{
			GTSLink* plink =this->get_gtslink( pgts->server_index_);
			plink->send_protocol( p);
			autorelease =false;
		}
	}
}

void CTSSvr::controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease)
{
	Pro_CtrlProfilerAnalysisCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerAnalysisCtrl*>( p);

	if( ctrl->machine_type_ == 3)
	{
		//cts
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
	else if( ctrl->machine_type_ == 4)
	{
		//dpx
		send_to_dpx( p);
		autorelease =false;
	}
	else if( ctrl->machine_type_ == 5)
	{
		//css
		CSSOption* pcss =owner_svrteam_->get_cssoption( ctrl->machine_id2_);
		if( pcss)
		{
			CSSLink* plink =this->get_csslink( pcss->server_index_);
			plink->send_protocol( p);
			autorelease =false;
		}
	}
	else if( ctrl->machine_type_ == 6)
	{
		//gts
		GTSOption* pgts =owner_svrteam_->get_gtsoption( ctrl->machine_id2_);
		if( pgts)
		{
			GTSLink* plink =this->get_gtslink( pgts->server_index_);
			plink->send_protocol( p);
			autorelease =false;
		}
	}
}
