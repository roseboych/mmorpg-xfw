/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskConsume.h"

#include "reslib/items/ItemInfoBase.h"

TaskConsume::TaskConsume():
item_( 0),
must_taskitem_( false),
item_nums_( 0),
will_destroy_( true)
{

}

int TaskConsume::get_itemid()
{
	return item_->get_itemid();
}

bool TaskConsume::is_taskitem()
{
	return item_->is_taskitem();
}

bool TaskConsume::validate()
{
	if( item_ == 0)
		return false;

	if( must_taskitem_ && !item_->is_taskitem())
		return false;

	return true;
}