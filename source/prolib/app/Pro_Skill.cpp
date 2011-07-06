/**
* protocol module
*
* 定义了技能使用协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/app/Pro_Skill.h"

#include <algorithm>

PROTOCOL_NAMESPACE_BEGIN

//-----------------------------Pro_SkillInit_ntf--------------------//
Pro_SkillInit_ntf::Pro_SkillInit_ntf()
:inherit( APP_SKILLINIT_NTF)
{
}

Pro_SkillInit_ntf::~Pro_SkillInit_ntf()
{
	SKILLPKGDATA_LIST::iterator iter =pkgs_.begin(), eiter =pkgs_.end();
	for( ; iter != eiter; ++iter)
		delete (*iter);
	pkgs_.clear();
}

//-----------------------------Pro_SkillStateChg_ntf--------------------//
Pro_SkillStateChg_ntf::Pro_SkillStateChg_ntf()
:inherit( APP_SKILLSTATECHG_NTF)
{
}

Pro_SkillStateChg_ntf::~Pro_SkillStateChg_ntf()
{
	SKILLPKGDATA_LIST::iterator iter =pkgs_.begin(), eiter =pkgs_.end();
	for( ; iter != eiter; ++iter)
		delete (*iter);
	pkgs_.clear();
}

//-----------------------------Pro_SkillStudy_req--------------------//
Pro_SkillStudy_req::Pro_SkillStudy_req()
:inherit( APP_SKILLSTUDY_REQ)
{
}

//-----------------------------Pro_SkillStudy_ack--------------------//
Pro_SkillStudy_ack::Pro_SkillStudy_ack()
:inherit( APP_SKILLSTUDY_ACK),
skillobj_( 0)
{
}

Pro_SkillStudy_ack::~Pro_SkillStudy_ack()
{
	if( skillobj_)
		delete skillobj_;
	skillobj_ =0;
}

//-----------------------------Pro_SkillUsed_req--------------------//
Pro_SkillUsed_req::Pro_SkillUsed_req()
:inherit( APP_SKILLUSED_REQ)
{
}

//-----------------------------Pro_SkillUsed_ack--------------------//
Pro_SkillUsed_ack::Pro_SkillUsed_ack()
:inherit( APP_SKILLUSED_ACK),
skillobj_( 0)
{
}

Pro_SkillUsed_ack::~Pro_SkillUsed_ack()
{
	if( skillobj_)
		delete skillobj_;
	skillobj_ =0;
}

PROTOCOL_NAMESPACE_END
