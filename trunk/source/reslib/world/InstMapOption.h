/**
* reslib
*
* @category		副本配置
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSTMAPOPTION__H__
#define __INSTMAPOPTION__H__

#include <corelib/corelibdef.h>
#include <corelib/condition/ConditionElement.h>
#include "reslib/reslib_def.h"

#include <string>

class TiXmlElement;
class EnterInstParamBase;
class StoryMapOption;

/**
* @class InstMapOption
* 
* @brief 副本地图相关配置
* 
**/
class InstMapOption
{
public:
	InstMapOption();

	/**
	* 加载和副本相关的配置选项
	* @param e
	**/
	bool	load_instmapopt( TiXmlElement* e, StoryMapOption* storyopt);

protected:
	/*
	* 加载进入副本判定
	* @param e
	* @return
	*/
	bool	load_enterinstopt( TiXmlElement* e);

public:
	//是否检查进入副本条件
	bool				is_check_enterinst_;
	//进入副本支持
	FUNCTION_SUPPORT	enterinst_fun_;
	//c++参数
	EnterInstParamBase*	enterinst_param_;
	NS_STL::string		enterinst_class_;
	//condition参数
	ConditionElement*	enterinst_condition_;

	//脚本信息
	NS_STL::string		inst_script_;

	//副本状态控制实现
	NS_STL::string		ctrl_classname_;
};

#endif	//__INSTMAPOPTION__H__
