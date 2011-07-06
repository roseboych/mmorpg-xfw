/**
* css application
*
* @category		buffer system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BUFFERRUNTIME__H__
#define __BUFFERRUNTIME__H__

#include "../../css_pre.h"

#include <prolib/datastore/buffer_define.h>
#include <corelib/script/ScriptContext.h>
#include <reslib/buffers/BufferDesc.h>

class MyBufferContainer;

/**
* @class BufferRuntime
* 
* @brief buffer运行时
**/
class BufferRuntime
{
public:
	BufferRuntime();
	BufferRuntime( const PRO::bufferdata_item& buf, BufferDesc* conf, MyBufferContainer* powner);
	~BufferRuntime();

	S_INT_32 get_bufferid(){ return buff_conf_->buffer_id_;}

	/**
	* 获取buffer创建时间
	* @return
	**/
	S_INT_32 get_createtime(){ buffer_data_.createtime_;}

	S_INT_32 get_lasttime(){ buffer_data_.scopetime_;}
	void set_lasttime( S_INT_32 t){ buffer_data_.scopetime_ =t;}

	bool is_supportscript(){ return buff_conf_->is_supportscript();}
	bool will_affectbaseprop(){ return buff_conf_->will_affectbaseprop();}

	bool notify_others(){ return buff_conf_->notifyother_;}

	const char* get_scriptinsname(){ return buff_conf_->script_file_.c_str();}

	PRO::bufferdata_item& get_bufferdata(){ return buffer_data_;}
	void set_bufferdata( PRO::bufferdata_item& d){ buffer_data_ =d;}

	void tick( const ACE_UINT64& tnow, app::script::ScriptContext& context);

	bool is_finish();

	//---------------------------------------script function wrapper---------------------------------------
	static void script_regist( lua_State* l);

private:
	//buffer数据
	PRO::bufferdata_item	buffer_data_;

	//buffer配置信息
	BufferDesc*	buff_conf_;

	//上一次tick时间
	ACE_UINT64	last_ticktime_;

	MyBufferContainer*	owner_container_;
};

#endif	//__BUFFERRUNTIME__H__
