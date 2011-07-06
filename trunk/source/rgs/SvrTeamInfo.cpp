/**
* lgs application
* 
* @category		lgs content
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "SvrTeamInfo.h"

#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>

SvrTeamInfo::SvrTeamInfo()
{
}

void SvrTeamInfo::init_svrteaminfo()
{
	NS_STL::list< SvrTeamOption*> svrs;
	GLOBALCONFIG_INS->get_svrteamoptions( svrs);

	team_nums_ =(int)svrs.size();
	teams_.reset( FRAMEWK_NEW teaminfo[team_nums_]);

	for( NS_STL::list< SvrTeamOption*>::iterator iter =svrs.begin(); iter != svrs.end(); ++iter)
	{
		SvrTeamOption* opt =(*iter);

		teams_[opt->server_index_].server_index_ =opt->server_index_;
		teams_[opt->server_index_].is_validate_ =false;
		teams_[opt->server_index_].max_user_ =opt->maxuser_;
		teams_[opt->server_index_].cur_user_ =0;
		teams_[opt->server_index_].teamid_ =opt->sn_;
	}
}

void SvrTeamInfo::update_teaminfo( PRO::Pro_SvrTeamInfo_NTF* info)
{
	PRO::Pro_SvrTeamInfo_NTF::SVRTEAMINF_LIST::iterator iter =info->teams_.begin();

	for( ; iter != info->teams_.end(); ++iter)
	{
		PRO::Pro_SvrTeamInfo_NTF::svrteaminfo& f =(*iter);
		teaminfo* p =get_teaminfo( f.server_index_);
		if( p == 0)
			continue;

		p->cur_user_ =f.curuser_;
		p->is_validate_ =f.is_validate_;
	}
}

PRO::Pro_SvrTeamInfo_NTF* SvrTeamInfo::construct_notify()
{
	PRO::Pro_SvrTeamInfo_NTF* ret =PROTOCOL_NEW PRO::Pro_SvrTeamInfo_NTF();
	
	for( int ii =0; ii < team_nums_; ++ii)
	{
		PRO::Pro_SvrTeamInfo_NTF::svrteaminfo f;
		f.server_index_ =teams_[ii].server_index_;
		f.teamid_ =teams_[ii].teamid_;
		f.is_validate_ =teams_[ii].is_validate_;
		f.maxuser_ =teams_[ii].max_user_;
		f.curuser_ =teams_[ii].cur_user_;

		ret->teams_.push_back( f);
	}

	return ret;
}