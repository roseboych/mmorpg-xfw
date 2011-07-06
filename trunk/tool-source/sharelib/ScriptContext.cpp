#include "ScriptContext.h"

#include <luabind/function.hpp>

#define LUABIND_NO_HEADERS_ONLY

ScriptContext::ScriptContext():
lua_state_( 0)
{
}

ScriptContext::~ScriptContext()
{
}

bool ScriptContext::init_scriptcontext()
{
	if( lua_state_)
		return true;

	lua_state_ = lua_open();
	if( lua_state_ == 0)
		return false;

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

int ScriptContext::run_script( const char* cmd)
{
	if( lua_state_ == 0)
		return 0;

	if( luaL_loadbuffer( lua_state_, cmd, strlen( cmd), NULL) == 0)
		return lua_pcall( lua_state_, 0, LUA_MULTRET, 0);

	return 0;
}

void ScriptContext::remove_4_context( const char* id)
{
	if( lua_state_ == 0 || id == 0 || strlen( id) == 0)
		return;

	luabind::globals( lua_state_)[id] =luabind::nil;
}

bool ScriptContext::regist_bindclass()
{
	return true;
}