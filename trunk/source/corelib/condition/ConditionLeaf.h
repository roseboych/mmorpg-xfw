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
* @brief �����ж�Ҷ�ӽڵ�
**/
class ConditionLeaf : public ConditionElement
{
public:
	ConditionLeaf();
	virtual ~ConditionLeaf();

	virtual S_BOOL is_leaf(){ return true;}
	virtual S_BOOL condition_calcuate( ConditionContext& context);

protected:
	//�����
	CONDITION_OPERATOR	operator_;
	//���������ֵ x_ֻ��between��ʹ��
	ConditionValue		*left_, *right_, *x_;
};

#endif	//__CONDITIONLEAF__H__
