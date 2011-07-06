/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICPROTOCOL__H__
#define __BASICPROTOCOL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/prolibdef.h"

#include "prolib/core_type.h"

#include "prolib/StaticString.h"

#include "prolib/pro_macro.h"
#include "prolib/SerializeObject.h"
#include "prolib/SerializeFactoryContainer.h"

/**
* @class BasicProtocol
* 
* @brief 通讯协议类基类
**/
class BasicProtocol
{
public:
	struct internal_uuid
	{
		internal_uuid():global_index_(NO_INITVALUE),userid_(NO_INITVALUE),init_stmp_(0){}

		//玩家全局索引
		S_INT_32		global_index_;
		//玩家id+登陆时的时间戳
		S_INT_32		userid_;
		S_TIMESTAMP		init_stmp_;
	};

public:
	BasicProtocol( S_INT_32 id);
	virtual ~BasicProtocol();

public:
	S_INT_32 get_uuidglobalindex(){
		return uuid_.global_index_;
	}

	void set_uuidglobalindex( S_INT_32 s){
		uuid_.global_index_ =s;
	}

	S_INT_32 get_uuiduserid(){
		return uuid_.userid_;
	}
	void set_uuiduserid( S_INT_32 s){
		uuid_.userid_ =s;
	}

	S_TIMESTAMP get_uuidinitstmp(){
		return uuid_.init_stmp_;
	}
	void set_uuidinitstmp( S_TIMESTAMP s){
		uuid_.init_stmp_ =s;
	}

	/**
	* 检查是否同一个session
	**/
	bool same_session( S_INT_32 rid, S_TIMESTAMP t){
		return (rid == uuid_.userid_ && t == uuid_.init_stmp_);
	}
	bool same_session( uuid_session& u){
		return uuid_.userid_ == u.userid_ && uuid_.init_stmp_ == u.init_timestamp_;
	}

public:
	/**
	* 克隆一个相同数据的协议包
	* @return 
	**/
	virtual BasicProtocol* clone() =0;

	/**
	* 由byte数据构造协议对象，data数据protocol不负责释放
	* @param pdata		协议数据块开始地址
	* @param len		数据块长度
	* @param ext		扩展信息,保留
	* @param internals	true:构造的是来自服务器组内部的协议 false:构造的是来自客户端的协议
	*
	* @return 
	**/
	bool format( char* data, S_INT_32 len, void* ext =0, bool internals =true);

	/**
	* 把协议序列化到buf中
	* @param buf	协议序列化首地址
	* @param buflen	buf的总长度
	* @param len	当前处理的长度
	* @param ext	保留
	* @param internals	true:构造的是来自服务器组内部的协议 false:构造的是来自客户端的协议
	*
	* @return 
	**/
	bool data( char* buf, S_INT_32 buflen, S_INT_32& len, void* ext =0, bool internals =true);

protected:
	/**
	* 从网络数据构造协议
	* @param data
	* @param totlen
	* @param ext
	*
	* @return
	**/
	virtual bool to_protocol( char* data, S_INT_32& totlen, void* ext=0) =0;

	/**
	* 从协议构造数据块
	* @param data
	* @param buflen
	* @param len
	* @param ext
	*
	* @return 
	**/
	virtual bool to_buffer( char* data, S_INT_32 buflen, S_INT_32&len, void* ext =0) =0;

	/**
	* 克隆协议
	* @oaram pro2
	**/
	virtual void clone_pro( BasicProtocol* pro2) =0;

public:
	//协议编号
	S_INT_32		iid_;
	internal_uuid	uuid_;
};

template<class T_>
class AppProtocol : public BasicProtocol
{
public:
	AppProtocol( S_INT_32 id);
	virtual ~AppProtocol();

	virtual BasicProtocol* clone();
};

template<class T_>
AppProtocol<T_>::AppProtocol( S_INT_32 id):BasicProtocol(id)
{
}

template<class T_>
AppProtocol<T_>::~AppProtocol()
{
}

template<class T_>
BasicProtocol* AppProtocol<T_>::clone()
{
	T_ *ret =PRO_NEW_EX T_();

	//user_index_
	CLONE_VAR_PRO( ret, uuid_.global_index_);
	//roleid_
	CLONE_VAR_PRO( ret, uuid_.userid_);
	//rolestmp_
	CLONE_VAR_PRO( ret, uuid_.init_stmp_);

	//用户扩展协议
	clone_pro( ret);

	return (BasicProtocol*)ret;
}

#endif //__BASICPROTOCOL__H__

