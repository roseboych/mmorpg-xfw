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
* @brief 任务奖励信息
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
	//物品配置
	ItemInfoBase*	item_;
	//数量
	S_INT_32		item_nums_;
};

/**
* @class TaskRewardGroup
* 
* @brief 任务奖励分组
**/
class TaskRewardGroup
{
	typedef NS_STL::vector<TaskReward*>	TASKREWARDS_V;
public:
	TaskRewardGroup();
	~TaskRewardGroup();

	bool load_reward( TiXmlElement* e1);

	/**
	* 奖励需要占据的背包格子数
	**/
	S_INT_32 get_reward_slotnums(){
		return (S_INT_32)rewards_.size();
	}

protected:
	void release();

public:
	//分组编号
	S_INT_32		group_id_;
	//奖励的经验
	S_INT_32		exp_;

	//分组包含的奖励
	TASKREWARDS_V	rewards_;
};

#endif	//__TASKREWARD__H__
