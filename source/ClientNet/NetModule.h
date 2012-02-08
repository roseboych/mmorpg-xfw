/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __NETMODULE__H__
#define __NETMODULE__H__

#include "windows.h"

#include "prolib/Pro_all.h"
#include "prolib/BasicProtocol.h"
#include "prolib/ProtocolFactory.h"
#include "prolib/SingletonT.h"
#include "BasicCommand.h"

#include <list>
#include <string>

class BaseTestCase;

#define MAX_PACK_LEN 16*1024
#define SINGLE_PACK_LEN 1024*8

//连接事件
enum ConnectEventEnum{
	//正在连接服务器
	CONNEVT_CONNECTING =0,
	//已经发起一个连接请求
	CONNEVT_HASREQCONN,
	//成功连接服务器
	CONNEVT_CONNECTED,
	//服务器连接失败
	CONNEVT_CONNECTFAIL,
	//开始验证用户名密码
	CONNEVT_UNAMEPWD,
	//登陆失败
	CONNEVT_LOGINFAIL,
	//登陆成功
	CONNEVT_LOGIN
};

//服务器类型
enum GameServerTypeEnum
{
	//lgs
	GAMESERVER_LGS,
	//gts
	GAMESERVER_GTS,
};

//定义登陆和断线的事件回调
typedef void (*FUN_NETCONNECT_CALLBACK)( ConnectEventEnum, GameServerTypeEnum);
typedef void (*FUN_NETDISCONN_CALLBACK)( GameServerTypeEnum);
typedef void (*FUN_NETCOMMAND_CALLBACK)( BasicProtocol*, bool&);

/**
* @class	ConnectEvtCommand
* 
* @brief 连接系统事件
**/
class ConnectEvtCommand : public BasicCommand
{
public:
	ConnectEvtCommand( FUN_NETCONNECT_CALLBACK fun, ConnectEventEnum evt, GameServerTypeEnum stype):call_fun_( fun), evt_(evt), stype_(stype){}

	virtual void run(){
		if( call_fun_)
			call_fun_( evt_, stype_);
	}

private:
	FUN_NETCONNECT_CALLBACK call_fun_;
	ConnectEventEnum evt_;
	GameServerTypeEnum	stype_;
};

/**
* @class DisConnectCommand
* 
* @brief 系统断线事件
**/
class DisConnectCommand : public BasicCommand
{
public:
	DisConnectCommand( FUN_NETDISCONN_CALLBACK fun, GameServerTypeEnum st):call_fun_( fun), stype_(st){}

	virtual void run(){
		if( call_fun_)
			call_fun_( stype_);
	}

private:
	FUN_NETDISCONN_CALLBACK call_fun_;
	GameServerTypeEnum		stype_;
};

/**
* @class NetCommand
* 
* @brief 网络命令
**/
class NetCommand : public BasicCommand
{
public:
	NetCommand( FUN_NETCOMMAND_CALLBACK fun, BasicProtocol* pro)
		:call_fun_( fun),pro_( pro), autorelease_(false){}

	virtual ~NetCommand(){
		if( pro_ && autorelease_)
			delete pro_;
	}

	virtual void run(){
		if( call_fun_)
			call_fun_( pro_, autorelease_);
	}

private:
	FUN_NETCOMMAND_CALLBACK call_fun_;
	BasicProtocol*	pro_;
	bool autorelease_;
};

/**
* @class MutexGuard
* 
* @brief 
**/
class MutexGuard
{
public:
	MutexGuard( CRITICAL_SECTION* l):ck_( l){
		::EnterCriticalSection( ck_);
	}

	~MutexGuard(){
		::LeaveCriticalSection( ck_);
	}

private:
	CRITICAL_SECTION* ck_;
};

/**
* @class NetModule
* 
* @brief 客户端使用的网络模块
**/
class NetModule : public SingletonT< NetModule>
{
public:
	typedef NS_STL::list< BasicCommand*>	COMMAND_LIST;
	typedef NS_STL::list< BasicProtocol*>	PROTOCOL_LIST;

public:
	NetModule();
	~NetModule();

public:
	/**
	* 初始化网络模块
	* @param f1			连接回调函数
	* @param disfun		断线回调函数
	* @param offline	是否离线模式
	* @param testcase
	*
	* @return 
	**/
	bool init_netmodule( FUN_NETCONNECT_CALLBACK f1, FUN_NETDISCONN_CALLBACK disfun, FUN_NETCOMMAND_CALLBACK netfun,
		bool offline =false, const char* testcase ="");

	/**
	* 释放网络模块
	**/
	void uninit_netmodule();

	/**
	* 连接到服务器
	* @param ip		服务器ip
	* @param port	服务器端口
	* @param uname
	* @param pwd
	**/
	void connect_lgs( const char* ip, int port, const char* uname, const char* pwd);
	void disconnect_lgs();

	void connect_gts( const char* ip, int port, int server_index, S_TIMESTAMP token);
	void disconnect_gts();

	/**
	* 网络模块主循环，在unreal中调用
	**/
	void net_loop();

	/**
	* 发送协议
	* @param pro	需要发送的协议
	**/
	void send_lgs_protocol( BasicProtocol* pro);
	void send_gts_protocol( BasicProtocol* pro);

	/**
	* 离线模式使用的向客户端发送协议
	* @param pro
	**/
	void offline_sendtoclient( BasicProtocol* pro);

	/**
	* 插入系统命令和网路命令
	**/
	void push_syscmd( BasicCommand* pcmd);
	void push_netcmd( BasicCommand* pcmd);

protected:

	/**
	* 是否已经连接上
	* @return
	**/
	bool is_connected_lgs(){
		return lgs_socket_ != INVALID_SOCKET;
	}
	bool is_connected_gts(){
		return gts_socket_ != INVALID_SOCKET;
	}

	void recv_lgs_protocol();
	void send_lgs_protocol();

	void recv_gts_protocol();
	void send_gts_protocol();

	/**
	* 释放资源
	**/
	void release();

	/**
	* 关闭socket
	**/
	void wait_socket_close( int type =2);

	/**
	* 线程处理函数
	*
	* @param pdata
	**/
	static void thread_proc_lgs( void* pdata);
	static void thread_proc_gts( void* pdata);


	/**
	* 离线模式下的tick处理
	**/
	void offline_tick();

protected:
	NS_STL::string ip_;
	int	port_;
	NS_STL::string username_;
	NS_STL::string password_;

	FUN_NETCONNECT_CALLBACK	connevt_fun_;
	FUN_NETDISCONN_CALLBACK	disconn_fun_;
	FUN_NETCOMMAND_CALLBACK	netcmd_fun_;

	bool	is_offline_;
	std::string casename_;
	BaseTestCase* testcase_;

	COMMAND_LIST	sys_cmds_;
	COMMAND_LIST	net_cmds_;

	CRITICAL_SECTION lock_;
	CRITICAL_SECTION lock2_;

	PROTOCOL_LIST	lgs_send_pros_;
	SOCKET	lgs_socket_;
	char lgs_recv_buffer_[MAX_PACK_LEN];
	int	 lgs_data_len_;

	NS_STL::string gts_ip_;
	int		gts_port_;
	int		server_index_;
	S_TIMESTAMP token_;
	PROTOCOL_LIST	gts_send_pros_;
	SOCKET	gts_socket_;
	char gts_recv_buffer_[MAX_PACK_LEN];
	int	 gts_data_len_;

	ProtocolFactory* pro_factory_;

	unsigned int thread_handle_lgs_;
	bool volatile exit_thread_lgs_;
	unsigned int thread_handle_gts_;
	bool volatile exit_thread_gts_;
};

#endif	//__NETMODULE__H__
