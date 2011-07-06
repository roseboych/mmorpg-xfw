/**
* corelib
*
* @category		script module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ScriptContext.h"

#include "corelib/corelibdef.h"
#include "corelib/log/logmacro.h"

#include <luabind/function.hpp>
#include <sstream>
#include <iosfwd>

NAMESPACE_APPSCRIPT_BEGIN

ScriptContext::ScriptContext():
lua_state_( 0)
{
}

ScriptContext::~ScriptContext()
{
}

#ifdef LUABIND_NO_EXCEPTIONS

void ScriptContext::error_callback_fun( lua_State* L)
{
	if( lua_isstring( L, -1))
	{
		NS_STL::string err =lua_tostring( L, -1);
		MODULE_LOG_ERROR( MODULE_SYSTEM, "ScriptContext error_callback_fun: ===> %s <===", err.c_str());
	}
}

void ScriptContext::cast_failed_callback_fun( lua_State* L, luabind::type_id const& id)
{
	if( lua_isstring( L, -1))
	{
		NS_STL::string err =lua_tostring( L, -1);
		MODULE_LOG_ERROR( MODULE_SYSTEM, "ScriptContext cast_failed_callback_fun: ===> %s <===", err.c_str());
	}
}

#endif

int ScriptContext::pcall_callback_fun( lua_State* L)
{
	lua_Debug d;
	lua_getstack( L, 1, &d);
	lua_getinfo(L, "Sln", &d);

	NS_STL::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	NS_STL::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if( d.name != 0)
	{
		msg << "(" << d.namewhat << " " << d.name << ")";
	}

	msg << " " << err;
	lua_pushstring(L, msg.str().c_str());

	return 1;
}

bool ScriptContext::init_scriptcontext()
{
	if( lua_state_)
		return true;

	lua_state_ = lua_open();
	if( lua_state_ == 0)
		return false;

#ifdef LUABIND_NO_EXCEPTIONS
	luabind::set_error_callback( ScriptContext::error_callback_fun);
	luabind::set_cast_failed_callback( ScriptContext::cast_failed_callback_fun);
#endif

	luaL_openlibs( lua_state_);
	luabind::open( lua_state_);

	if( !regist_bindclass())
		return false;

	return true;
}

void ScriptContext::uninit_scriptcontext()
{
	if( lua_state_)
		lua_close( lua_state_);
	lua_state_ =0;
}

bool ScriptContext::run_script( const char* cmd)
{
	int ret =LUA_ERRRUN;

	if( lua_state_ == 0)
		return false;

	ret =luaL_loadbuffer( lua_state_, cmd, strlen( cmd), NULL);
	if( ret != 0)
		return false;
		
	ret =luabind::detail::pcall( lua_state_, 0, LUA_MULTRET);

	return ret == 0;
}

bool ScriptContext::run_globalfunction( const char* fun)
{
	if( lua_state_ == 0 || fun == 0 || strlen( fun) == 0)
		return false;

	try{
		lua_getfield( lua_state_, LUA_GLOBALSINDEX, fun);
		if( lua_isfunction( lua_state_, -1) == false)
		{
			lua_pop( lua_state_, 1);
			return false;
		}

		lua_call( lua_state_, 0, 0);
	}
	catch( ...){
		return false;
	}

	return true;
}

void ScriptContext::remove_4_context( const char* id)
{
	if( lua_state_ == 0 || id == 0 || strlen( id) == 0)
		return;

	luabind::globals( lua_state_)[id] =luabind::nil;
}

void ScriptContext::do_gc()
{
	if( lua_state_ == 0)
		return;

	lua_gc( lua_state_, LUA_GCCOLLECT, 0);
}

int ScriptContext::get_luamemsize()
{
	if( lua_state_ == 0)
		return -1;

	return lua_gc( lua_state_, LUA_GCCOUNT, 0);
}

NAMESPACE_APPSCRIPT_END
