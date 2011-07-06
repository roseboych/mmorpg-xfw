/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "Player.h"

#include <ace/OS_NS_sys_time.h>
#include <prolib/Pro_all.h>
#include <corelib/script/ScriptContext.h>

#include "../CSSModule.h"

void Player::calcuate_playerproperty()
{
	//reset all properties
	prop_force_ =prop_agile_ = prop_armor_ = prop_endurance_ = prop_intellect_ = prop_spirit_ =0;
	max_hp_ =max_mp_ =max_exp_ =0;

	app::script::ScriptContext& scriptc =CONTENTSERVICE_INS->get_scriptcontext();

	//根据level,race初始化属性
	try{
		luabind::call_function<void>( scriptc.get_luastate(), "init_character_property", this);
	}
	catch( ...){
		lua_pop( scriptc.get_luastate(), 1);
	}

	//附加上额外的刷新属性
	inc_force( baseinfo_.power_);
	inc_agile( baseinfo_.agile_);
	inc_armor( baseinfo_.armor_);
	inc_endurance( baseinfo_.endurance_);
	inc_intellect( baseinfo_.intellect_);
	inc_spirit( baseinfo_.spirit_);

	//计算装备对属性的影响
	for( int ind =0; ind < PRO::AVATAR_SLOT_MAX; ++ind)
	{
		if( avatar_items_[ind] == 0)
			continue;

		avatar_items_[ind]->calcuate_playerproperty( this);
	}

	//计算buffer对属性的影响
	mybuffers_.affect_playerprops( scriptc);

	//规整属性值范围
	try{
		luabind::call_function<void>( scriptc.get_luastate(), "max_character_property", this);
	}
	catch( ...){
		lua_pop( scriptc.get_luastate(), 1);
	}
}
