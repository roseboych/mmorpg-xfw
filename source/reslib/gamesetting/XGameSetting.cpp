/**
* css application
* res module
* @category		game setting
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/gamesetting/XGameSetting.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

XGameSetting::XGameSetting()
{
}

XGameSetting::~XGameSetting()
{
}

bool XGameSetting::load_gamesetting()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	if( dsrc == 0)
		return false;

	script_character_property_init_ =dsrc->get_txtfilecontent( "appdata/character", "character_property.lua");
	if( script_character_property_init_.size() <= 0)
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load character property initialize lua script file(appdata/character/character_property.lua) failed");
		return false;
	}

	return true;
}

bool XGameSetting::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	if( !context.run_script( script_character_property_init_.c_str()))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "run game setting appdata/character/character_property.lua script file failed");
		return false;
	}

	return true;
}
