/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/world/WorldInfoConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

WorldInfoConfig::WorldInfoConfig()
{
}

WorldInfoConfig::~WorldInfoConfig()
{
	release();
}

bool WorldInfoConfig::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	//ע�ḱ�������ű�
	if( !context.run_script( inst_script_.c_str()))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "regist appdata/maps/ins/inst-base.lua script file failed");
		return false;
	}

	//ע����������Ľű�
	INSTSTORYOPTION_MAP::iterator eiter =instmaps_.end();
	for( INSTSTORYOPTION_MAP::iterator iter =instmaps_.begin(); iter != eiter; ++iter)
	{
		StoryMapOption* opt =iter->second;
		if( opt->instmap_opt_.inst_script_ != "")
		{
			if( !context.run_script( opt->instmap_opt_.inst_script_.c_str()))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "regist appdata/maps/ins/%d/inst-impl.lua script file failed", opt->get_mapid());
				return false;
			}
		}
	}

	return true;
}

void WorldInfoConfig::release()
{
	bornpos_.clear();

	for( MAINSTORYOPTION_MAP::iterator iter =mainmaps_.begin(); iter != mainmaps_.end(); ++iter)
		delete iter->second;
	mainmaps_.clear();

	for( INSTSTORYOPTION_MAP::iterator iter =instmaps_.begin(); iter != instmaps_.end(); ++iter)
		delete iter->second;
	instmaps_.clear();
}

bool WorldInfoConfig::load_worldconfig()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	NS_STL::string cnt =dsrc->get_txtfilecontent( "appdata", "worlddef.xml");

	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();

	worldwidth_ =XmlUtil::GetXmlAttrInt( root, "width", 0);
	worldheight_ =XmlUtil::GetXmlAttrInt( root, "height", 0);

	//���س�����
	TiXmlElement* bn =root->FirstChildElement( "bornpos");
	if( bn)
	{
		for( TiXmlElement* e =bn->FirstChildElement( "born"); e != NULL; e =e->NextSiblingElement( "born"))
		{
			world_bornpos born;
			born.posx_ =XmlUtil::GetXmlAttrFloat( e, "x", 0);
			born.posy_ =XmlUtil::GetXmlAttrFloat( e, "y", 0);
			born.posz_ =XmlUtil::GetXmlAttrFloat( e, "z", 0);
			born.face_ =XmlUtil::GetXmlAttrFloat( e, "f", 0);

			S_INT_32 bnid =XmlUtil::GetXmlAttrInt( e, "race", 0);
			if( bnid < 0)
				return false;

			bornpos_[bnid] =born;
		}
	}

	//����ת����
	TiXmlElement* teleports =root->FirstChildElement( "allteleports");
	if( teleports)
	{
		if( !teleport_config_.init_teleports( teleports))
			return false;
	}

	TiXmlElement* transcripttels =root->FirstChildElement( "transcripts-teleports");
	if( transcripttels)
	{
		if( !totranscript_config_.init_teleports( transcripttels))
			return false;
	}

	//�������߼���ͼ
	TiXmlElement* maps =root->FirstChildElement( "mainstorys");
	if( maps == 0)
		return false;

	for( TiXmlElement* e =maps->FirstChildElement( "map"); e != NULL; e =e->NextSiblingElement( "map"))
	{
		S_INT_32 mid =XmlUtil::GetXmlAttrInt( e, "id", NO_INITVALUE);
		if( mid == NO_INITVALUE)
			continue;
		if( get_mainstorymapres( mid))
			continue;

		S_INT_32 offsetx, offsety;
		offsetx =XmlUtil::GetXmlAttrInt( e, "offsetx", 0);
		offsety =XmlUtil::GetXmlAttrInt( e, "offsety", 0);

		StoryMapOption* mr =STATICRES_NEW StoryMapOption( "appdata/maps/ms/", mid, offsetx, offsety, false);
		if( !mr->load_mapres())
		{
			MODULE_LOG_ERROR( MODULE_BOOT, ".......map resource load failed[appdata/maps/ms/%d]............", mid);
			return false;
		}

		mr->mapdesc_ =XmlUtil::GetXmlAttrStr( e, "desc");
		mr->owner_css_ =GLOBALCONFIG_INS->get_cssoptbymapid( mid);
		ACE_ASSERT( mr->owner_css_ != 0);

		mainmaps_[mid] =mr;
	}

	//���ظ��������ű�
	inst_script_ =dsrc->get_txtfilecontent( "appdata/maps/ins", "inst-base.lua");
	if( inst_script_.size() <= 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load instance base lua script file(appdata/maps/ins/inst-base.lua) failed");
		return false;
	}

	//���ظ�������
	TiXmlElement* inststorys =root->FirstChildElement( "inststorys");
	if( inststorys)
	{
		for( TiXmlElement* e =inststorys->FirstChildElement( "ins"); e != NULL; e =e->NextSiblingElement( "ins"))
		{
			S_INT_32 mid =XmlUtil::GetXmlAttrInt( e, "id", NO_INITVALUE);
			if( mid == NO_INITVALUE)
				continue;
			if( get_instancemapres( mid))
				continue;

			StoryMapOption* mr =STATICRES_NEW StoryMapOption( "appdata/maps/ins/", mid, 0, 0, true);
			if( !mr->load_mapres())
			{
				MODULE_LOG_ERROR( MODULE_BOOT, ".......map resource load failed[appdata/maps/ins/%d]............", mid);
				return false;
			}

			mr->mapdesc_ =XmlUtil::GetXmlAttrStr( e, "desc");

			instmaps_[mid] =mr;
		}
	}

	return true;
}

StoryMapOption* WorldInfoConfig::get_mainstorybyxy( S_FLOAT_32 x, S_FLOAT_32 y)
{
	MAINSTORYOPTION_MAP::iterator iter =mainmaps_.begin(), eiter =mainmaps_.end();
	for( ; iter != eiter; ++iter)
	{
		StoryMapOption* opt =iter->second;
		if( opt->worldxy_inthismap( x, y))
			return opt;
	}

	return 0;
}

StoryMapOption* WorldInfoConfig::get_mainstorymapres( S_INT_32 mid)
{
	MAINSTORYOPTION_MAP::iterator fiter =mainmaps_.find( mid);
	if( fiter == mainmaps_.end())
		return 0;
	return fiter->second;
}

void WorldInfoConfig::get_mainstorymapres( NS_STL::list<int>& mids, NS_STL::vector<StoryMapOption*>& mr)
{
	if( mids.size() == 0)
	{
		//��ȡ���е�
		for( MAINSTORYOPTION_MAP::iterator iter =mainmaps_.begin(); iter != mainmaps_.end(); ++iter)
			mr.push_back( iter->second);
	}
	else
	{
		for( NS_STL::list<int>::iterator iter =mids.begin(); iter != mids.end(); ++iter)
		{
			StoryMapOption* p =get_mainstorymapres( *iter);
			if( p == 0)
				continue;
			mr.push_back( p);
		}
	}
}

StoryMapOption* WorldInfoConfig::get_instancemapres( S_INT_32 insid)
{
	INSTSTORYOPTION_MAP::iterator fiter =instmaps_.find( insid);
	if( fiter == instmaps_.end())
		return 0;
	return fiter->second;
}

S_INT_32 WorldInfoConfig::get_mainstorynum()
{
	return (S_INT_32)mainmaps_.size();
}

S_INT_32 WorldInfoConfig::get_insstorynum()
{
	return (S_INT_32)instmaps_.size();
}

world_bornpos* WorldInfoConfig::get_bornpos( S_INT_32 race)
{
	WORLDBORNPOS_MAP::iterator fiter =bornpos_.find( race);
	if( fiter == bornpos_.end())
		return 0;
	return &(fiter->second);
}
