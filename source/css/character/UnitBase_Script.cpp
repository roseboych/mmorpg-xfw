/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "UnitBase.h"

#include <corelib/script/ScriptContext.h>

using namespace luabind;

void UnitBase::script_regist( lua_State* l)
{
	module(l)
	[
		class_<UnitBase>( "UnitBase")
			.def(constructor<>())
			.property( "id", &UnitBase::get_id)
			.property("posx", &UnitBase::get_positionx, &UnitBase::set_positionx)
			.property("posy", &UnitBase::get_positiony, &UnitBase::set_positiony)
			.property("posz", &UnitBase::get_positionz, &UnitBase::set_positionz)
	];
}
