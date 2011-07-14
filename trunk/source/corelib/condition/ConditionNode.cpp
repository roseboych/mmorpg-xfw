/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionNode.h"

#include <ace/Assert.h>

ConditionNode::ConditionNode( ConditionElement* l, ConditionElement* r, CONDITION_CONNECTOR connector):
left_( l),
right_( r),
connector_( connector)
{
	ACE_ASSERT( left_ != 0);

	if( connector_ != CONDITION_CONNECTOR_NOT)
		ACE_ASSERT( right_ != 0);
}

S_BOOL ConditionNode::condition_calcuate( ConditionContext& context)
{
	S_BOOL lret =true, rret =true;
	if( left_)
		lret =left_->condition_calcuate( context);
	if( right_)
		rret =right_->condition_calcuate( context);

	if( connector_ == CONDITION_CONNECTOR_NOT)
		return !lret;
	else if( connector_ == CONDITION_CONNECTOR_AND)
		return lret & rret;
	else if( connector_ == CONDITION_CONNECTOR_OR)
		return lret || rret;
	else
	{
		ACE_ASSERT( false && "ConditionNode::condition_calcuate Not support condition connector type");
		return false;
	}
}
