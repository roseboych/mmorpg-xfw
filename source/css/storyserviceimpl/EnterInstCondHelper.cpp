/**
* css
*
* @category		enter inst
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "EnterInstCondHelper.h"

#include "EnterInstTask.h"

#include <reslib/reslib_def.h>
#include <reslib/world/StoryMapOption.h>
#include <reslib/world/InstMapOption.h>
#include <corelib/condition/ConditionContext.h>
#include <corelib/script/ScriptContext.h>
#include "../BaseStoryService.h"
#include "../character/Player.h"

EnterInstCondHelper::EnterInstCondHelper()
{
	//regist enterinst class
	conds_.push_back( FRAMEWK_NEW EnterInstTask());
}

EnterInstCondHelper::~EnterInstCondHelper()
{
	ENTERINSTCOND_VECTOR::iterator eiter =conds_.end();
	for( ENTERINSTCOND_VECTOR::iterator iter =conds_.begin(); iter != eiter; ++iter)
		delete (*iter);
	conds_.clear();
}

EnterInstConditionBase* EnterInstCondHelper::get_enterinstimpl( NS_STL::string clsname)
{
	for( size_t i =0; i < conds_.size(); ++i)
	{
		if( conds_[i]->get_classname() == clsname)
			return conds_[i];
	}

	return 0;
}

S_BOOL EnterInstCondHelper::can_joininstance( BaseStoryService* psvr, StoryMapOption* instopt, Player* curplayer)
{
	if( !instopt->instmap_opt_.is_check_enterinst_)
		return true;

	ACE_ASSERT( instopt->instmap_opt_.enterinst_fun_ != FUNSUPPORT_NONE);

	if( instopt->instmap_opt_.enterinst_fun_ == FUNSUPPORT_CLASSIMPL)
	{
		EnterInstConditionBase* pcond =get_enterinstimpl( instopt->instmap_opt_.enterinst_class_);
		ACE_ASSERT( pcond != 0);

		return pcond->join_instance( instopt->instmap_opt_.enterinst_param_, curplayer);
	}
	else if( instopt->instmap_opt_.enterinst_fun_ == FUNSUPPORT_CONDITION)
	{
		ConditionContext context;
		//×¢²á»·¾³
		context.regist_context( "curplayer", curplayer);

		return instopt->instmap_opt_.enterinst_condition_->condition_calcuate( context);
	}
	else if( instopt->instmap_opt_.enterinst_fun_ == FUNSUPPORT_SCRIPT)
	{
		S_INT_32 bret =-1;
		app::script::ScriptContext& context =psvr->get_scriptcontext();
		try{
			bret =luabind::call_function<S_INT_32>( context.get_luastate(), "inst_entercheck", instopt->get_mapid(), curplayer);
		}
		catch( ...){
			lua_pop( context.get_luastate(), 1);
			bret =-1;
		}

		return bret == 0;
	}

	return false;
}
