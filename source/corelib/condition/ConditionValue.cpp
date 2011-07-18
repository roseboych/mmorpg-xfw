/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionValue.h"

#include "corelib/condition/ConditionContext.h"
#include "corelib/xml/XmlUtil.h"

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

ConditionValueConst::ConditionValueConst( TiXmlElement* e)
{
	NS_STL::string ctype =XmlUtil::GetXmlAttrStr( e, "consttype", "int");
	if( ctype == "int")
	{
		S_INT_32 v =XmlUtil::GetXmlAttrInt( e, "constvalue", 0);
		const_value_.set_int( v);
	}
	else if( ctype == "float")
	{
		S_FLOAT_32 v =XmlUtil::GetXmlAttrFloat( e, "constvalue", 0);
		const_value_.set_float( v);
	}
	else if( ctype == "string")
	{
		NS_STL::string v =XmlUtil::GetXmlAttrStr( e, "constvalue", "");
		const_value_.set_str( v.c_str());
	}
	else
	{
		ACE_ASSERT( false && "not supported condition const value");
	}
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

ConditionValueObject::ConditionValueObject( TiXmlElement* e)
{
	objname_ =XmlUtil::GetXmlAttrStr( e, "objectname", "");
	propname_ =XmlUtil::GetXmlAttrStr( e, "propertyname", "");
	ACE_ASSERT( objname_ != "" && propname_ != "");
}

PropertyOperatorValue ConditionValueObject::get_conditionvalue( ConditionContext& context)
{
	IPropertyOperator* obj =context.get_object( objname_.c_str());
	ACE_ASSERT( obj != 0);

	return obj->get_property( propname_.c_str());
}
