/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CHRCREATEACTION__H__
#define __CHRCREATEACTION__H__

#include "../dpx_pre.h"

#include <prolib/Pro_all.h>
#include <corelib/db/BasicPoolAction.h>

#include "dbaction_def.h"
#include "../player/ChrListInfo.h"

class Player;

/**
* @class ChrCreateAction
* 
* @brief 创建角色action
*
**/
class ChrCreateAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	ChrCreateAction( S_TIMESTAMP s);
	virtual ~ChrCreateAction();

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
	PRO::Pro_ChrCreate_req* param_;

	//输出的结果
	S_INT_32	ret_;
	ChrListInfo	*chrinfo_;
};

#endif	//__CHRCREATEACTION__H__
