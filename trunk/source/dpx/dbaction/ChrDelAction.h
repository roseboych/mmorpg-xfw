/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CHRDELACTION__H__
#define __CHRDELACTION__H__

#include "../dpx_pre.h"

#include <corelib/db/BasicPoolAction.h>

#include "dbaction_def.h"

class Player;

/**
* @class ChrDelAction
* 
* @brief 删除角色action
*
**/
class ChrDelAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	ChrDelAction( S_TIMESTAMP s);

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
	Player*	player_;
	uuid_session uuid_;
	S_INT_32	chrid_;

	//输出的结果
	S_INT_16	ret_;

};

#endif	//__CHRDELACTION__H__
