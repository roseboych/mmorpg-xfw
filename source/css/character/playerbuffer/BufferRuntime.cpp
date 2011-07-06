/**
* css application
*
* @category		buffer system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "BufferRuntime.h"

#include <ace/OS_NS_time.h>
#include <ace/OS_NS_sys_time.h>
#include "../Player.h"
#include "MyBufferContainer.h"

BufferRuntime::BufferRuntime():
buff_conf_( 0),
owner_container_( 0)
{
	last_ticktime_ =ACE_OS::gettimeofday().get_msec();
}

BufferRuntime::BufferRuntime( const PRO::bufferdata_item& buf, BufferDesc* conf, MyBufferContainer* powner):
buffer_data_( buf),
buff_conf_( conf),
owner_container_( powner)
{
	//reset buffer type
	buffer_data_.types_ =buff_conf_->is_sysbuffer()?0:1;
	last_ticktime_ =ACE_OS::gettimeofday().get_msec();
}

BufferRuntime::~BufferRuntime()
{
}

void BufferRuntime::tick( const ACE_UINT64& tnow, app::script::ScriptContext& context)
{
	if( this->last_ticktime_ + buff_conf_->tickstep_ > tnow)
		return;

	if( buff_conf_->is_onlinescope())
	{
		//按照在线时间计算
		buffer_data_.scopetime_ += ( tnow - this->last_ticktime_)/1000;
	}
	else
	{
		//按照绝对时间计算
		buffer_data_.scopetime_ =ACE_OS::time();
	}

	this->last_ticktime_ =tnow;

	//执行脚本tick函数
	if( buff_conf_->is_supportscript() && buff_conf_->scripttick_)
	{
		try{
			luabind::call_function<void>( context.get_luastate(), "buff_tick", buff_conf_->script_file_.c_str(), owner_container_->get_owner(), this);
		}
		catch( ...){
			lua_pop( context.get_luastate(), 1);
		}
	}
}

bool BufferRuntime::is_finish()
{
	if( buff_conf_->is_onlinescope())
	{
		//按照在线时间计算
		if( buffer_data_.scopetime_ >= buff_conf_->scopetime_)
			return true;
	}
	else
	{
		//按照绝对时间计算
		if( buffer_data_.scopetime_ >= (buff_conf_->scopetime_ + buffer_data_.createtime_))
			return true;
	}

	return false;
}

void BufferRuntime::script_regist( lua_State* l)
{
	using namespace luabind;

	module( l)
	[
		class_<BufferRuntime>( "BufferRuntime")
			.def(constructor<>())
	];
}
