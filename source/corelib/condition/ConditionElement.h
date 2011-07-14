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

//定义了条件的组合方式
enum CONDITION_CONNECTOR
{
	CONDITION_CONNECTOR_AND =0,
	CONDITION_CONNECTOR_OR,
	CONDITION_CONNECTOR_NOT,
};

//定义了支持的运算符
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
* @brief 条件判定节点基类
**/
class ConditionElement
{
public:
	ConditionElement();

	/**
	* 是否是叶子节点，只有叶子节点进行逻辑运算
	* @return
	**/
	virtual S_BOOL is_leaf() =0;

	/**
	* 根据条件计算设定的条件逻辑
	* @param context
	* @return
	**/
	virtual S_BOOL condition_calcuate( ConditionContext& context) =0;
};

#endif	//__CONDITIONELEMENT__H__
