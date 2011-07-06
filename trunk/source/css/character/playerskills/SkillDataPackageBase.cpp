/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "SkillDataPackageBase.h"

#include <corelib/script/ScriptContext.h>
#include <prolib/datastore/skill_define.h>
#include "SkillNearFightDP.h"
#include "SkillFarFightDP.h"

SkillDataPackageBase::SkillDataPackageBase()
{

}

SkillDataPackageBase::~SkillDataPackageBase()
{

}

SkillDataPackageBase* SkillDataPackageBase::get_skilldatapackage( S_INT_16 iid)
{
	if( iid == SKILLDATAPKG_NEARFIGHT)
		return FRAMEWK_NEW SkillNearFightDP();
	else if( iid == SKILLDATAPKG_FARFIGHT)
		return FRAMEWK_NEW SkillFarFightDP();
	else
		return 0;
}

void SkillDataPackageBase::script_regist( lua_State* l)
{
	using namespace luabind;

	module( l)
	[
		class_<SkillDataPackageBase>( "SkillDataPackageBase")
			.def(constructor<>())
	];
}
