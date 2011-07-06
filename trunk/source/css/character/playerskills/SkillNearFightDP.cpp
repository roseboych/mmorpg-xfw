/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "SkillNearFightDP.h"

#include <corelib/script/ScriptContext.h>
#include <corelib/log/logmacro.h>

SkillNearFightDP::SkillNearFightDP():inherit()
{

}

SkillNearFightDP::~SkillNearFightDP()
{
}

void SkillNearFightDP::script_regist( lua_State* l)
{
	using namespace luabind;

	module( l)
	[
		class_<SkillNearFightDP, SkillDataPackageBase>( "SkillNearFightDP")
			.def(constructor<>())
	];
}

void SkillNearFightDP::init_skillruntime()
{

}
