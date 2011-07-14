/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __CONDITIONNODE__H__
#define __CONDITIONNODE__H__

#include "corelib/corelibdef.h"
#include "corelib/condition/ConditionElement.h"

/**
* @class ConditionNode
* 
* @brief 条件判定子节点
**/
class ConditionNode : public ConditionElement
{
public:
	ConditionNode( ConditionElement* l, ConditionElement* r =0, CONDITION_CONNECTOR connector =CONDITION_CONNECTOR_AND);

	virtual S_BOOL is_leaf(){ return false;}

	virtual S_BOOL condition_calcuate( ConditionContext& context);

	ConditionElement* get_left(){ return left_;}
	ConditionElement* get_right(){ return right_;}

protected:
	//条件组合方式
	CONDITION_CONNECTOR	connector_;
	//条件
	ConditionElement	*left_, *right_;
};

#endif	//__CONDITIONNODE__H__
