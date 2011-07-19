/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/world/StoryMapOption.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"
#include "reslib/deploy/CSSOption.h"

StoryMapOption::StoryMapOption( const char* path, int mapid, int offsetx, int offsety, bool binst):
res_path_( path),
mapid_( mapid),
offsetx_( offsetx),
offsety_( offsety),
is_instancemap_( binst),
owner_css_( 0)
{
	//¹¹ÔìÂ·¾¶
	char buf[16] ={0,};
	NS_STL::string ret =ACE_OS::itoa( mapid, buf, 10);
	res_path_ += ret;
	res_path_ += "/";
}

bool StoryMapOption::load_mapres()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	ACE_ASSERT( dsrc != 0);

	NS_STL::string cnt =dsrc->get_txtfilecontent( res_path_.c_str(), "mapdef.xml");

	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();

	S_INT_32 mid =XmlUtil::GetXmlAttrInt( root, "mapid", NO_INITVALUE);
	if( mapid_ == NO_INITVALUE || mid != mapid_)
		return false;

	width_ =XmlUtil::GetXmlAttrInt( root, "width", 0);
	height_ =XmlUtil::GetXmlAttrInt( root, "height", 0);
	leftx_ =0;
	lefty_ =0;
	bottomx_ =width_ -1;
	bottomy_ =height_ -1;

	if( width_ <= 0 || height_ <= 0)
		return false;

	if( is_instancemap_ && !instmap_opt_.load_instmapopt( root, this))
		return false;

	return true;
}

bool StoryMapOption::worldxy_inthismap( S_FLOAT_32 x, S_FLOAT_32 y)
{
	return ( x >= ( leftx_ + offsetx_) && x <= ( bottomx_ + offsetx_)) &&
		( y >= ( lefty_ + offsety_) && y <= ( bottomy_ + offsety_));
}
