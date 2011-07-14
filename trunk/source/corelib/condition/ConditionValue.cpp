/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionValue.h"

#include "corelib/condition/ConditionContext.h"

//--------------------------------------------ConditionValueConst-------------------------------------------------
ConditionValueConst::ConditionValueConst( S_INT_32 val)
{
	const_value_.set_int( val);
}

ConditionValueConst::ConditionValueConst( S_FLOAT_32 val)
{
	const_value_.set_float( val);
}

ConditionValueConst::ConditionValueConst( const S_CHAR_8* val)
{
	const_value_.set_str( val);
}

PropertyOperatorValue ConditionValueConst::get_conditionvalue( ConditionContext& context)
{
	return const_value_;
}


//-------------------------------------------ConditionValueObject------------------------------------------------
ConditionValueObject::ConditionValueObject( const char* objname, const char* propname):
objname_( objname),
propname_( propname)
{
}

PropertyOperatorValue ConditionValueObject::get_conditionvalue( ConditionContext& context)
{
	IPropertyOperator* obj =context.get_object( objname_.c_str());
	ACE_ASSERT( obj != 0);

	return obj->get_property( propname_.c_str());
}
