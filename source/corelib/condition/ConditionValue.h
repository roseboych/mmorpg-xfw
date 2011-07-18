/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __CONDITIONVALUE__H__
#define __CONDITIONVALUE__H__

#include "corelib/corelibdef.h"
#include "corelib/condition/IPropertyOperator.h"

#include <string>

class ConditionContext;
class TiXmlElement;

/**
* @class ConditionValue
* 
* @brief 条件计算中定义的值
**/
class ConditionValue
{
public:
	/**
	* 返回设置的值
	**/
	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context) =0;
};

/**
* @class ConditionValueConst
* 
* @brief 封装了常量值
**/
class ConditionValueConst : public ConditionValue
{
public:
	ConditionValueConst( S_INT_32 val);
	ConditionValueConst( S_FLOAT_32 val);
	ConditionValueConst( const S_CHAR_8* val);

	ConditionValueConst( TiXmlElement* e);

	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context);

protected:
	PropertyOperatorValue	const_value_;
};

/**
* @class ConditionValueObject
* 
* @brief 封装了对象属性
**/
class ConditionValueObject : public ConditionValue
{
public:
	ConditionValueObject( const char* objname, const char* propname);

	ConditionValueObject( TiXmlElement* e);

	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context);

protected:
	NS_STL::string	objname_;
	NS_STL::string  propname_;
};

#endif	//__CONDITIONVALUE__H__
