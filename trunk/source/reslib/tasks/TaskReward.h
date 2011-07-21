/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKREWARD__H__
#define __TASKREWARD__H__

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>

#include <vector>

class ItemInfoBase;
class TiXmlElement;

/**
* @class TaskReward
* 
* @brief ��������Ϣ
**/
class TaskReward
{
public:
	TaskReward();

	S_INT_32 get_itemid();
	S_INT_32 get_itemnum(){
		return item_nums_;
	}

	operator ItemInfoBase* () const;

public:
	//��Ʒ����
	ItemInfoBase*	item_;
	//����
	S_INT_32		item_nums_;
};

/**
* @class TaskRewardGroup
* 
* @brief ����������
**/
class TaskRewardGroup
{
	typedef NS_STL::vector<TaskReward*>	TASKREWARDS_V;
public:
	TaskRewardGroup();
	~TaskRewardGroup();

	bool load_reward( TiXmlElement* e1);

	/**
	* ������Ҫռ�ݵı���������
	**/
	S_INT_32 get_reward_slotnums(){
		return (S_INT_32)rewards_.size();
	}

protected:
	void release();

public:
	//������
	S_INT_32		group_id_;
	//�����ľ���
	S_INT_32		exp_;

	//��������Ľ���
	TASKREWARDS_V	rewards_;
};

#endif	//__TASKREWARD__H__
