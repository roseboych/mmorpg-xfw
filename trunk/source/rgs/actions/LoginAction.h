/**
* rgs application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LOGINACTION__H__
#define __LOGINACTION__H__

#include "../rgs_pre.h"

#include <corelib/db/BasicPoolAction.h>

#include "dbaction_def.h"

class PlayerInfo;

/**
* @class LoginAction
* 
* @brief 登陆认证action
*
**/
class LoginAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	LoginAction( S_TIMESTAMP s);

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
	NS_STL::string token_;
	PlayerInfo*	player_;

	uuid_session uuid_;

	//输出的结果
	S_INT_8		ret_;
	S_INT_32	uid_;
	S_INT_32	validatetime_;
	S_INT_16	lastregion_;
	S_INT_32	lastlogout_;
	S_INT_16	types_;
	S_INT_16	isdel_;
};

#endif	//__LOGINACTION__H__
