/**
* corelib
*
* @category		script module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SCRIPTCONTEXT__H__
#define __SCRIPTCONTEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <luabind/luabind.hpp>

#include "corelib/corelibdef.h"
#include <prolib/core_type.h>
#include <string>

NAMESPACE_APPSCRIPT_BEGIN

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
	* @return bool
	**/
	bool run_script( const char* cmd);

	/**
	* 执行全局函数
	* 参数为空,无返回值
	* @param fun	函数名称
	* @return
	**/
	bool run_globalfunction( const char* fun);

	/**
	* 注册变量到脚本环境
	* @param id
	* @param p
	**/
	template<class X>
	bool regist_2_context( const char* id, X *p);
	void remove_4_context( const char* id);

#ifdef LUABIND_NO_EXCEPTIONS
	static void error_callback_fun( lua_State* L);
	static void cast_failed_callback_fun( lua_State* L, luabind::type_id const& id);
#endif
	static int pcall_callback_fun( lua_State* L);

public:
	//-----------------------------debug function---------------------------------
	/**
	* gc回收调用
	**/
	void do_gc();

	/**
	* 获取当前的Lua使用的内存大小
	**/
	int get_luamemsize();

protected:
	/**
	* 注册所有需要绑定到lua的class。
	* 该函数必须在具体的应用中被实现。
	* 
	* @return bool
	**/
	bool regist_bindclass();

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

/**
* @class	ScriptGlobalParamGuard
* @author	Created by lujianfeng, 2010
* 
* @brief 脚本环境全局变量注册和注销帮助
**/
template<class T>
class ScriptGlobalParamGuard
{
public:
	ScriptGlobalParamGuard( ScriptContext* pcontext, const char* id, T* val);
	~ScriptGlobalParamGuard();

private:
	ScriptContext*	script_;
	const char*	iid_;
};

template<class T>
ScriptGlobalParamGuard<T>::ScriptGlobalParamGuard( ScriptContext* pcontext, const char* id, T* val):
script_( pcontext),
iid_( id)
{
	script_->regist_2_context<T>( iid_, val);
}

template<class T>
ScriptGlobalParamGuard<T>::~ScriptGlobalParamGuard()
{
	script_->remove_4_context( iid_);
}


NAMESPACE_APPSCRIPT_END

#endif	//__SCRIPTCONTEXT__H__
