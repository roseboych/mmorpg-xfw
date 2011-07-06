/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/world/ToTranscriptConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

ToTranscriptConfig::ToTranscriptConfig()
{

}

bool ToTranscriptConfig::init_teleports(TiXmlElement *root)
{
	for( TiXmlElement* tels =root->FirstChildElement("teleport"); tels != 0; tels =tels->NextSiblingElement( "teleport"))
	{
		S_INT_32 mid =XmlUtil::GetXmlAttrInt( tels, "id");

		NS_STL::pair<S_INT_32, transcript_teleport_info> vs( mid, transcript_teleport_info());
		NS_STL::pair< TRANSCRIPTTELEPORTS_MAP::iterator, bool> rt;

		rt =all_transcript_teleports_.insert( vs);
		if( rt.second == false)
			return false;

		transcript_teleport_info& ti =(rt.first)->second;
		ti.id_ =mid;

		ti.mainmap_id_ =XmlUtil::GetXmlAttrInt( tels, "mainmap");
		ti.mainmap_pos_.x_ =XmlUtil::GetXmlAttrFloat( tels, "mx");
		ti.mainmap_pos_.y_ =XmlUtil::GetXmlAttrFloat( tels, "my");
		ti.mainmap_pos_.z_ =XmlUtil::GetXmlAttrFloat( tels, "mz");

		ti.instmap_id_ =XmlUtil::GetXmlAttrInt( tels, "instmap");
		ti.instmap_pos_.x_ =XmlUtil::GetXmlAttrFloat( tels, "ix");
		ti.instmap_pos_.y_ =XmlUtil::GetXmlAttrFloat( tels, "iy");
		ti.instmap_pos_.z_ =XmlUtil::GetXmlAttrFloat( tels, "iz");
	}

	return true;
}

transcript_teleport_info* ToTranscriptConfig::get_teleportinfobyid( S_INT_32 telid)
{
	TRANSCRIPTTELEPORTS_MAP::iterator fiter =all_transcript_teleports_.find( telid);
	if( fiter == all_transcript_teleports_.end())
		return 0;
	return &(fiter->second);
}
