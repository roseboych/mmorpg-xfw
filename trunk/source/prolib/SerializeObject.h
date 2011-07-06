/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SERIALIZEOBJECT__H__
#define __SERIALIZEOBJECT__H__

#include "prolib/prolibdef.h"
#include "prolib/core_type.h"
#include "prolib/pro_macro.h"

/**
* @class SerializeObject
* 
* @brief 通讯协议支持的对象基类
**/
class SerializeObject
{
public:
	SerializeObject( S_INT_8 fctype, S_INT_16 siid);
	~SerializeObject();

	/**
	* 克隆一个相同数据的协议包
	* @return 
	**/
	virtual SerializeObject* clone() =0;

	/**
	* 由byte数据构造协议对象，data数据protocol不负责释放
	* @param pdata		协议数据块开始地址
	* @param len		数据块长度
	* @param ext		扩展信息,保留
	*
	* @return 
	**/
	bool format( char** data, S_INT_32& len, void* ext =0);

	/**
	* 把协议序列化到buf中
	* @param buf	协议序列化首地址
	* @param buflen	buf的总长度
	* @param len	当前处理的长度
	* @param ext	保留
	*
	* @return 
	**/
	bool data( char** buf, S_INT_32 buflen, S_INT_32& len, void* ext =0);

protected:
	/**
	* 从网络数据构造协议
	* @param data
	* @param totlen
	* @param ext
	*
	* @return
	**/
	virtual bool to_protocol( char** data, S_INT_32& totlen, void* ext=0) =0;

	/**
	* 从协议构造数据块
	* @param data
	* @param buflen
	* @param len
	* @param ext
	*
	* @return 
	**/
	virtual bool to_buffer( char** data, S_INT_32 buflen, S_INT_32&len, void* ext =0) =0;

	/**
	* 克隆协议
	* @oaram pro2
	**/
	virtual void clone_pro( SerializeObject* pro2) =0;

public:
	//所在的factory类型
	S_INT_8		factory_;
	//序列化对象的编号，仅在factory中唯一
	S_INT_16	serialize_obj_iid_;
};

template<class T_>
class AppSerializeObject : public SerializeObject
{
public:
	AppSerializeObject( S_INT_8 fctype, S_INT_16 siid);
	virtual ~AppSerializeObject();

	virtual SerializeObject* clone();
};

template<class T_>
AppSerializeObject<T_>::AppSerializeObject( S_INT_8 fctype, S_INT_16 siid):
SerializeObject( fctype, siid)
{
}

template<class T_>
AppSerializeObject<T_>::~AppSerializeObject()
{
}

template<class T_>
SerializeObject* AppSerializeObject<T_>::clone()
{
	T_ *ret =PRO_NEW_EX T_();

	//factory_
	CLONE_VAR_PRO( ret, factory_);
	//serialize_obj_iid_
	CLONE_VAR_PRO( ret, serialize_obj_iid_);

	//用户扩展协议
	clone_pro( ret);

	return (SerializeObject*)ret;
}

#endif	//__SERIALIZEOBJECT__H__
