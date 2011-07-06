/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ITEMUPDATEACTION__H__
#define __ITEMUPDATEACTION__H__

#include "../dpx_pre.h"

#include <corelib/db/BasicPoolAction.h>

#include "dbaction_def.h"

class Player;

/**
* @class ItemUpdateAction
* 
* @brief 物品更新action
*
**/
class ItemUpdateAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	ItemUpdateAction( S_TIMESTAMP s);

public:
	/**
	* @see BasicPoolAction::run_inmainpool()
	**/
	virtual void run_inmainpool();

	/**
	* @see BasicPoolAction::validate()
	**/
	virtual bool validate();

protected:
	/**
	* @see BasicPoolAction::run_indbpool( otl_connect *pconn)
	**/
	virtual void run_indbpool( otl_connect *pconn);

public:
	//输入的参数和环境
	S_INT_32	userid_;
	Player*	player_;
	uuid_session uuid_;

	//输出的结果
	S_INT_8		ret_;

};

#endif	//__ITEMUPDATEACTION__H__
