/**
* prolib
* 
* @category		skill module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/datastore/SkillHelper.h"

#include "prolib/datastore/skill_define.h"

int SkillHelper::get_skillnofromid( int id)
{
	return (id & MASK_SKILL_IDHEAD) >> MASK_SKILL_LENGTH;
}

bool SkillHelper::is_skillexist( int skillid, int skillsave[])
{
	int len =sizeof( skillsave)/sizeof(int);
	if( len == 0)
		return false;

	int ind =get_skillnofromid( skillid);
	--ind;
	if( ind < 0 || ind >= len)
		return false;

	return (skillsave[ind] & skillid) > 0;
}

void SkillHelper::set_skillflag( int skillid, int* skillsave, int savesize, bool vset)
{
	int ind =get_skillnofromid( skillid);
	--ind;
	if( ind < 0 || ind >= savesize)
		return;

	skillid =skillid & MASK_SKILL_IDBIT;
	skillsave =skillsave + ind;
	*skillsave = ((*skillsave) | skillid);
}
