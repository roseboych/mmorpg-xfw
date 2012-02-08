/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __CONDITIONCONTEXT__H__
#define __CONDITIONCONTEXT__H__

#include "corelib/corelibdef.h"
#include "corelib/condition/IPropertyOperator.h"

#include <map>
#include <string>

/**
* @class ConditionContext
* 
* @brief 条件判定用到的环境类
**/
class ConditionContext
{
public:
	typedef NS_STL::map< NS_STL::string, IPropertyOperator*>	CONTEXT_MAP;

public:
	ConditionContext(){}

	void reset(){ context_.clear();}

	void regist_context( const char* objname, IPropertyOperator* obj){
		ACE_ASSERT( objname !=0 && obj != 0);
		context_[objname] =obj;
	}

	IPropertyOperator* get_object( const char* objname){
		CONTEXT_MAP::iterator fiter =context_.find( objname);
		if( fiter == context_.end())
			return 0;
		return fiter->second;
	}

protected:
	//支持的对象列表 obj.property
	CONTEXT_MAP	context_;
};

#endif	//__CONDITIONCONTEXT__H__
