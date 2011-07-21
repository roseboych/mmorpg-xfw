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
* @brief ����ϵͳ������
* 
**/
class TaskHelper
{
	typedef NS_STL::vector< TaskBase*>	TASKIMPL_V;
public:
	TaskHelper();
	~TaskHelper();

	/**
	* ��ȡ����
	* @param tid	����id
	* @param cplayr	������
	* @param psvr	
	* @param npc	�������ڵ�npc
	* @return		0:�ɹ� 
	**/
	S_INT_8 get_task( S_INT_32 tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc);

	/**
	* �ύ����
	* @param tid	����id
	* @param cplayr	������
	* @param psvr	
	* @param npc	�������ڵ�npc
	* @return		0:�ɹ� 
	**/
	S_INT_8	submit_task( S_INT_32 tid, Player* cplayer, BaseStoryService* psvr, NPCEntity* npc);

protected:
	void release();

	/**
	* ����������ȡʵ��
	* @param clsname
	* @return
	**/
	TaskBase* get_taskbyclsname( NS_STL::string clsname);

protected:
	//����c++ʵ��
	TASKIMPL_V	taskimpl_;
};

#define TASKHelper ACE_Singleton< TaskHelper, ACE_Null_Mutex>::instance()

#endif	//__TASKHELPER__H__
