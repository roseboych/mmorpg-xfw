/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "prolib/prolibdef.h"

#include "prolib/SerializeFactoryContainer.h"
#include "prolib/SkillPkgSerializeFactoryImpl.h"

struct SingletonT<class SerializeFactoryContainer>::object_creator SingletonT<class SerializeFactoryContainer>::creator_;

SerializeFactoryContainer::SerializeFactoryContainer()
{
	factory_[SERIALIZEFACTORY_SKILLPKG] =PRO_NEW_EX SkillPkgSerializeFactoryImpl();
}

SerializeFactoryContainer::~SerializeFactoryContainer()
{
	for( int ii =0; ii < SERIALIZEFACTORY_MAX; ++ii)
	{
		delete factory_[ii];
		factory_[ii] =0;
	}
}

SerializeObjectFactory* SerializeFactoryContainer::get_factory( S_INT_8 type)
{
	if( type <= SERIALIZEFACTORY_NONE || type >= SERIALIZEFACTORY_MAX)
		return 0;

	return factory_[type];
}
