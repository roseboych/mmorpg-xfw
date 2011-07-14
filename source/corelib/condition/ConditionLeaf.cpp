/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionLeaf.h"

ConditionLeaf::ConditionLeaf():
left_( 0),
right_( 0),
x_( 0)
{
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
