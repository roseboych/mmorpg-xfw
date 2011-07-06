/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CHRGETLISTACTION__H__
#define __CHRGETLISTACTION__H__

#include "../dpx_pre.h"

#include <corelib/db/BasicPoolAction.h>
#include "dbaction_def.h"
#include "../player/ChrListInfo.h"

class Player;

/**
* @class ChrGetListAction
* 
* @brief 获取角色列表action
*
**/
class ChrGetListAction : public BasicPoolAction
{
	typedef BasicPoolAction	inherit;
public:
	ChrGetListAction( S_TIMESTAMP s);
	virtual ~ChrGetListAction();

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

	void release();

	ChrListInfo* get_chrinfo( S_INT_32 cid);

public:
	//输入的参数和环境
	Player*	player_;
	uuid_session uuid_;

	//返回值
	NS_STL::vector<ChrListInfo*>	chrs_;
};

#endif	//__CHRGETLISTACTION__H__
