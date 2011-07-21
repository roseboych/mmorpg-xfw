/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TASKCONSUME__H__
#define __TASKCONSUME__H__

#include "reslib/reslib_def.h"
#include <corelib/corelibdef.h>

class ItemInfoBase;

/**
* @class TaskConsume
* 
* @brief 任务消耗信息
**/
class TaskConsume
{
public:
	TaskConsume();

	/**
	* 验证数据的合法性
	**/
	S_BOOL		validate();

	S_INT_32	get_itemid();
	S_BOOL		is_taskitem();

public:
	//对应的物品配置
	ItemInfoBase*	item_;
	//是否必须是任务物品
	S_BOOL			must_taskitem_;
	//物品数量
	S_INT_32		item_nums_;
	//完成任务时是否从背包销毁
	S_BOOL			will_destroy_;
};

#endif	//__TASKCONSUME__H__
