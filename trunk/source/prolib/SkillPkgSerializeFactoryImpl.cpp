/**
* protocol module
* 
* @category		skill package serialize
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/SkillPkgSerializeFactoryImpl.h"

#include "prolib/datastore/dtpkg_skill.h"

USE_PROTOCOL_NAMESPACE

SkillPkgSerializeFactoryImpl::SkillPkgSerializeFactoryImpl()
{
}

BEGIN_SERIALIZEOBJ_FACTORY( SkillPkgSerializeFactoryImpl, SERIALIZEFACTORY_SKILLPKG)
	SERIALIZEOBJ_MAPPING( SKILLDATAPKG_NEARFIGHT, skpkg_nearfight)
	SERIALIZEOBJ_MAPPING( SKILLDATAPKG_FARFIGHT, skpkg_farfight)
END_SERIALIZEOBJ_FACTORY( SERIALIZEFACTORY_SKILLPKG)
