/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "SkillFarFightDP.h"

#include <corelib/script/ScriptContext.h>

SkillFarFightDP::SkillFarFightDP():inherit()
{

}

SkillFarFightDP::~SkillFarFightDP()
{

}

void SkillFarFightDP::script_regist( lua_State* l)
{
	using namespace luabind;

	module( l)
	[
		class_<SkillFarFightDP, SkillDataPackageBase>( "SkillFarFightDP")
			.def(constructor<>())
	];
}

void SkillFarFightDP::init_skillruntime()
{

}
