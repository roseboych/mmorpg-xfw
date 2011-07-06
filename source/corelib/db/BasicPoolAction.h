/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICPOOLACTION__H__
#define __BASICPOOLACTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "prolib/core_type.h"

class otl_connect;

/**
* @class BasicPoolAction
* 
* @brief database线程池中处理命令的功能基类
*
**/
class BasicPoolAction
{
public:
	BasicPoolAction( int t, S_TIMESTAMP st);
	virtual ~BasicPoolAction();

	/**
	* 数据库线程执行的数据库访问
	* @param pconn	使用的数据库连接
	**/
	void rundb( otl_connect *pconn){
		try{
			run_indbpool( pconn);
		}
		catch(...){}
	}

	/**
	* 如果是返回的数据库任务，主线程的执行入口
	**/
	virtual void run_inmainpool(){}

	/**
	* 数据库任务类型
	**/
	int get_type(){ return type_;}

	/**
	* 检查改环境是否有效
	**/
	virtual bool validate(){ return true;};

protected:
	/**
	* 数据库线程的执行实现
	* @param pconn
	**/
	virtual void run_indbpool( otl_connect *pconn){};

public:
	//类型
	int	 type_;
	//创建时间
	S_TIMESTAMP	create_time_;
};

#endif	//__BASICPOOLACTION__H__
