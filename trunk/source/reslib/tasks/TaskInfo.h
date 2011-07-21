/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKINFO__H__
#define __TASKINFO__H__

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>

#include "reslib/tasks/TaskConsume.h"
#include "reslib/tasks/TaskReward.h"

#include <string>
#include <vector>

class TiXmlElement;
class TaskParamBase;

/**
* @class TaskInfo
* 
* @brief 任务配置信息
**/
class TaskInfo
{
	typedef NS_STL::vector< TaskConsume*>		TASKCONSUME_V;
	typedef NS_STL::vector< TaskRewardGroup*>	TASKREWARDGROUP_V;
public:
	TaskInfo();
	~TaskInfo();

	/**
	* 加载任务信息
	* @param e
	* @return
	**/
	S_BOOL	load_task();

	/**
	* 根据奖励的分组编号获取奖励信息
	* @param gid
	* @return
	**/
	TaskRewardGroup* get_rewardbygid( S_INT_32 gid);

protected:
	void release();

public:
	//任务编号
	S_INT_32			task_id_;
	//是否有效
	S_BOOL				is_validate_;
	//支持类型
	FUNCTION_SUPPORT	fun_type_;
	//c++参数配置
	TaskParamBase*		clsimpl_param_;
	//脚本支持
	NS_STL::string		task_script_;

	//任务消耗
	TASKCONSUME_V		taskconsumes_;
	//任务奖励
	TASKREWARDGROUP_V	taskrewards_;

	//任务信息存放路径
	NS_STL::string	res_path_;
};

#endif	//__TASKINFO__H__
