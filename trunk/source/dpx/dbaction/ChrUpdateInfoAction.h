/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CHRUPDATEINFOACTION__H__
#define __CHRUPDATEINFOACTION__H__

#include "../dpx_pre.h"

#include <corelib/db/BasicPoolAction.h>
#include <prolib/datastore/character_define.h>

#include "dbaction_def.h"

/**
* @class ChrUpdateInfoAction
* 
* @brief 更新角色信息action
*
**/
class ChrUpdateInfoAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	ChrUpdateInfoAction( S_TIMESTAMP s);
	virtual ~ChrUpdateInfoAction();

public:
	/**
	* @see BasicPoolAction::run_inmainpool()
	**/
	virtual void run_inmainpool();

protected:
	/**
	* @see BasicPoolAction::run_indbpool( otl_connect *pconn)
	**/
	virtual void run_indbpool( otl_connect *pconn);

public:
	//输入的参数和环境
	S_INT_32	chrid_;
	PRO::character_baseinfo baseinfo_;

	//输出的结果
	S_INT_32	ret_;
};

#endif	//__CHRUPDATEINFOACTION__H__
