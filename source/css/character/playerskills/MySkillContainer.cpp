/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "MySkillContainer.h"

#include <reslib/skills/SkillTreeConfig.h>
#include <prolib/datastore/SkillHelper.h>
#include <prolib/Pro_all.h>

#include "../Player.h"

MySkillContainer::MySkillContainer():
owner_player_( 0),
skill_tree_( 0),
cur_skill_( 0)
{
}

MySkillContainer::~MySkillContainer()
{
	reset();
}

void MySkillContainer::set_owner( Player* p)
{
	this->owner_player_ =p;
}

void MySkillContainer::reset()
{
	this->cur_skill_ =0;

	this->skill_tree_ =0;

	SKILLRUNTIME_MAP::iterator iter =runtimes_.begin(), eiter =runtimes_.end();
	for( ; iter != eiter; ++iter)
		delete iter->second;
	runtimes_.clear();
}

SkillRuntime* MySkillContainer::get_skillruntime( S_INT_32 skid)
{
	SKILLRUNTIME_MAP::iterator fiter =runtimes_.find( skid);
	if( fiter == runtimes_.end())
		return 0;

	return fiter->second;
}

void MySkillContainer::regist_finish()
{
	skill_tree_ =SKILLTREE_CFG->get_skilltree( owner_player_->get_race(), owner_player_->get_profession());
	ACE_ASSERT( skill_tree_ != 0);

	PRO::Pro_SkillInit_ntf* ntf =PROTOCOL_NEW PRO::Pro_SkillInit_ntf();
	PRO_UUID_FILL( ntf, owner_player_->global_index_, owner_player_->uuid_);
	ntf->chrid_ =this->owner_player_->chrid_;

	//构造所有支持的技能运行时数据
	NS_STL::list<SkillDesc*> sks;
	skill_tree_->get_allskills( sks);
	NS_STL::list<SkillDesc*>::iterator iter =sks.begin(), eiter =sks.end();
	for( ; iter != eiter; ++iter)
	{
		SkillDesc* pd =(*iter);
		if( !SkillHelper::is_skillexist( pd->skillid_, owner_player_->baseinfo_.skill_))
			continue;

		SkillRuntime* sr =FRAMEWK_NEW SkillRuntime( pd);
		runtimes_[pd->skillid_] =sr;
		sr->build_runtimepkg();

		ntf->add_skillinfo( sr->clone_runtimedata());
	}

	this->owner_player_->send_to_gts( ntf);
}

SkillDesc* MySkillContainer::get_skillfromtree( S_INT_32 skid)
{
	return skill_tree_->get_skilldesc( skid);
}

SkillRuntime* MySkillContainer::study_skill( SkillDesc* pdesc)
{
	SkillRuntime* sr =FRAMEWK_NEW SkillRuntime( pdesc);
	runtimes_[pdesc->skillid_] =sr;
	sr->build_runtimepkg();

	//设置技能标签
	SkillHelper::set_skillflag( pdesc->skillid_, owner_player_->baseinfo_.skill_, sizeof(owner_player_->baseinfo_.skill_)/sizeof( S_INT_32), true);
	
	return sr;
}

void MySkillContainer::tick( const ACE_UINT64& tnow)
{
	if( cur_skill_ == 0)
		return;

	if( cur_skill_->process( tnow, owner_player_))
	{
		//技能执行结束
		set_curskill( 0);
	}
}

void MySkillContainer::set_curskill( SkillRuntime* sr)
{
	this->cur_skill_ =sr;
}
