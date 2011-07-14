/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __CONDITIONELEMENT__H__
#define __CONDITIONELEMENT__H__

#include "corelib/corelibdef.h"

class ConditionContext;

//��������������Ϸ�ʽ
enum CONDITION_CONNECTOR
{
	CONDITION_CONNECTOR_AND =0,
	CONDITION_CONNECTOR_OR,
	CONDITION_CONNECTOR_NOT,
};

//������֧�ֵ������
enum CONDITION_OPERATOR
{
	// >
	CONDITION_OPERATOR_GREAT =0,
	// >=
	CONDITION_OPERATOR_GREATEQUAL,
	// ==
	CONDITION_OPERATOR_EQUAL,
	// <
	CONDITION_OPERATOR_LESS,
	// <=
	CONDITION_OPERATOR_LESSEQUAL,
	// !=
	CONDITION_OPERATOR_NOTEQUAL,
	// > x <
	CONDITION_OPERATOR_BETWEEN,
};

/**
* @class ConditionElement
* 
* @brief �����ж��ڵ����
**/
class ConditionElement
{
public:
	ConditionElement();

	/**
	* �Ƿ���Ҷ�ӽڵ㣬ֻ��Ҷ�ӽڵ�����߼�����
	* @return
	**/
	virtual S_BOOL is_leaf() =0;

	/**
	* �������������趨�������߼�
	* @param context
	* @return
	**/
	virtual S_BOOL condition_calcuate( ConditionContext& context) =0;
};

#endif	//__CONDITIONELEMENT__H__
