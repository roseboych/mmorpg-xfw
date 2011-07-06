/**
* reslib application
*
* @category		game setting
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __XGAMESETTING__H__
#define __XGAMESETTING__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
	#pragma once
#endif

#include "reslib/reslib_def.h"

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <corelib/corelibdef.h>

#include <string>

NAMESPACE_APPSCRIPT_BEGIN

class ScriptContext;

NAMESPACE_APPSCRIPT_END

class XGameSetting
{
public:
	XGameSetting();
	~XGameSetting();

	bool load_gamesetting();

	/**
	* 注册所有的gamesetting脚本到脚本环境
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

private:
	//gamesetting初始化脚本
	//character property初始化脚本
	NS_STL::string script_character_property_init_;
};

#define GAMESETTING_CFG	ACE_Singleton< XGameSetting, ACE_Null_Mutex>::instance()

#endif	//__XGAMESETTING__H__
