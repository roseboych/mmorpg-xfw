/**
* reslib
*
* @category		副本配置
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTPARAMBASE__H__
#define __ENTERINSTPARAMBASE__H__

#include <corelib/corelibdef.h>
#include <string>

class TiXmlElement;

#define ENTERINSTCHECK_DEFINE( classname)							\
public:																\
	static const NS_STL::string& get_classname(){					\
		static NS_STL::string s_##classname_name =#classname;		\
		return s_##classname_name;									\
	}

/**
* @class EnterInstParamBase
* 
* @brief 进入副本地图判定条件配置基类
* 
**/
class EnterInstParamBase
{
public:
	static EnterInstParamBase* create( NS_STL::string clsname);

public:
	/**
	* 加载配置信息
	* @param ptr
	* @return
	**/
	virtual bool load_params( TiXmlElement* ptr) =0;
};

#endif	//__ENTERINSTPARAMBASE__H__
