/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/core/BasicService.h"

#if defined (_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4267)
#endif /* _MSC_VER */

#pragma pack( push, 8)
#include <boost/bind.hpp>
#pragma  pack( pop)

#include "ace/OS_NS_sys_socket.h"
#include "ace/Auto_Ptr.h"

#ifndef CORELIB_WIN32
	#include "ace/Reactor.h"
#endif

#include "prolib/ProtocolFactory.h"
#include "corelib/log/logmacro.h"

#include "corelib/core/BasicModule.h"
#include "corelib/core/BasicCommand.h"

#ifdef CORELIB_WIN32

BasicService::BasicService( MutexAllocator *allocator_, 
									ACE_Proactor *proactor, ProtocolFactory *impl, void *ext, bool internals)
									:inherit(),
									read_message_( SINGLE_PROTOCOL_SIZE*2),
									write_message_( SINGLE_PROTOCOL_SIZE*2),
									write_message_len_( 0),
									mutex_allocator_( allocator_),
									write_mutex_( 0),
									initialized_( false),
									ext_data_( ext),
									pro_factory_( impl),
									delete_profactory_( false),
									qos_( 0),
									delete_qos_( false),
									internals_( internals)
{
	//set proactor instance
	this->proactor( proactor);

	write_mutex_ =mutex_allocator_->alloc_one();

	if( pro_factory_ == 0)
	{
		pro_factory_ =FRAMEWK_NEW ProtocolFactory();
		delete_profactory_ =true;
	}
}

#else

BasicService::BasicService( MutexAllocator *allocator_, 
								   ACE_Reactor *reactor, ProtocolFactory *impl, void *ext, bool internals)
								   :inherit( reactor),
								   read_message_( SINGLE_PROTOCOL_SIZE*2),
								   write_message_( SINGLE_PROTOCOL_SIZE*2),
								   write_message_len_( 0),
								   mutex_allocator_( allocator_),
								   write_mutex_( 0),
								   initialized_( false),
								   ext_data_( ext),
								   pro_factory_( impl),
								   delete_profactory_( false),
								   qos_( 0),
								   delete_qos_( false),
								   internals_( internals)
{
	write_mutex_ =mutex_allocator_->alloc_one();

	if( pro_factory_ == 0)
	{
		pro_factory_ =FRAMEWK_NEW ProtocolFactory();
		delete_profactory_ =true;
	}
}

#endif

BasicService::~BasicService(void)
{
	close_service();

	release_queue();

	//释放占用的锁引用
	if( write_mutex_)
		mutex_allocator_->free_one( write_mutex_);
	write_mutex_ =0;

	if( delete_profactory_)
		delete pro_factory_;

	if( delete_qos_ && qos_)
		delete qos_;
	qos_ =0;
}

void BasicService::release_queue()
{
	for( PROTOCOLLIST_ITER iter =protocol_queue_.begin(); iter != protocol_queue_.end(); ++iter)
	{
		BasicProtocol* p =(*iter);
		if( p) 
			delete p;
	}

	protocol_queue_.clear();
}

void BasicService::set_protocolfactory( ProtocolFactory *fac, bool delete_it)
{
	if( delete_profactory_ && pro_factory_)
		delete pro_factory_;

	delete_profactory_ =delete_it;
	pro_factory_ =fac;
}

void BasicService::set_qos( Basic_Qos *q, bool delete_it)
{
	if( delete_qos_ && qos_)
		delete qos_;

	delete_qos_ =delete_it;
	qos_ =q;
}

void BasicService::close_service( bool mutex)
{
	if( mutex)
	{
		ACE_Guard<ACE_Thread_Mutex> mon( *write_mutex_);

		initialized_ =false;

#ifdef CORELIB_WIN32
		if( this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket( this->handle());
			this->handle( ACE_INVALID_HANDLE);
		}

#else
		if( this->get_handle() != ACE_INVALID_HANDLE)
			stream_.close();

#endif

	}
	else
	{
		initialized_ =false;

#ifdef CORELIB_WIN32
		if( this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket( this->handle());
			this->handle( ACE_INVALID_HANDLE);
		}

#else
		if( this->get_handle() != ACE_INVALID_HANDLE)
			stream_.close();

#endif
	}

}

#ifdef CORELIB_WIN32

void BasicService::open( ACE_HANDLE new_handle, ACE_Message_Block &message_block)
{
	this->handle( new_handle);

	if( this->reader_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0 ||
		this->writer_.open( *this, ACE_INVALID_HANDLE, 0, proactor()) != 0)
	{
		MODULE_LOG_DEBUG( MODULE_TEMP, "open connect socket failed");
		delete this;
		return;
	}
	else
	{
		//设置连接状态
		set_initalized();

		//开始一个服务注册
		BasicModule::base_instance_->begin_registservice( this);

		//开始接收数据
		if( reader_.read( read_message_, read_message_.space()) != 0)
		{
			this->conn_error();
			read_message_.reset();
		}
		else
			begin_registservice_2();
	}
}

#else

int BasicService::open()
{
	//设置连接状态
	set_initalized();

	BasicModule::base_instance_->begin_registservice( this);

	return reactor()->register_handler( this, ACE_Event_Handler::RWE_MASK);
}

#endif	//CORELIB_WIN32

#ifdef CORELIB_WIN32

void BasicService::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
	//防止多次处理
	if( !initialized_) return;

	//asynch read tcp/ip data package 
	ACE_Message_Block& mb =result.message_block();
	size_t trlen =result.bytes_transferred();
	if( !result.success() || trlen == 0)
	{
		this->conn_error();
		mb.reset();
		return;
	}
	else
	{
		if( qos_)
			qos_->read_ready( trlen);

		analy_package( trlen);

		//继续下一个包的读取
		if( initialized_ && reader_.read( read_message_, read_message_.space()>PROTOCOL_READ_SIZE?PROTOCOL_READ_SIZE:read_message_.space()) != 0)
		{
			this->conn_error();
			read_message_.reset();
		}
	}
}

void BasicService::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
	//防止多次处理
	if( !initialized_) return;

	ACE_Message_Block& mb2 =result.message_block();
	size_t trlen =result.bytes_transferred();

	if( !result.success() || trlen == 0)
	{
		this->conn_error();
		zero_write_status();
		return;
	}
	else
	{
		ACE_Guard<ACE_Thread_Mutex> mon( *write_mutex_);

		if( qos_)
			qos_->write_ready( trlen);

		//填充发送的数据
		fill_writebuff();

		if(  write_message_.length() > 0)
		{
			if( qos_)
				qos_->start_write();

			write_message_len_ =write_message_.length() > PROTOCOL_SEND_SIZE?PROTOCOL_SEND_SIZE:write_message_.length();

			if( this->writer_.write( write_message_, write_message_len_) != 0)
			{
				zero_write_status();
				this->conn_error_nomutex();
			}
		}
		else
			write_message_len_ =0;
	}
}

#else

int BasicService::handle_input( ACE_HANDLE handle)
{
	//防止多次处理
	if( !initialized_) return 0;

	size_t rsize =( read_message_.space()>PROTOCOL_READ_SIZE?PROTOCOL_READ_SIZE:read_message_.space());
	char *buf =read_message_.wr_ptr();
	int ret =stream_.recv( buf, rsize);

	if( ret == 0)
		return -1;

	if( ret == -1 && ACE_OS::last_error() != EWOULDBLOCK)
		return -1;
	else
	{
		if( qos_)
			qos_->read_ready( ret);

		//读取成功
		read_message_.wr_ptr( buf + ret);

		return analy_package( ret);
	}
}

int BasicService::handle_output (ACE_HANDLE handle)
{
	//防止多次处理
	if( !initialized_) return 0;

	//填充发送的数据
	fill_writebuff();

	if( write_message_.length() > 0)
	{
		if( qos_)
			qos_->start_write();

		write_message_len_ =write_message_.length()>PROTOCOL_SEND_SIZE?PROTOCOL_SEND_SIZE:write_message_.length();
		char *buf =write_message_.rd_ptr();

		size_t slen =stream_.send( buf, write_message_len_);
		if( slen == -1 && ACE_OS::last_error() != EWOULDBLOCK)
			return -1;
		else
		{
			write_message_.rd_ptr( buf + slen);
			write_message_len_ -=slen;
		}

		if( qos_ && slen >= 0)
			qos_->write_ready( slen);
	}

	if( write_queue_size() == 0 && write_message_len_ <= 0)
	{
		reactor()->remove_handler( this, ACE_Event_Handler::WRITE_MASK);
	}

	return 0;
}

int BasicService::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask msk)
{
	if( msk == ACE_Event_Handler::WRITE_MASK)
		return 0;

	zero_write_status();
	if( initialized_)
		this->conn_error();

	return 0;
}

bool BasicService::send_one_block( BasicProtocol* p)
{
	char *pbuf =( send_block_tmpbuf_ + sizeof( S_INT_32));

	int len2 =0;
	p->data( pbuf, len2);

	*((S_INT_32*)send_block_tmpbuf_) =(S_INT_32)len2;
	len2 += sizeof( S_INT_32);

	return stream_.send_n( send_block_tmpbuf_, len2) == len2;
}

BasicProtocol* BasicService::recv_one_block()
{
	BasicProtocol *ret =0;
	char *pbuf =&recv_block_tmpbuf_[0];

	S_INT_32 len =0;
	len =stream_.recv_n( pbuf, sizeof( S_INT_32));
	if( len == -1 || len != sizeof( S_INT_32))
		return ret;

	len =*((S_INT_32*)pbuf);
	if( len +sizeof( S_INT_32) > SINGLE_PROTOCOL_SIZE)
		return ret;

	pbuf =pbuf + sizeof( S_INT_32);
	S_INT_32 l =stream_.recv_n( pbuf, len);
	if( l == -1 || l != len)
		return ret;

	ret =pro_factory_->data_protocol( pbuf, len, ext_data_);

	return ret;
}

#endif	//CORELIB_WIN32

int BasicService::analy_package( size_t trlen)
{
	if( read_message_.length() == 0)
		return 0;

	//处理数据包
	if( read_message_.base() != read_message_.rd_ptr())
	{
#ifdef CORELIB_WIN32
		//问题链接
		this->conn_error();
		read_message_.reset();

#endif
		return -1;
	}

	int len =0;
	do
	{
		//缓存中的消息长度
		len =(int)read_message_.length();
		//数据地址
		char *pdata =read_message_.rd_ptr();

		//不足一个int 长度描述
		if( len < sizeof( S_INT_32))
			break;

		//数据的长度不包括 int长度的前导
		S_INT_32 len2 =*((S_INT_32*)pdata);
		if( len2 + sizeof( S_INT_32) > SINGLE_PROTOCOL_SIZE)
		{
#ifdef CORELIB_WIN32
			//问题链接
			this->conn_error();
			read_message_.reset();

#endif
			return -1;
		}

		//计算长度的时候要加上int长度的前导包
		len -= (len2 + sizeof( S_INT_32));
		if( len < 0)
			break;

		pdata =pdata + sizeof( S_INT_32);
		BasicProtocol *protocol =pro_factory_->data_protocol( pdata, len2, ext_data_, internals_);

		ACE_Auto_Ptr<BasicProtocol> p_pro( protocol);
		if( protocol == 0)
		{
#ifdef CORELIB_WIN32
			//问题链接
			this->conn_error();
			read_message_.reset();

#endif
			return -1;
		}
		else
		{
			//处理收到的协议
			if( !recv_msg( p_pro.release()))
			{
#ifdef CORELIB_WIN32
				this->conn_error();
				read_message_.reset();

#endif
				return -1;
			}
		}

		//移动读指针
		read_message_.rd_ptr( len2 + sizeof( S_INT_32));
	}
	while( len > 0);

	//规格内存
	read_message_.crunch();

	return 0;
}

inline 
void BasicService::fill_writebuff()
{
	//剩下未发送的数据
	write_message_.crunch();

	BasicProtocol* msg =0;
	char *pbuf =(char*)fill_writebuff_tmp_;

	S_INT_32 len =0;
	//有空间可以放数据
	while( (len=write_message_.space()) > 0 && write_queue_size())
	{
		msg =next_write_msg();
		ACE_Auto_Ptr<BasicProtocol> p_msg( msg);
		if( msg == 0) continue;

		S_INT_32 len2 =0;
		if( !msg->data( pbuf, SINGLE_PROTOCOL_SIZE, len2, ext_data_, internals_))
		{
			push_head_write_msg( p_msg.release());
			break;
		}

		if( len < len2)
		{
			push_head_write_msg( p_msg.release());
			break;
		}

		//真正发送前处理
		pre_send_protocol( msg);

		char *pdata =write_message_.wr_ptr();
		*((S_INT_32*)pdata) =len2;
		pdata =pdata + sizeof(S_INT_32);

		memcpy( pdata, fill_writebuff_tmp_, len2);

		pdata =pdata + len2;
		write_message_.wr_ptr( pdata);
	}
}

int BasicService::append_write_msg( BasicProtocol* msg)
{
	ACE_Auto_Ptr<BasicProtocol> p_msg( msg);

	//socket handle have closed
	if( !initialized_) return 0;

	ACE_Guard<ACE_Thread_Mutex> mon( *write_mutex_);

	if( !initialized_) return 0;

#ifdef CORELIB_WIN32
	if( write_queue_size() == 0 && write_message_len_ == 0)
	{
		//发送失败
		if( push_write_msg( p_msg.release()) != 0)
			return 1;

		//填充发送的数据
		fill_writebuff();

		if( write_message_.length() > 0)
		{
			if( qos_)
				qos_->start_write();

			write_message_len_ =write_message_.length()>PROTOCOL_SEND_SIZE?PROTOCOL_SEND_SIZE:write_message_.length();

			if( this->writer_.write( write_message_, write_message_len_) != 0)
			{
				zero_write_status();
				this->conn_error_nomutex();
			}
		}
	}
	else
	{
		//发送失败
		if( push_write_msg( p_msg.release()) != 0)
			return 1;
	}

#else
	if( push_write_msg( p_msg.release()) != 0)
		return 1;

	if( write_queue_size() == 1)
	{
		reactor()->register_handler( this, ACE_Event_Handler::WRITE_MASK);
	}

#endif

	return 0;
}

inline void BasicService::zero_write_status()
{
	write_message_.reset();
	write_message_len_ =0;
}

void BasicService::conn_error()
{
#ifdef CORELIB_WIN32
#else
	reactor()->remove_handler( this, ACE_Event_Handler::RWE_MASK|ACE_Event_Handler::DONT_CALL);
#endif

	close_service();

	SystemCommand<BasicService> *pcmd =
		TASKCMD_NEW SystemCommand<BasicService>( boost::bind( &BasicModule::disconnect_service, BasicModule::base_instance_, this));

	BasicModule::base_instance_->regist_syscmd( pcmd);
}

void BasicService::conn_error_nomutex()
{
#ifdef CORELIB_WIN32
#else
	reactor()->remove_handler( this, ACE_Event_Handler::RWE_MASK|ACE_Event_Handler::DONT_CALL);
#endif

	close_service( false);
}

BasicProtocol* BasicService::next_write_msg()
{
	if( protocol_queue_.size() == 0)
		return 0;

	BasicProtocol *ret =protocol_queue_.front();
	protocol_queue_.pop_front();

	return ret;
}

void BasicService::push_head_write_msg( BasicProtocol* msg)
{
	protocol_queue_.push_front( msg);
}

int BasicService::push_write_msg( BasicProtocol* msg)
{
	protocol_queue_.push_back( msg);

	return 0;
}

int BasicService::write_queue_size()
{
	return protocol_queue_.size();
}
