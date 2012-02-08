/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/world/TeleportConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

TeleportConfig::TeleportConfig()
{
}

bool TeleportConfig::init_teleports( TiXmlElement* root)
{
	//map port
	TiXmlElement* mps =root->FirstChildElement( "maps");
	if( mps == 0)
		return false;

	for( TiXmlElement* mp =mps->FirstChildElement("map"); mp != 0; mp =mp->NextSiblingElement( "map"))
	{
		int mid =XmlUtil::GetXmlAttrInt( mp, "id");
		
		NS_STL::pair<int, Teleport> vs( mid, Teleport());
		NS_STL::pair< NS_STL::map< int, Teleport>::iterator, bool> rt;

		rt =teleports_.insert( vs);
		if( rt.second == false)
			return false;

		Teleport& mt =(rt.first)->second;

		mt.set_mapid( mid);

		for( TiXmlElement* tp =mp->FirstChildElement("tp"); tp != 0; tp =tp->NextSiblingElement( "tp"))
		{
			teleport_info ti;

			ti.id_ =XmlUtil::GetXmlAttrInt( tp, "id");
			ti.ownermapid_ =mid;
			ti.pos_.x_ =XmlUtil::GetXmlAttrFloat( tp, "x");
			ti.pos_.y_ =XmlUtil::GetXmlAttrFloat( tp, "y");
			ti.pos_.z_ =XmlUtil::GetXmlAttrFloat( tp, "z");
			ti.face_ =XmlUtil::GetXmlAttrFloat( tp, "f");

			mt.set_teleportinfo( ti);
		}
	}

	//ports
	TiXmlElement* tps =root->FirstChildElement( "teleports");
	if( tps == 0)
		return false;

	//解析所有的转跳点
	for( TiXmlElement* ps =tps->FirstChildElement( "port"); ps != 0; ps =ps->NextSiblingElement( "port"))
	{
		int id =XmlUtil::GetXmlAttrInt( ps, "id");
		if( get_teleportpair( id))
			return false;

		NS_STL::pair<int,TeleportPair> vv( id, TeleportPair());
		NS_STL::pair< NS_STL::map< int, TeleportPair>::iterator, bool> rt;

		rt =teleportpairs_.insert( vv);
		if( rt.second == false)
			return false;
		
		TeleportPair& tp =(rt.first)->second;
		tp.set_iid( id);

		NS_STL::vector<int> ss;
		if( !ShareUtil::splitstr2int( XmlUtil::GetXmlAttrStr( ps, "from").c_str(), ":", ss))
			return false;

		if( ss.size() != 2)
			return false;

		Teleport* mt =get_mapteleport( ss[0]);
		if( mt == 0)
			return false;

		teleport_info* ti =mt->get_teleportinfo( ss[1]);
		if( ti == 0)
			return false;

		tp.set_from( ti);

		for( TiXmlElement* t =ps->FirstChildElement( "to"); t != 0; t =t->NextSiblingElement( "to"))
		{
			int dd =XmlUtil::GetXmlAttrInt( t, "id");

			NS_STL::vector<int> ss1;
			if( !ShareUtil::splitstr2int( XmlUtil::GetXmlAttrStr( t, "tp").c_str(), ":", ss1))
				return false;
			if( ss1.size() != 2)
				return false;

			Teleport* mt1 =get_mapteleport( ss1[0]);
			if( mt1 == 0)
				return false;

			teleport_info* ti1 =mt1->get_teleportinfo( ss1[1]);
			if( ti1 == 0)
				return false;

			tp.add_to( dd, ti1);
		}
	}

	return true;
}

Teleport* TeleportConfig::get_mapteleport( int mapid)
{
	ALLTELEPORT_MAP::iterator fiter =teleports_.find( mapid);
	if( fiter == teleports_.end())
		return 0;
	return &(fiter->second);
}

TeleportPair* TeleportConfig::get_teleportpair( int iid)
{
	TELEPORTPAIR_MAP::iterator fiter =teleportpairs_.find( iid);
	if( fiter == teleportpairs_.end())
		return 0;
	return &(fiter->second);
}
