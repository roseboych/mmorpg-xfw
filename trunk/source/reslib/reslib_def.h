/**
* reslib application
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __RESLIB_DEF__H__
#define __RESLIB_DEF__H__

#include <prolib/core_type.h>

#include <corelib/memory/CoreMemory.h>

//功能支持
enum FUNCTION_SUPPORT
{
	//none
	FUNSUPPORT_NONE =0,
	//c++实现
	FUNSUPPORT_CLASSIMPL,
	//条件判定实现
	FUNSUPPORT_CONDITION,
	//脚本支持
	FUNSUPPORT_SCRIPT,
};

#endif	//__RESLIB_DEF__H__
