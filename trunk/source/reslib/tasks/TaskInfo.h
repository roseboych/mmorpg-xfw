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
* @brief ����������Ϣ
**/
class TaskInfo
{
	typedef NS_STL::vector< TaskConsume*>		TASKCONSUME_V;
	typedef NS_STL::vector< TaskRewardGroup*>	TASKREWARDGROUP_V;
public:
	TaskInfo();
	~TaskInfo();

	/**
	* ����������Ϣ
	* @param e
	* @return
	**/
	S_BOOL	load_task();

	/**
	* ���ݽ����ķ����Ż�ȡ������Ϣ
	* @param gid
	* @return
	**/
	TaskRewardGroup* get_rewardbygid( S_INT_32 gid);

protected:
	void release();

public:
	//������
	S_INT_32			task_id_;
	//�Ƿ���Ч
	S_BOOL				is_validate_;
	//֧������
	FUNCTION_SUPPORT	fun_type_;
	//c++��������
	TaskParamBase*		clsimpl_param_;
	//�ű�֧��
	NS_STL::string		task_script_;

	//��������
	TASKCONSUME_V		taskconsumes_;
	//������
	TASKREWARDGROUP_V	taskrewards_;

	//������Ϣ���·��
	NS_STL::string	res_path_;
};

#endif	//__TASKINFO__H__
