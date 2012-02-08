/**
* reslib application
* 
* @category		buffer config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BUFFERCONFIG__H__
#define __BUFFERCONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <map>

#include "reslib/buffers/BufferDesc.h"

#include <corelib/corelibdef.h>
#include <prolib/Pro_define.h>

NAMESPACE_APPSCRIPT_BEGIN

class ScriptContext;

NAMESPACE_APPSCRIPT_END

/**
* @class BufferConfig
* 
* @brief buffer配置
**/
class BufferConfig
{
public:
	typedef NS_STL::map<int, BufferDesc>	BUFFERDESC_MAP;

public:
	BufferConfig();
	~BufferConfig();

	bool load_bufferconfig();
	void unload_bufferconfig();

	/**
	* 注册所有的buffer脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	/**
	* 根据buffer编号获取描述
	* @param buffcode
	* @return
	**/
	BufferDesc* get_bufferbycode( int bufcode);

protected:
	//buffer描述
	BUFFERDESC_MAP	buffers_;

	//buffer的初始化lua脚本
	NS_STL::string buffbase_script_;
};

#define BUFFER_CFG	ACE_Singleton< BufferConfig, ACE_Null_Mutex>::instance()

#endif	//__BUFFERCONFIG__H__
