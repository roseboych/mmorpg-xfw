/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include <corelib/script/ScriptContext.h>
#include <corelib/log/logmacro.h>

#include "character/UnitBase.h"
#include "character/Player.h"

#include <reslib/items/ItemsConfig.h>
#include <reslib/skills/SkillTreeConfig.h>
#include <reslib/buffers/BufferConfig.h>
#include <reslib/gamesetting/XGameSetting.h>

#include "character/playerbuffer/BufferRuntime.h"

#include "character/playerskills/SkillDataPackageBase.h"
#include "character/playerskills/SkillNearFightDP.h"
#include "character/playerskills/SkillFarFightDP.h"

NAMESPACE_APPSCRIPT_BEGIN

bool ScriptContext::regist_bindclass()
{
	if( !GAMESETTING_CFG->regist_to_storyscriptcontext( *this))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "game setting script regist to script context failed.......");
		return false;
	}

	//注册脚本环境
	UnitBase::script_regist( lua_state_);
	Player::script_regist( lua_state_);

	//物品注册
	if( !ITEMRESMGR_INS->regist_to_storyscriptcontext( *this))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "item resource script regist to script context failed.......");
		return false;
	}

	if( !SKILLTREE_CFG->regist_to_storyscriptcontext( *this))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "skill script regist to script context failed.......");
		return false;
	}

	if( !BUFFER_CFG->regist_to_storyscriptcontext( *this))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "buffer module script regist to script context failed.......");
		return false;
	}

	//注册bufferruntime
	BufferRuntime::script_regist( lua_state_);

	//注册技能包
	SkillDataPackageBase::script_regist( lua_state_);
	SkillNearFightDP::script_regist( lua_state_);
	SkillFarFightDP::script_regist( lua_state_);

	return true;
}

NAMESPACE_APPSCRIPT_END
