/**
* reslib application
* 
* @category		buffer config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/buffers/BufferConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

BufferConfig::BufferConfig()
{

}

BufferConfig::~BufferConfig()
{

}

bool BufferConfig::load_bufferconfig()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	buffbase_script_ =dsrc->get_txtfilecontent( "appdata/buffs", "bufferbase.lua");
	if( buffbase_script_.size() <= 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load bufferbase lua script file(appdata/buffs/bufferbase.lua) failed");
		return false;
	}

	NS_STL::string cnt =dsrc->get_txtfilecontent( "appdata/buffs", "bufferdefine.xml");
	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();

	NS_STL::string buftag[] ={ "sys_buffer", "app_buffer"};
	int	buftagmask[] ={ BUFFERID_HEAD_SYS, BUFFERID_HEAD_APP};
	for( int ind =0; ind < sizeof( buftagmask)/sizeof( int); ++ind)
	{
		TiXmlElement* syss =root->FirstChildElement( buftag[ind].c_str());
		if( syss)
		{
			for( TiXmlElement* e =syss->FirstChildElement( "buffer"); e != NULL; e =e->NextSiblingElement( "buffer"))
			{
				NS_STL::string sid =XmlUtil::GetXmlAttrStr( e, "id");
				int iid =ShareUtil::atoi( sid.c_str(), 16);

				NS_STL::pair< BUFFERDESC_MAP::iterator, bool> mp1 =
					buffers_.insert( NS_STL::make_pair( iid, BufferDesc()));

				if( !mp1.second)
				{
					MODULE_LOG_ERROR( MODULE_BOOT, "load appdata/buffs/bufferdefine.xml buffer:%s redfined", sid.c_str());
					return false;
				}

				BufferDesc& sd =mp1.first->second;
				sd.buffer_id_ =iid;
				sd.save2db_ =XmlUtil::GetXmlAttrBool( e, "save2db", false);
				sd.scopetype_ =( char)XmlUtil::GetXmlAttrInt( e, "scopetype", 0);
				sd.scopetime_ =XmlUtil::GetXmlAttrInt( e, "scopetime", 0);
				sd.tickstep_ =XmlUtil::GetXmlAttrInt( e, "tickstep", 500);
				sd.notifyother_ =XmlUtil::GetXmlAttrBool( e, "notifyother", false);
				sd.affectbaseprop_ =XmlUtil::GetXmlAttrBool( e, "affectbaseprop", false);
				sd.scripttick_ =XmlUtil::GetXmlAttrBool( e, "scripttick", false);
				sd.script_file_ =XmlUtil::GetXmlAttrStr( e, "buffscript", "");

				if( sd.is_supportscript())
					sd.script_content_ =dsrc->get_txtfilecontent( "appdata/buffs", sd.get_scriptfile().c_str());
				else
				{
					sd.affectbaseprop_ =false;
					sd.scripttick_ =false;
				}

				if( !sd.validate_buffer( buftagmask[ind]))
				{
					MODULE_LOG_ERROR( MODULE_BOOT, "appdata/buffs/bufferdefine.xml buffer:%s validate error", sid.c_str());
					return false;
				}
			}
		}
	}

	return true;
}

void BufferConfig::unload_bufferconfig()
{
	buffers_.clear();
}

bool BufferConfig::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	//注册基础类
	if( !context.run_script( buffbase_script_.c_str()))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "run buffer appdata/buffs/bufferbase.lua script file failed");
		return false;
	}

	BUFFERDESC_MAP::iterator iter =buffers_.begin(), eiter =buffers_.end();
	for( ; iter != eiter; ++iter)
	{
		BufferDesc& sd =iter->second;
		if( !sd.is_supportscript())
			continue;

		if( !context.run_script( sd.script_content_.c_str()))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load buffer appdata/buffs/%s.lua script file failed", sd.script_file_.c_str());
			return false;
		}
	}

	return true;
}

BufferDesc* BufferConfig::get_bufferbycode( int bufcode)
{
	BUFFERDESC_MAP::iterator fiter =buffers_.find( bufcode);
	if( fiter == buffers_.end())
		return 0;
	return &(fiter->second);
}
