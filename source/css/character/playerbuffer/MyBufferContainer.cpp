/**
* css application
*
* @category		buffer system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "MyBufferContainer.h"

#include <corelib/script/ScriptContext.h>
#include <prolib/Pro_all.h>
#include <ace/OS_NS_time.h>
#include <reslib/buffers/BufferConfig.h>

#include "../Player.h"
#include "../../BaseStoryService.h"
#include "../../CSSModule.h"

MyBufferContainer::MyBufferContainer()
{
}

MyBufferContainer::~MyBufferContainer()
{
	reset();
}

void MyBufferContainer::set_owner( Player* owner)
{
	this->owner_ =owner;
}

void MyBufferContainer::reset()
{
	buff_runtimes_.clear();
}

BufferRuntime* MyBufferContainer::get_bufferbyid( S_INT_32 buffcode)
{
	BUFFRUNTIME_MAP::iterator fiter =buff_runtimes_.find( buffcode);
	if( fiter == buff_runtimes_.end())
		return 0;

	return &(fiter->second);
}

void MyBufferContainer::init_baseinfo( PRO::Pro_ChrLoad_ack* pack)
{
	reset();

	for( PRO::Pro_ChrLoad_ack::BUFFERDATA_LIST::iterator iter =pack->buffers_.begin(); iter != pack->buffers_.end(); ++iter)
	{
		PRO::bufferdata_item& buf =(*iter);
		
		BufferDesc* pconf =BUFFER_CFG->get_bufferbycode( buf.bufferid_);
		ACE_ASSERT( pconf != 0);
		if( pconf == 0)
			continue;

		NS_STL::pair< BUFFRUNTIME_MAP::iterator, bool> mp1 =
			buff_runtimes_.insert( NS_STL::make_pair( buf.bufferid_, BufferRuntime( buf, pconf, this)));

		ACE_ASSERT( mp1.second);
	}
}

void MyBufferContainer::tick( const ACE_UINT64& tnow)
{
	//是否需要重新计算player属性
	bool brest =false;

	if( buff_runtimes_.size() > 0)
	{
		app::script::ScriptContext& scriptc =CONTENTSERVICE_INS->get_scriptcontext();

		for( BUFFRUNTIME_MAP::iterator iter =buff_runtimes_.begin(); iter != buff_runtimes_.end();)
		{
			BufferRuntime& buff =iter->second;
			buff.tick( tnow, scriptc);

			if( buff.is_finish())
			{
				if( buff.will_affectbaseprop())
					brest =true;

				//notify buffer finish
				notify_bufferchange( buff, 1);

				BUFFRUNTIME_MAP::iterator deliter =iter;
				++iter;
				buff_runtimes_.erase( deliter);
			}
			else
				++iter;
		}
	}

	if( brest)
		owner_->calcuate_playerproperty();
}

BufferRuntime* MyBufferContainer::attach_newbuffer( S_INT_32 buffcode)
{
	BufferDesc* pconf =BUFFER_CFG->get_bufferbycode( buffcode);
	if( pconf == 0)
		return 0;

	PRO::bufferdata_item buff;
	buff.bufferid_ =buffcode;
	buff.types_ =pconf->is_sysbuffer()?0:1;
	buff.createtime_ =(S_INT_32)ACE_OS::time();
	if( pconf->is_onlinescope())
		buff.scopetime_ =0;
	else
		buff.scopetime_ =buff.createtime_;

	BufferRuntime* prun =get_bufferbyid( buffcode);
	if( prun)
	{
		prun->set_bufferdata( buff);

		//通知buffer发生变化，替换旧的buffer
		notify_bufferchange( *prun, 2);
	}
	else
	{
		NS_STL::pair< BUFFRUNTIME_MAP::iterator, bool> mp1 =
			buff_runtimes_.insert( NS_STL::make_pair( buff.bufferid_, BufferRuntime( buff, pconf, this)));

		ACE_ASSERT( mp1.second);

		prun =&(mp1.first->second);

		//通知增加了一个新的buffer
		notify_bufferchange( *prun, 0);
	}

	return prun;
}

void MyBufferContainer::cancel_onebuffer( S_INT_32 buffcode)
{
	BufferRuntime* buff =get_bufferbyid( buffcode);
	if( buff == 0)
		return;

	bool bprop =buff->will_affectbaseprop();

	//通知buffer被取消了
	notify_bufferchange( *buff, 1);

	buff_runtimes_.erase( buffcode);

	if( bprop)
		owner_->calcuate_playerproperty();
}

void MyBufferContainer::notify_bufferchange( BufferRuntime& buff, S_INT_8 type)
{
	USE_PROTOCOL_NAMESPACE;

	Pro_BuffChange_ntf* ntf =PROTOCOL_NEW Pro_BuffChange_ntf();
	ntf->chrid_ =owner_->chrid_;
	ntf->ntf_type_ =type;
	ntf->change_buff_ =buff.get_bufferdata();

	if( buff.notify_others())
	{
		CONTENTSERVICE_INS->get_scenemanage()->broadcast_inview( owner_, ntf->clone(), true);
	}
	else
	{
		PRO_UUID_FILL( ntf, owner_->global_index_, owner_->uuid_);
		owner_->send_to_gts( ntf->clone());
	}

	//通知dpx
	PRO_UUID_FILL( ntf, owner_->global_index_, owner_->uuid_);
	CSSMODULE->send_to_dpx( ntf);
}

void MyBufferContainer::affect_playerprops( app::script::ScriptContext& context)
{
	for( BUFFRUNTIME_MAP::iterator iter =buff_runtimes_.begin(); iter != buff_runtimes_.end(); ++iter)
	{
		BufferRuntime& buff =iter->second;
		if( !buff.is_supportscript() || !buff.will_affectbaseprop())
			continue;

		try{
			luabind::call_function<void>( context.get_luastate(), "buff_initplayerprop", buff.get_scriptinsname(), owner_);
		}
		catch( ...){
			lua_pop( context.get_luastate(), 1);
		}
	}
}
