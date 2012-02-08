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
	bool		validate();

	int			get_itemid();
	bool		is_taskitem();

public:
	//对应的物品配置
	ItemInfoBase*	item_;
	//是否必须是任务物品
	bool			must_taskitem_;
	//物品数量
	int				item_nums_;
	//完成任务时是否从背包销毁
	bool			will_destroy_;
};

#endif	//__TASKCONSUME__H__
