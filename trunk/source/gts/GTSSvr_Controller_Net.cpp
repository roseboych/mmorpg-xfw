/**
* gts application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "GTSSvr.h"

#include <prolib/Pro_all.h>

#include <corelib/profiler/ProfilerAnalysis.h>

#include "GTSModule.h"

USE_PROTOCOL_NAMESPACE

void GTSSvr::controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease)
{
	Pro_CtrlProfilerFilterCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerFilterCtrl*>( p);

#ifdef ENABLE_PROFILING

	G_PROFILEANALYSIS->set_activeprofilethreadtype( (EProfileThreadType)ctrl->profiler_thread_);
	G_PROFILEANALYSIS->set_activeprofilescope( (EProfileScope)ctrl->profiler_scope_);
	G_PROFILEANALYSIS->set_maxlevel( ctrl->level_);

#endif
}

void GTSSvr::controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease)
{
	Pro_CtrlProfilerAnalysisCtrl* ctrl =dynamic_cast<Pro_CtrlProfilerAnalysisCtrl*>( p);

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
