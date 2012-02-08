/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __CONDITIONLEAF__H__
#define __CONDITIONLEAF__H__

#include "corelib/corelibdef.h"
#include "corelib/condition/ConditionElement.h"
#include "corelib/condition/ConditionValue.h"

/**
* @class ConditionLeaf
* 
* @brief 条件判定叶子节点
**/
class ConditionLeaf : public ConditionElement
{
public:
	ConditionLeaf();
	ConditionLeaf( TiXmlElement* e);
	virtual ~ConditionLeaf();

	virtual bool is_leaf(){ return true;}
	virtual bool condition_calcuate( ConditionContext& context);

protected:
	//运算符
	CONDITION_OPERATOR	operator_;
	//运算符左右值 x_只在between中使用
	ConditionValue		*left_, *right_, *x_;
};

#endif	//__CONDITIONLEAF__H__
