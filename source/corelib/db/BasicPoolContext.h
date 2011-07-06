/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICPOOLCONTEXT__H__
#define __BASICPOOLCONTEXT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "ace/Time_Value.h"
#include "prolib/core_type.h"

class otl_connect;

/**
* @class BasicPoolContext
* 
* @brief database线程池中处理命令的基类
*
**/
class BasicPoolContext
{
public:
	BasicPoolContext();
	virtual ~BasicPoolContext(void);

	/**
	* 获取action类型
	**/
	virtual int get_type()=0;

	/**
	* 数据库线程池中执行的方法入口
	* @param pconn
	**/
	virtual void run_it( otl_connect *pconn) =0;

	/**
	* 检查改环境是否有效
	**/
	virtual bool validate() =0;

	void set_mapid( int id){
		map_id_ =id;
	}
	int get_mapid(){
		return map_id_;
	}
	void set_used( bool u){
		used_it_ =u;
	}
	bool not_used(){
		return !used_it_;
	}

public:
	//双向链表管理
	BasicPoolContext* get_next(){
		return after_context_;
	}
	void set_next( BasicPoolContext* p){
		after_context_ =p;
	}
	BasicPoolContext* get_pre(){
		return before_context_;
	}
	void set_pre( BasicPoolContext* p){
		before_context_ =p;
	}

public:
	//是否异步返回 如果是需要异步返回的，必须继承BasicCommand基类
	bool return_it_;

private:
	//用双向链表实现 db的时序操作
	BasicPoolContext	*before_context_;
	BasicPoolContext	*after_context_;
	//使用的排序id
	int			map_id_;
	bool		used_it_;
};


#endif	//__BASICPOOLCONTEXT__H__
