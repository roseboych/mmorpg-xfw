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
* @brief ����������Ϣ
**/
class TaskConsume
{
public:
	TaskConsume();

	/**
	* ��֤���ݵĺϷ���
	**/
	S_BOOL		validate();

	S_INT_32	get_itemid();
	S_BOOL		is_taskitem();

public:
	//��Ӧ����Ʒ����
	ItemInfoBase*	item_;
	//�Ƿ������������Ʒ
	S_BOOL			must_taskitem_;
	//��Ʒ����
	S_INT_32		item_nums_;
	//�������ʱ�Ƿ�ӱ�������
	S_BOOL			will_destroy_;
};

#endif	//__TASKCONSUME__H__
