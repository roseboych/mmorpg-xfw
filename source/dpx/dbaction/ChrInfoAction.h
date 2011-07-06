/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CHRINFOACTION__H__
#define __CHRINFOACTION__H__

#include "../dpx_pre.h"

#include <prolib/datastore/character_define.h>
#include <prolib/datastore/buffer_define.h>
#include <corelib/db/BasicPoolAction.h>

#include "dbaction_def.h"

class Player;

/**
* @class ChrInfoAction
* 
* @brief 角色详情action
*
**/
class ChrInfoAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
	typedef NS_STL::list< PRO::equipment_item*>	EQUIPITEM_LIST;
	typedef NS_STL::list< PRO::petdata_item*>	PETDATA_LIST;
	typedef NS_STL::list< PRO::bufferdata_item*>	BUFFERDATA_LIST;
public:
	ChrInfoAction( S_TIMESTAMP s);
	virtual ~ChrInfoAction();

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

private:
	void release();

public:
	//输入的参数和环境
	Player*	player_;
	uuid_session uuid_;
	S_INT_32	chrid_;

	//输出的结果
	PRO::character_baseinfo	baseinfo_;
	EQUIPITEM_LIST	items_;
	PETDATA_LIST	pets_;
	BUFFERDATA_LIST	buffers_;
};

#endif	//__CHRINFOACTION__H__
