/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionLeaf.h"

#include "corelib/xml/XmlUtil.h"
#include <string>

ConditionLeaf::ConditionLeaf():
left_( 0),
right_( 0),
x_( 0)
{
}

ConditionLeaf::ConditionLeaf( TiXmlElement* e)
{
	TiXmlElement* lparam =e->FirstChildElement( "lparam");
	TiXmlElement* rparam =e->FirstChildElement( "rparam");
	ACE_ASSERT( lparam != 0 && rparam != 0);

	NS_STL::string stype =XmlUtil::GetXmlAttrStr( lparam, "type", "const");
	if( stype == "const")
		left_ =FRAMEWK_NEW ConditionValueConst( lparam);
	else
		left_ =FRAMEWK_NEW ConditionValueObject( lparam);

	stype =XmlUtil::GetXmlAttrStr( rparam, "type", "const");
	if( stype == "const")
		right_ =FRAMEWK_NEW ConditionValueConst( rparam);
	else
		right_ =FRAMEWK_NEW ConditionValueObject( rparam);

	NS_STL::string copt =XmlUtil::GetXmlAttrStr( e, "operator", "great");
	if( copt == "between")
	{
		TiXmlElement* xparam =e->FirstChildElement( "xparam");
		ACE_ASSERT( xparam != 0);

		stype =XmlUtil::GetXmlAttrStr( xparam, "type", "const");
		if( stype == "const")
			x_ =FRAMEWK_NEW ConditionValueConst( xparam);
		else
			x_ =FRAMEWK_NEW ConditionValueObject( xparam);

		operator_ =CONDITION_OPERATOR_BETWEEN;
	}
	else if( copt == "great")
	{
		operator_ =CONDITION_OPERATOR_GREAT;
	}
	else if( copt == "egreat")
	{
		operator_ =CONDITION_OPERATOR_GREATEQUAL;
	}
	else if( copt == "less")
	{
		operator_ =CONDITION_OPERATOR_LESS;
	}
	else if( copt == "eless")
	{
		operator_ =CONDITION_OPERATOR_LESSEQUAL;
	}
	else if( copt == "equal")
	{
		operator_ =CONDITION_OPERATOR_EQUAL;
	}
	else if( copt == "nequal")
	{
		operator_ =CONDITION_OPERATOR_NOTEQUAL;
	}
	else
	{
		ACE_ASSERT( false && "not supported operator type");
	}
}

ConditionLeaf::~ConditionLeaf()
{
	if( left_)
		delete left_;
	left_ =0;

	if( right_)
		delete right_;
	right_ =0;

	if( x_)
		delete x_;
	x_ =0;
}

S_BOOL ConditionLeaf::condition_calcuate( ConditionContext& context)
{
	PropertyOperatorValue lv =left_->get_conditionvalue( context);
	PropertyOperatorValue rv =right_->get_conditionvalue( context);

	if( operator_ == CONDITION_OPERATOR_BETWEEN)
	{
		PropertyOperatorValue xv =x_->get_conditionvalue( context);
		return xv > lv && xv < rv;
	}
	else if( operator_ == CONDITION_OPERATOR_GREAT)
		return lv > rv;
	else if( operator_ == CONDITION_OPERATOR_GREATEQUAL)
		return lv >= rv;
	else if( operator_ == CONDITION_OPERATOR_LESS)
		return lv < rv;
	else if( operator_ == CONDITION_OPERATOR_LESSEQUAL)
		return lv <= rv;
	else if( operator_ == CONDITION_OPERATOR_EQUAL)
		return lv == rv;
	else if( operator_ == CONDITION_OPERATOR_NOTEQUAL)
		return lv != rv;

	ACE_ASSERT( false && "ConditionLeaf::condition_calcuate not support condition operator type");
	return false;
}
