/**
* reslib application
* 
* @category		item res
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/items/ItemInfoBase.h"

#include "reslib/items/EquipmentItemImpl.h"
#include "reslib/items/ConsumeItemImpl.h"
#include "reslib/items/TaskItemImpl.h"
#include "reslib/items/MerchandiseItemImpl.h"
#include "reslib/items/OtherItemImpl.h"

#include <corelib/script/ScriptContext.h>

using namespace luabind;

void ItemInfoBase::script_regist( lua_State* l)
{
	module(l)
	[
		class_<ItemInfoBase>( "ItemInfoBase")
			.def(constructor<>())
			.def_readonly( "itemid", &ItemInfoBase::itemid_)
	];
}

void EquipmentItemImpl::script_regist( lua_State* l)
{
	module(l)
	[
		class_<EquipmentItemImpl,ItemInfoBase>( "EquipmentItemImpl")
			.def(constructor<>())
	];
}

void ConsumeItemImpl::script_regist( lua_State* l)
{
	module(l)
	[
		class_<ConsumeItemImpl,ItemInfoBase>( "ConsumeItemImpl")
			.def(constructor<>())
	];
}

void TaskItemImpl::script_regist( lua_State* l)
{
	module(l)
	[
		class_<TaskItemImpl,ItemInfoBase>( "TaskItemImpl")
			.def(constructor<>())
	];
}

void MerchandiseItemImpl::script_regist( lua_State* l)
{
	module(l)
	[
		class_<MerchandiseItemImpl,ItemInfoBase>( "MerchandiseItemImpl")
			.def(constructor<>())
	];
}

void OtherItemImpl::script_regist( lua_State* l)
{
	module(l)
	[
		class_<OtherItemImpl,ItemInfoBase>( "OtherItemImpl")
			.def(constructor<>())
	];
}
