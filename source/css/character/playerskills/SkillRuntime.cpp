/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "SkillRuntime.h"

#include <ace/OS_NS_sys_time.h>
#include <ace/Assert.h>

#include <corelib/script/ScriptContext.h>
#include "../Player.h"
#include "../../BaseStoryService.h"

SkillRuntime::SkillRuntime( SkillDesc* sk):
skill_desc_( sk),
runtime_data_( 0)
{

}

SkillRuntime::~SkillRuntime()
{
	if( runtime_data_)
		delete runtime_data_;
	runtime_data_ =0;
}

void SkillRuntime::build_runtimepkg()
{
	ACE_ASSERT( skill_desc_ != 0 && runtime_data_ == 0);

	runtime_data_ =SkillDataPackageBase::get_skilldatapackage( skill_desc_->data_package_);
	ACE_ASSERT( runtime_data_ != 0);

	runtime_data_->init_skillruntime();
	
	runtime_data_->set_skillid( skill_desc_->skillid_);
	//TODO:需要处理地图切换时回复迁移钱的时间数据
	runtime_data_->set_lastruntime( 0);
}

SerializeObject* SkillRuntime::clone_runtimedata()
{
	return ( runtime_data_ == 0 ? 0 : runtime_data_->clone_runtimedata());
}

bool SkillRuntime::is_cooldown()
{
	ACE_UINT64 tnow =ACE_OS::gettimeofday().get_msec();

	return (runtime_data_->get_lastruntime() + skill_desc_->cooltime_*1000) <= tnow;
}

void SkillRuntime::update_lastruntime()
{
	ACE_UINT64 tnow =ACE_OS::gettimeofday().get_msec();
	runtime_data_->set_lastruntime( tnow);
}

bool SkillRuntime::process( const ACE_UINT64& tnow, Player* user)
{
	char ret =0;
	app::script::ScriptContext& context =CONTENTSERVICE_INS->get_scriptcontext();
	try{
		ret =luabind::call_function<char>( context.get_luastate(), "skill_process", skill_desc_->skillidsn_.c_str(), 
			user, runtime_data_);
	}
	catch( ...){
		lua_pop( context.get_luastate(), 1);
		ret =-1;
	}

	return ret == 1;
}
