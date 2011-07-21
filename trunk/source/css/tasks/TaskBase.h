/**
* css
*
* @category		task system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKBASE__H__
#define __TASKBASE__H__

#include <reslib/reslib_def.h>
#include <reslib/tasks/TaskInfo.h>

#include <corelib/corelibdef.h>
#include <string>

class Player;
class TaskInfo;

#define TASKCLASSIMPL_DEFINE( classname)							\
public:																\
	virtual const NS_STL::string& get_classname(){					\
		static NS_STL::string s_##classname_name =#classname;		\
		return s_##classname_name;									\
	}

/**
* @class TaskBase
* 
* @brief 任务实现基类。每个具体的实现类对应task.xml里面的一个配置节点
**/
class TaskBase
{
public:
	virtual const NS_STL::string& get_classname() =0;

	/**
	* 获取任务检测
	* @param curplayer
	* @param tparam
	* @return
	**/
	virtual S_BOOL get_task( Player* curplayer, TaskInfo* tparam) =0;

	/**
	* 提交任务
	* @param curplayer
	* @param tparam
	* @return
	**/
	virtual S_BOOL submit_task( Player* curplayer, TaskInfo* tparam) =0;
};

#endif	//__TASKBASE__H__
