/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKCONFIG__H__
#define __TASKCONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <corelib/script/ScriptContext.h>

#include "reslib/tasks/TaskInfo.h"

#include <string>
#include <map>

/**
* @class TaskConfig
* 
* @brief ������Ϣ���ù���
**/
class TaskConfig
{
	typedef NS_STL::map< S_INT_32, TaskInfo*>	TASKINFO_MAP;
public:
	TaskConfig();
	~TaskConfig();

	/**
	* ��ʼ��������Դ
	* @return
	**/
	bool load_taskconfig();

	/**
	* ע�����е�gamesetting�ű����ű�����
	* @param context
	* @param bool
	**/
	bool regist_to_storyscriptcontext( app::script::ScriptContext& context);

	/**
	* ���������Ż�ȡ����������Ϣ
	* @param id
	* @return
	**/
	TaskInfo* get_taskinfobyid( S_INT_32 id);

protected:
	void release();

protected:
	//����ע�������
	TASKINFO_MAP	tasks_res_;
	//����ϵͳ�����ű�
	NS_STL::string	task_script_;
};

#define TASKRES_CFG	ACE_Singleton< TaskConfig, ACE_Null_Mutex>::instance()

#endif	//__TASKCONFIG__H__
