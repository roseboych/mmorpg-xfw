/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICSERVICE__H___
#define __BASICSERVICE__H___

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "corelib/corelibdef.h"

#include <ace/Message_Block.h>
#include <ace/Mutex.h>
#include <ace/Thread_Mutex.h>

#ifdef CORELIB_WIN32
	#include <ace/Asynch_IO.h>
#else
	#include <ace/SOCK_Stream.h>
	#include <ace/Event_Handler.h>
#endif

#include <list>

#include "prolib/core_type.h"
#include "prolib/BasicProtocol.h"
#include "prolib/ProtocolFactory.h"

#include "corelib/core/MutexAllocator.h"
#include "corelib/net/Basic_Qos.h"
#include "corelib/net/Asynch_Read_Stream_Wrap.h"
#include "corelib/net/Asynch_Write_Stream_Wrap.h"

//max size of single protocl package
#define SINGLE_PROTOCOL_SIZE	8192	//8k

//send/read protocol package size one times
#define PROTOCOL_SEND_SIZE		8192	//8k
#define PROTOCOL_READ_SIZE		8192	//8k

/**
* @class BasicService
* 
* @brief asynchacceptor、asynchconnector中用的到的参数类基类。
* 封装了从iocp读取写入流式数据的功能。
**/
class BasicService
#ifdef CORELIB_WIN32
	: public ACE_Service_Handler
#else
	: public ACE_Event_Handler
#endif
{
#ifdef CORELIB_WIN32
	typedef ACE_Service_Handler inherit;
#else
	typedef ACE_Event_Handler inherit;
#endif

	typedef NS_STL::list< BasicProtocol*>	PROTOCOLLIST;
	typedef NS_STL::list< BasicProtocol*>::iterator PROTOCOLLIST_ITER;

public:
	virtual ~BasicService(void);

#ifdef CORELIB_WIN32
	BasicService( MutexAllocator *allocator_, 
		ACE_Proactor *proactor, ProtocolFactory *impl =0, void *ext =0,
		bool internals =true);

	//win32 use iocp/proactor
	virtual void open( ACE_HANDLE new_handle, ACE_Message_Block &message_block);
	virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
	virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

#else
	BasicService( MutexAllocator *allocator_, 
		ACE_Reactor *reactor, ProtocolFactory *impl =0, void *ext =0,
		bool internals =true);

	//exclude win32 .tp_reactor
	virtual int open();
	virtual int handle_input( ACE_HANDLE =ACE_INVALID_HANDLE);
	virtual int handle_output (ACE_HANDLE =ACE_INVALID_HANDLE);
	virtual int handle_close( ACE_HANDLE =ACE_INVALID_HANDLE, ACE_Reactor_Mask =0);

	virtual ACE_HANDLE get_handle (void) const{
		return stream_.get_handle();
	}
	ACE_SOCK_Stream &peer(){
		return stream_;
	}

	//阻塞模式接收发送数据
	bool send_one_block( BasicProtocol* p);
	BasicProtocol* recv_one_block();

#endif

	//链接断开错误
	virtual void conn_error_nomutex();
	virtual void conn_error();

	/**
	* 收到一个协议
	* 在iocp线程池中执行
	* @param protocol
	**/
	virtual bool recv_msg( BasicProtocol* protocol) =0;

	/**
	* 增加需要发送的协议到发送队列
	* 线程安全
	* @param msg
	**/
	virtual int append_write_msg( BasicProtocol* msg);

	/**
	* 主动关闭socket的service
	* @param mutex
	**/
	virtual void close_service( bool mutex =true);

	/**
	* 是否初始化
	* @return
	**/
	bool initialized(){ return initialized_;}

	/**
	* 设置初始化完成标志
	**/
	void set_initalized(){ 
		ACE_Guard<ACE_Thread_Mutex> mon( *write_mutex_);
		initialized_ =true;
	};

	/**
	* 获取和socket绑定的ip地址
	* @return
	**/
	virtual NS_STL::string get_ip(){ return "";};

	/**
	* 获取每个应用各自定义的Service类型
	* @return int
	**/
	virtual int get_serviceimpl() =0;

protected:
	//往队列中加一个消息
	int push_write_msg( BasicProtocol* msg);

	/**
	* 获取下一个发送协议
	* 在iocp线程池中执行
	* @return
	**/
	BasicProtocol* next_write_msg();
	
	/**
	* 把协议加入发送队列最前端
	* @param msg
	**/
	void push_head_write_msg( BasicProtocol* msg);
	
	/**
	* 获取发送队列长度
	* @return 
	**/
	int write_queue_size();

	/**
	* 释放队列数据
	**/
	void release_queue();

	/**
	* 发送数据前处理
	* 可以在gateway端重载使用
	* @param msg
	**/
	virtual void pre_send_protocol( BasicProtocol* msg){};

	/**
	* 开始注册一个服务器
	**/
	virtual void begin_registservice_2(){}

protected:
	/**
	* 处理收到的协议，解析成协议包
	* @param len
	**/
	int analy_package( size_t len);

	/**
	* 把需要发送的协议，填充到发送缓冲
	**/
	inline void fill_writebuff();

	/**
	* 发送缓冲置空
	**/
	inline void zero_write_status();

	/**
	* 设置协议工厂类实现
	* @param fac
	* @param delete_it
	**/
	void set_protocolfactory( ProtocolFactory *fac, bool delete_it);

	/**
	* 设置qos实现
	* @param q
	* @param delete_id
	**/
	void set_qos( Basic_Qos *q, bool delete_it);

protected:

#ifdef CORELIB_WIN32
	//ACE_Asynch_Read_Stream reader_;
	Asynch_Read_Stream_Wrap	reader_;
	//ACE_Asynch_Write_Stream writer_;
	Asynch_Write_Stream_Wrap writer_;

#else
	ACE_SOCK_Stream	stream_;

	char send_block_tmpbuf_[SINGLE_PROTOCOL_SIZE];
	char recv_block_tmpbuf_[SINGLE_PROTOCOL_SIZE];
#endif

	//读写协议块
	ACE_Message_Block  read_message_;
	ACE_Message_Block  write_message_;
	//已经写入的消息长度
	size_t	write_message_len_;

	//临时缓存
	char fill_writebuff_tmp_[SINGLE_PROTOCOL_SIZE];

	//initialized status
	bool	initialized_;

	//发数据同步锁
	ACE_Thread_Mutex*	write_mutex_;

	//扩展数据
	void*	ext_data_;
	//是否服务器间传递
	bool	internals_;

	//protocol factory
	ProtocolFactory	*pro_factory_;
	bool	delete_profactory_;

	//qos
	Basic_Qos*	qos_;
	bool	delete_qos_;

	//发送消息队列
	PROTOCOLLIST	protocol_queue_;

private:
	//锁分配器
	MutexAllocator	*mutex_allocator_;
};

#endif //__BASICSERVICE__H___
