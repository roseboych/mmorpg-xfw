/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/SvrTeamOption.h"

#include <corelib/corelibdef.h>

SvrTeamOption::SvrTeamOption():server_index_(0)
{
}

SvrTeamOption::~SvrTeamOption()
{
	release();
}

void SvrTeamOption::release()
{
	for( GTSOPTION_MAP::iterator iter =gtss_.begin(); iter != gtss_.end(); ++iter)
		delete iter->second;
	gtss_.clear();

	for( CSSOPTION_MAP::iterator iter =csss_.begin(); iter != csss_.end(); ++iter)
		delete iter->second;
	csss_.clear();

	for( INSCELLOPTION_MAP::iterator iter =inscells_.begin(); iter != inscells_.end(); ++iter)
		delete iter->second;
	inscells_.clear();
}

CTSOption* SvrTeamOption::get_ctsoption()
{
	return &ctsopt_;
}

DPXOption* SvrTeamOption::get_dpxoption()
{
	return &dpxopt_;
}

GTSOption* SvrTeamOption::get_gtsoption( int sn)
{
	GTSOPTION_MAP::iterator fiter =gtss_.find( sn);
	if( fiter == gtss_.end())
		return 0;

	return fiter->second;
}

void SvrTeamOption::get_gtsoptions( NS_STL::list<GTSOption*>& ret)
{
	GTSOPTION_MAP::iterator iter, eiter =gtss_.end();
	for( iter =gtss_.begin(); iter != eiter; ++iter)
		ret.push_back( iter->second);
}

int SvrTeamOption::get_gtsnums()
{
	return (int)gtss_.size();
}

CSSOption* SvrTeamOption::get_cssoption( int sn)
{
	CSSOPTION_MAP::iterator fiter =csss_.find( sn);
	if( fiter == csss_.end())
		return 0;

	return fiter->second;
}

void SvrTeamOption::get_cssoptions( NS_STL::list<CSSOption*>& ret)
{
	CSSOPTION_MAP::iterator iter, eiter =csss_.end();
	for( iter =csss_.begin(); iter != eiter; ++iter)
		ret.push_back( iter->second);
}

int SvrTeamOption::get_cssnums()
{
	return (int)csss_.size();
}

CSSOption* SvrTeamOption::get_cssoptbymapid( int mid)
{
	CSSOPTION_MAP::iterator iter, eiter =csss_.end();
	for( iter =csss_.begin(); iter != eiter; ++iter)
	{
		CSSOption* opt =iter->second;
		if( opt->test_mapincss( mid))
			return opt;
	}

	return 0;
}

int SvrTeamOption::get_inscellnums()
{
	return (int)inscells_.size();
}

INSCellOption* SvrTeamOption::get_inscellopt( int cellid)
{
	INSCELLOPTION_MAP::iterator fiter =inscells_.find( cellid);
	if( fiter == inscells_.end())
		return 0;
	return fiter->second;
}

bool SvrTeamOption::check_validate()
{
	//设置最大玩家数
	ctsopt_.totleusers_ =this->maxuser_;

	if( !check_gtsoption())
		return false;

	if( !check_svrteamoption())
		return false;

	return true;
}

bool SvrTeamOption::check_gtsoption()
{
	int tuser =0;
	for( GTSOPTION_MAP::iterator iter =gtss_.begin(); iter != gtss_.end(); ++iter)
	{
		GTSOption* opt =iter->second;
		tuser += opt->usernum_;
	}

	//gts支持的最大玩家数不能超过服务器组的最大支持人数
	if( tuser > this->maxuser_)
		return false;

	return true;
}

bool SvrTeamOption::check_svrteamoption()
{
	return true;
}

#include <algorithm>

template< class T>
static bool normal_sort( const T* l, const T* r)
{
	return l->sn_ < r->sn_;
}

void SvrTeamOption::build_config()
{
	//css
	NS_STL::list<CSSOption*> css;
	this->get_cssoptions( css);
	css.sort( normal_sort<CSSOption>);

	int index =0;
	for( NS_STL::list<CSSOption*>::iterator iter =css.begin(); iter != css.end(); ++iter, ++index)
		(*iter)->server_index_ =index;
	css.clear();

	//gts
	NS_STL::list<GTSOption*> gts;
	this->get_gtsoptions( gts);
	gts.sort( normal_sort<GTSOption>);

	index =0;
	for( NS_STL::list<GTSOption*>::iterator iter =gts.begin(); iter != gts.end(); ++iter, ++index)
		(*iter)->server_index_ =index;
	gts.clear();

	//计算副本服务器配置
	index =0;
	for( CSSOPTION_MAP::iterator iter =csss_.begin(); iter != csss_.end(); ++iter)
	{
		CSSOption* copt =iter->second;
		NS_STL::list<INSOption*> insopts;
		copt->get_insoptions( insopts);
		insopts.sort( normal_sort<INSOption>);

		for( NS_STL::list<INSOption*>::iterator iter2 =insopts.begin(); iter2 != insopts.end(); ++iter2)
		{
			INSOption* iopt =(*iter2);
			for( int ii =0; ii < iopt->nums_; ++ii)
			{
				INSCellOption* cellopt =STATICRES_NEW INSCellOption();
				cellopt->set_cellid( index);
				cellopt->ins_opt_ =iopt;

				iopt->inscells_.push_back( cellopt);
				inscells_[index] =cellopt;

				++index;
			}
		}
	}
}
