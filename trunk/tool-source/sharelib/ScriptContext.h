/**
* corelib
*
* @category		script module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ) ( lujf1978@163.com ), 2009
*/
#ifndef __SCRIPTCONTEXT__H__
#define __SCRIPTCONTEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>

/**
* @class	ScriptContext
* @author	Created by lujianfeng, 2009
* 
* @brief 脚本环境类
**/
class ScriptContext
{
public:
	ScriptContext(void);
	virtual ~ScriptContext(void);

public:
	/**
	* 初始化脚本环境，必须在系统初始化时调用
	* @return bool
	**/
	bool init_scriptcontext();

	/**
	* 卸载脚本环境
	**/
	void uninit_scriptcontext();

	/**
	* 获取lua运行环境
	* @return lua_State*
	**/
	lua_State* get_luastate(){ return lua_state_;}

	/**
	* 执行指定的脚本
	* 
	* @param cmd	const char*	需要执行的脚本文档
	* @return int 0:脚本错误 >0:成功
	**/
	int run_script( const char* cmd);

	/**
	* 注册变量到脚本环境
	* @param id
	* @param p
	**/
	template<class X>
	bool regist_2_context( const char* id, X *p);
	void remove_4_context( const char* id);

protected:
	/**
	* 注册所有需要绑定到lua的class。
	* 该函数必须在具体的应用中被实现。
	* 
	* @return bool
	**/
	static bool regist_bindclass();

private:
	//lua环境指针
	lua_State*	lua_state_;
};

template<class X>
bool ScriptContext::regist_2_context( const char* id, X *p)
{
	if( lua_state_ == 0 || id == 0 || strlen( id) == 0)
		return false;

	try{
		luabind::globals( lua_state_)[id] =p;
	}
	catch(...){
		return false;
	}

	return true;
}

#endif	//__SCRIPTCONTEXT__H__
