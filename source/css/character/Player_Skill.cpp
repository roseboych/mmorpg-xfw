/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"
#include <prolib/Pro_all.h>
#include <corelib/script/ScriptContext.h>
#include "../BaseStoryService.h"
#include "../CSSModule.h"

void Player::event_unitentity_reset( UnitBase* pobj)
{
	if( pobj == fight_target_obj_)
		set_fighttargetobj( 0);
}

void Player::set_fighttargetobj( UnitBase* obj)
{
	if( fight_target_obj_)
		fight_target_obj_->unregist_eventlistener( this);

	fight_target_obj_ =obj;

	if( fight_target_obj_)
		fight_target_obj_->regist_eventlistener( this);
}

bool Player::can_beattacked( UnitBase* pattack)
{
	switch( pattack->get_actortype())
	{
	case ACTOR_TYPE_PLAYER:
		{
			//不同种族的可以开战
			Player* patt =dynamic_cast<Player*>( pattack);
			//TODO:测试用
			//return this->baseinfo_.race_ != patt->baseinfo_.race_;
			return true;
		}

		break;

	case ACTOR_TYPE_MONSTER:
		return true;

	default:
		return false;
	}
}

void Player::skill_study( int skid)
{
	USE_PROTOCOL_NAMESPACE;

	S_INT_8 ret =0;

	SkillDesc* pdesc =0;
	if( ( pdesc =myskills_.get_skillfromtree( skid)) == 0)
		ret =1;
	else if( myskills_.get_skillruntime( skid) != 0)
		ret =2;
	else
	{
		//学习技能
		app::script::ScriptContext& context =CONTENTSERVICE_INS->get_scriptcontext();
		try{
			ret =luabind::call_function<char>( context.get_luastate(), "skill_study", pdesc->skillidsn_.c_str(), this);
		}
		catch( ...){
			lua_pop( context.get_luastate(), 1);
			ret =3;
		}
	}

	SkillRuntime* sr =0;
	if( ret == 0)
	{
		//学习成功
		sr =myskills_.study_skill( pdesc);

		//通知dpx保存数据
		Pro_DBPlayerBaseInfo_sav* sav =PROTOCOL_NEW Pro_DBPlayerBaseInfo_sav();
		PRO_UUID_FILL( sav, this->global_index_, this->uuid_);
		sav->chrid_ =this->chrid_;
		sav->baseinfo_ =this->baseinfo_;
		CSSMODULE->send_to_dpx( sav);
	}

	Pro_SkillStudy_ack* ack =PROTOCOL_NEW Pro_SkillStudy_ack();
	PRO_UUID_FILL( ack, this->global_index_, this->uuid_);
	ack->skillid_ =skid;
	ack->ret_ =ret;
	ack->skillobj_ =sr == 0?0:sr->clone_runtimedata();

	this->send_to_gts( ack);
}

void Player::skill_used( int skid, S_INT_64 targetid, CHRSTATE_TYPE_ENUM st)
{
	USE_PROTOCOL_NAMESPACE;

	S_INT_8 ret =0;
	//1.检查当前状态
	if( cur_state_->get_statetype() != st)
	{
		if( action_selector_.goto_state( st) == 0)
			ret =4;
	}

	//2.检查技能是否合法
	SkillRuntime* sr =0;
	SkillDesc* pdesc =0;
	if( ret == 0)
	{
		sr =myskills_.get_skillruntime( skid);
		//技能是否可用
		if( sr == 0)
			ret =1;
		else
		{
			pdesc =sr->get_skilldesc();
			if( !pdesc->driving_)
			{
				//是否是主动技能
				ret =6;
			}
			else if( !cur_state_->is_skillinstate( skid))
			{
				//技能是否存在于当前状态的技能列表中
				ret =5;
			}
			else
			{
				//检查cooldown时间
				if( !sr->is_cooldown())
					ret =2;
			}
		}
	}

	//3.检查目标是否有效
	if( ret == 0 && pdesc->singleattack_)
	{
		if( this->fight_target_obj_ == 0 || fight_target_obj_->get_uuid() != targetid)
		{
			UnitBase* pTarget =CONTENTSERVICE_INS->get_onlineentity( targetid);
			if( pTarget == 0 || pTarget == this)
			{
				ret =3;
			}
			else
			{
				//目标是否能被攻击
				if( !pTarget->can_beattacked( this))
				{
					ret =3;
				}
				else
				{
					//设置战斗对象
					this->set_fighttargetobj( pTarget);
				}
			}
		}
	}

	//4.执行脚本
	if( ret == 0)
	{
		app::script::ScriptContext& context =CONTENTSERVICE_INS->get_scriptcontext();
		try{
			ret =luabind::call_function<char>( context.get_luastate(), "skill_use", pdesc->skillidsn_.c_str(), this, sr->get_runtimedata());
		}
		catch( ...){
			lua_pop( context.get_luastate(), 1);
			ret =-1;
		}
	}

	if( ret == 0)
	{
		//是否是瞬发技能
		if( !sr->is_immediate())
			myskills_.set_curskill( sr);

		//设置最后执行时间
		sr->update_lastruntime();

		//通知其他人触发技能
	}

	Pro_SkillUsed_ack* ack =PROTOCOL_NEW Pro_SkillUsed_ack();
	PRO_UUID_FILL( ack, this->global_index_, this->uuid_);
	ack->skillid_ =skid;
	ack->curstate_ =cur_state_->get_statetype();
	ack->target_unit_ =( fight_target_obj_?fight_target_obj_->get_uuid():NO_INITVALUE);
	ack->ret_ =ret;
	if( ret == 0)
		ack->skillobj_ =sr->clone_runtimedata();

	this->send_to_gts( ack);
}
