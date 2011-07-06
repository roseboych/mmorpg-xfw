/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/CSSOption.h"

#include "reslib/deploy/SvrTeamOption.h"
#include "reslib/deploy/INSOption.h"

#include <algorithm>

CSSOption::CSSOption():
svrteamopt_( 0),
server_index_( 0)
{
}

CSSOption::~CSSOption()
{
	release();
}

void CSSOption::release()
{
	for( INSOPT_MAP::iterator iter =inss_.begin(); iter != inss_.end(); ++iter)
		delete iter->second;
	inss_.clear();
}

S_INT_32 CSSOption::get_maxuser()
{
	return svrteamopt_->maxuser_;
}

S_INT_32 CSSOption::get_mapsize()
{
	return (S_INT_32)mapids_.size();
}

S_INT_32 CSSOption::get_instancesize()
{
	S_INT_32 ret =0;

	INSOPT_MAP::iterator iter, eiter =inss_.end();
	for( iter =inss_.begin(); iter != eiter; ++iter)
	{
		INSOption* opt =iter->second;
		ret += opt->nums_;
	}

	return ret;
}

bool CSSOption::test_mapincss( S_INT_32 mid)
{
	MAPS_LIST::iterator fiter =NS_STL::find( mapids_.begin(), mapids_.end(), mid);
	return fiter != mapids_.end();
}

INSOption* CSSOption::get_insoption( S_INT_32 sn)
{
	INSOPT_MAP::iterator fiter =inss_.find( sn);
	if( fiter == inss_.end())
		return 0;

	return fiter->second;
}

void CSSOption::get_insoptions( NS_STL::list<INSOption*>& ret)
{
	INSOPT_MAP::iterator iter, eiter =inss_.end();
	for( iter =inss_.begin(); iter != eiter; ++iter)
		ret.push_back( iter->second);
}