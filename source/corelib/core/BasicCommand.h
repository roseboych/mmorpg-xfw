/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICCOMMAND__H__
#define __BASICCOMMAND__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/BasicProtocol.h>

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

typedef boost::function< void( BasicProtocol*, bool&)>			NETCMD_FUN_MAP;
typedef boost::function< void( BasicProtocol*, bool&, int)>		NETCMD_FUN_MAP2;
typedef boost::function< void( BasicProtocol*, bool&, void*)>	NETCMD_FUN_MAP3;

/**
* @class BasicCommand
* 
* @brief 命令模式中命令基类
**/
class BasicCommand
{
public:
	BasicCommand( bool d):delete_pro_(d){}
	BasicCommand():delete_pro_(true){}

	virtual ~BasicCommand(){}

	/**
	* 逻辑主线程中的执行入口
	**/
	virtual void run() =0;

public:
	//是否删除协议标志
	bool	delete_pro_;
};

/**
* @class NetCommand
* 
* @brief 封装了网络协议和入口函数的映射
* 
**/
class NetCommand : public BasicCommand
{
public:
	NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP fun, bool autorelease =true);
	NetCommand( BasicProtocol *d2, NETCMD_FUN_MAP2 fun, bool autorelease =true);
	virtual ~NetCommand();

	virtual void run();

private:
	//函数指针
	NETCMD_FUN_MAP	fun_;
	NETCMD_FUN_MAP2	fun2_;
	//保存的协议
	BasicProtocol*	pro_;
};

/**
* @class NetCommand
* 
* @brief 封装了网络协议和入口函数的映射
* 
**/
class NetCommandV : public BasicCommand
{
public:
	NetCommandV( BasicProtocol *d2, NETCMD_FUN_MAP3 fun, bool autorelease =true);
	virtual ~NetCommandV();

	virtual void run();

private:
	//函数指针
	NETCMD_FUN_MAP3	fun_;
	//保存的协议
	BasicProtocol*	pro_;
};

/**
* @class SystemCommand
* 
* @brief 封装了系统事件入口函数的映射
* 
**/
template< class T>
class SystemCommand : public BasicCommand
{
public:
	SystemCommand( boost::function< void( T*)> f);
	virtual ~SystemCommand();

	virtual void run();

private:
	boost::function< void( T*)>	fun_;
};

template< class T>
SystemCommand<T>::SystemCommand( boost::function< void( T*)> f):fun_( f)
{
}

template< class T>
SystemCommand<T>::~SystemCommand()
{
}

template< class T>
void SystemCommand<T>::run()
{
	fun_( (T*)0);
}

#endif //__BASICCOMMAND__H__
