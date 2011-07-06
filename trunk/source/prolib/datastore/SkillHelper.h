/**
* skill module
*
* @category		skill module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLHELPER__H__
#define __SKILLHELPER__H__

/**
* @class SkillHelper
* 
* @brief 技能工具类
**/
class SkillHelper
{
private:
	SkillHelper(){}

public:
	/**
	* 从技能id获取技能存储的字段编号
	* @param id
	* @return
	**/
	static int get_skillnofromid( int id);

	/**
	* 检测存储的技能数据，是否包含指定的技能
	* @param skillid	需要检测的技能
	* @param skillsave	存储的技能数据
	* @return
	**/
	static bool is_skillexist( int skillid, int skillsave[]);

	/**
	* 设置学习的技能标签
	* @param skillid
	* @param skillsave
	* @param vset
	**/
	static void set_skillflag( int skillid, int* skillsave, int savesize, bool vset =true);

};

#endif	//__SKILLHELPER__H__
