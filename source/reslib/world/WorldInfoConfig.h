/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MAPRESMGR__H__
#define __MAPRESMGR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <corelib/corelibdef.h>

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include "reslib/world/res_struct_def.h"
#include "reslib/world/StoryMapOption.h"
#include "reslib/world/TeleportConfig.h"
#include "reslib/world/ToTranscriptConfig.h"

#include <string>

NAMESPACE_APPSCRIPT_BEGIN
class ScriptContext;
NAMESPACE_APPSCRIPT_END

/**
* @class WorldInfoConfig
* 
* @brief 世界信息配置
**/
class WorldInfoConfig
{
	typedef NS_STL::map< S_INT_32, world_bornpos>	WORLDBORNPOS_MAP;
	typedef NS_STL::map< S_INT_32, StoryMapOption*>	MAINSTORYOPTION_MAP;
	typedef NS_STL::map< S_INT_32, StoryMapOption*>	INSTSTORYOPTION_MAP;

public:
	WorldInfoConfig();
	~WorldInfoConfig();

	/**
	* 初始化地图资源
	* @return
	**/
	bool load_worldconfig();

	/**
	* 注册所有的buffer脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	/**
	* 获取主逻辑中使用的地图资源
	* @param mid
	* @return
	**/
	StoryMapOption* get_mainstorymapres( S_INT_32 mid);
	void			get_mainstorymapres( NS_STL::list<int>& mids, NS_STL::vector<StoryMapOption*>& mr);
	StoryMapOption* get_mainstorybyxy( S_FLOAT_32 x, S_FLOAT_32 y);

	/**
	* 根据地图id获取副本地图描述
	* @param ct
	* @return
	**/
	StoryMapOption* get_instancemapres( S_INT_32 insid);

	/**
	* 计算总的地图数,包括副本数量，用来计算需要启动的线程数
	**/
	S_INT_32			get_mainstorynum();
	S_INT_32			get_insstorynum();

	world_bornpos*		get_bornpos( S_INT_32 race);

	TeleportConfig&		get_teleportconfig(){ return teleport_config_;}
	ToTranscriptConfig& get_totranscriptconfig(){ return totranscript_config_;}

	int get_worldwidth(){ return worldwidth_;}
	int get_worldheight(){ return worldheight_;}

protected:
	void release();

private:
	//整个世界的长宽
	int		worldwidth_;
	int		worldheight_;

	//出生点
	WORLDBORNPOS_MAP		bornpos_;
	//转跳点
	TeleportConfig			teleport_config_;
	//副本转跳点
	ToTranscriptConfig		totranscript_config_;

	//ms地图资源
	MAINSTORYOPTION_MAP		mainmaps_;
	//ins地图资源
	INSTSTORYOPTION_MAP		instmaps_;
	//副本脚本
	NS_STL::string			inst_script_;
};

#define WORLDINFO ACE_Singleton< WorldInfoConfig, ACE_Null_Mutex>::instance()

#endif	//__MAPRESMGR__H__
