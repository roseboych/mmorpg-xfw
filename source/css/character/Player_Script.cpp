/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <corelib/script/ScriptContext.h>
#include "playerbuffer/BufferRuntime.h"

void Player::script_regist( lua_State* l)
{
	using namespace luabind;

	module( l)
	[
		class_<Player,UnitBase>( "Player")
			.def(constructor<>())
			.property( "sex", &PlayerData::get_sex)
			.property( "race", &PlayerData::get_race)
			.property( "profession", &PlayerData::get_profession)
			.property( "level", &PlayerData::get_level)
			.property( "bagslot", &PlayerData::get_bagslots)
			.property( "money", &PlayerData::get_money, &PlayerData::set_money)
			.property( "hp", &PlayerData::get_hp, &PlayerData::set_hp)
			.property( "mp", &PlayerData::get_mp, &PlayerData::set_mp)
			.property( "died", &PlayerData::get_died, &PlayerData::set_died)
			.def_readwrite( "maxhp", &PlayerData::max_hp_)
			.def_readwrite( "maxmp", &PlayerData::max_mp_)
			.def_readwrite( "maxexp", &PlayerData::max_exp_)
			.def_readwrite( "force", &PlayerData::prop_force_)
			.def_readwrite( "agile", &PlayerData::prop_agile_)
			.def_readwrite( "endurance", &PlayerData::prop_endurance_)
			.def_readwrite( "intellect", &PlayerData::prop_intellect_)
			.def_readwrite( "spirit", &PlayerData::prop_spirit_)
			.def_readwrite( "armor", &PlayerData::prop_armor_)
			.def( "attachbuffer", (BufferRuntime* (PlayerData::*)(S_INT_32))&PlayerData::attach_newbuffer)
			.def( "cancelbuffer", (void (PlayerData::*)(S_INT_32))&PlayerData::cancel_onebuffer)
	];
}
