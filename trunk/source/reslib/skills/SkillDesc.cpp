/**
* reslib application
* 
* @category		skill config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/skills/SkillDesc.h"

SkillDesc::SkillDesc()
{

}

NS_STL::string SkillDesc::get_skillinstance()
{
	return "_sk_"+skillidsn_;
}
