/**
* css
*
* @category		task system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKHELPER__H__
#define __TASKHELPER__H__

#include <corelib/corelibdef.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <vector>

class TaskBase;
class Player;
class NPCEntity;
class BaseStoryService;

/**
* @class TaskHelper
* 
* @brief 任务系统帮助类
* 
**/
class TaskHelper
{
	typedef NS_STL::vector< TaskBase*>	TASKIMPL_V;
public:
	TaskHelper();
	~TaskHelper();

	/**
	* 获取任务
	* @param tid	任务id
	* @param cplayr	请求人
	* @param psvr	
	* @param npc	任务所在的npc
	* @return		0:成功 
	**/
	S_INT_8 get_task( S_INT_32 tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc);

	/**
	* 提交任务
	* @param tid	任务id
	* @param cplayr	请求人
	* @param psvr	
	* @param npc	任务所在的npc
	* @return		0:成功 
	**/
	S_INT_8	submit_task( S_INT_32 tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc);

protected:
	void release();

	/**
	* 根据类名获取实例
	* @param clsname
	* @return
	**/
	TaskBase* get_taskbyclsname( NS_STL::string clsname);

protected:
	//任务c++实现
	TASKIMPL_V	taskimpl_;
};

#define TASKHelper ACE_Singleton< TaskHelper, ACE_Null_Mutex>::instance()

#endif	//__TASKHELPER__H__
