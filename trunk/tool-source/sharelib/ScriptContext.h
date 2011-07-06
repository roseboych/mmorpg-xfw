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
* @brief �ű�������
**/
class ScriptContext
{
public:
	ScriptContext(void);
	virtual ~ScriptContext(void);

public:
	/**
	* ��ʼ���ű�������������ϵͳ��ʼ��ʱ����
	* @return bool
	**/
	bool init_scriptcontext();

	/**
	* ж�ؽű�����
	**/
	void uninit_scriptcontext();

	/**
	* ��ȡlua���л���
	* @return lua_State*
	**/
	lua_State* get_luastate(){ return lua_state_;}

	/**
	* ִ��ָ���Ľű�
	* 
	* @param cmd	const char*	��Ҫִ�еĽű��ĵ�
	* @return int 0:�ű����� >0:�ɹ�
	**/
	int run_script( const char* cmd);

	/**
	* ע��������ű�����
	* @param id
	* @param p
	**/
	template<class X>
	bool regist_2_context( const char* id, X *p);
	void remove_4_context( const char* id);

protected:
	/**
	* ע��������Ҫ�󶨵�lua��class��
	* �ú��������ھ����Ӧ���б�ʵ�֡�
	* 
	* @return bool
	**/
	static bool regist_bindclass();

private:
	//lua����ָ��
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
