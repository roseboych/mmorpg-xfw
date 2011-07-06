/**
* css application
*
* @category		player skill
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLDATAPACKAGEBASE__H__
#define __SKILLDATAPACKAGEBASE__H__

#include "../../css_pre.h"
#include <prolib/SerializeObject.h>

struct lua_State;

/**
* @class SkillDataPackageBase
* 
* @brief player技能数据包基类
**/
class SkillDataPackageBase
{
public:
	SkillDataPackageBase();
	virtual ~SkillDataPackageBase();

	/**
	* 设置技能编号
	* @param id
	**/
	virtual void set_skillid( S_INT_32 id){}
	/**
	* 设置最后运行时间
	* @param st
	**/
	virtual void set_lastruntime( S_TIMESTAMP st){}

	virtual S_TIMESTAMP get_lastruntime(){ return 0;}

	/**
	* 初始化技能运行时数据包
	**/
	virtual void init_skillruntime(){}

	/**
	* 复制一个运行时数据
	* @return
	**/
	virtual SerializeObject* clone_runtimedata(){ return 0;}

	/**
	* 根据数据包编号获取对应的对象
	* @param iid
	* @return
	**/
	static SkillDataPackageBase* get_skilldatapackage( S_INT_16 iid);

	static void script_regist( lua_State* l);
};

#endif	//__SKILLDATAPACKAGEBASE__H__
