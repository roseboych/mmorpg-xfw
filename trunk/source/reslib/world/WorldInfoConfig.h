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
* @brief ������Ϣ����
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
	* ��ʼ����ͼ��Դ
	* @return
	**/
	bool load_worldconfig();

	/**
	* ע�����е�buffer�ű����ű�����
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	/**
	* ��ȡ���߼���ʹ�õĵ�ͼ��Դ
	* @param mid
	* @return
	**/
	StoryMapOption* get_mainstorymapres( S_INT_32 mid);
	void			get_mainstorymapres( NS_STL::list<int>& mids, NS_STL::vector<StoryMapOption*>& mr);
	StoryMapOption* get_mainstorybyxy( S_FLOAT_32 x, S_FLOAT_32 y);

	/**
	* ���ݵ�ͼid��ȡ������ͼ����
	* @param ct
	* @return
	**/
	StoryMapOption* get_instancemapres( S_INT_32 insid);

	/**
	* �����ܵĵ�ͼ��,������������������������Ҫ�������߳���
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
	//��������ĳ���
	int		worldwidth_;
	int		worldheight_;

	//������
	WORLDBORNPOS_MAP		bornpos_;
	//ת����
	TeleportConfig			teleport_config_;
	//����ת����
	ToTranscriptConfig		totranscript_config_;

	//ms��ͼ��Դ
	MAINSTORYOPTION_MAP		mainmaps_;
	//ins��ͼ��Դ
	INSTSTORYOPTION_MAP		instmaps_;
	//�����ű�
	NS_STL::string			inst_script_;
};

#define WORLDINFO ACE_Singleton< WorldInfoConfig, ACE_Null_Mutex>::instance()

#endif	//__MAPRESMGR__H__
