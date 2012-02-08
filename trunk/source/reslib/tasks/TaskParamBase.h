/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKPARAMBASE__H__
#define __TASKPARAMBASE__H__

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>

#include <string>

class TiXmlElement;

#define TASKIMPLPARAM_DEFINE( classname)							\
public:																\
	static const NS_STL::string& get_classname(){					\
		static NS_STL::string s_##classname_name =#classname;		\
		return s_##classname_name;									\
	}

/**
* @class TaskParamBase
* 
* @brief 任务实现参数类基类
**/
class TaskParamBase
{
public:
	static TaskParamBase* create( NS_STL::string clsname);

public:
	/**
	* 加载配置信息
	* @param ptr
	* @return
	**/
	virtual bool load_params( TiXmlElement* ptr) =0;
};

#endif	//__TASKPARAMBASE__H__
