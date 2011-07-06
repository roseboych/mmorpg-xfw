/**
* protocol module
*
* 定义了所有的聊天协议
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PRO_CHAT__H__
#define __PRO_CHAT__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "prolib/core_type.h"
#include "prolib/pre.h"
#include "prolib/BasicProtocol.h"
#include "prolib/SerializeFactoryContainer.h"

#include <list>

#define MESSAGE_LEN_MAX		256

PROTOCOL_NAMESPACE_BEGIN

/**
* @class Pro_TeamChat_req
* 
* @brief 群聊天请求
**/
class Pro_TeamChat_req : public AppProtocol<Pro_TeamChat_req> 
{ 
	typedef AppProtocol<Pro_TeamChat_req> inherit;

public:
	Pro_TeamChat_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, chattype_)
		LOAD_STR_PRO( pdata, totlen, msg_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, chattype_)
		SAVE_STR_PRO( pdata, buflen, len, msg_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_TeamChat_req, proo)
		CLONE_VAR_PRO(proo, chattype_)
		CLONE_VAR_PRO(proo, msg_)
	END_CLONE_PRO()

public:
	//0:地图内聊天 1:aoi聊天 2:工会频道聊天 3:好友频道聊天 4:小队频道聊天
	S_INT_8		chattype_;
	// 消息
	StaticString<MESSAGE_LEN_MAX>	msg_;
};

/**
* @class Pro_TeamChat_ntf
* 
* @brief 群聊天广播
**/
class Pro_TeamChat_ntf : public AppProtocol<Pro_TeamChat_ntf> 
{ 
	typedef AppProtocol<Pro_TeamChat_ntf> inherit;

public:
	Pro_TeamChat_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, chattype_)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_STR_PRO( pdata, totlen, nick_)
		LOAD_STR_PRO( pdata, totlen, msg_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, chattype_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_STR_PRO( pdata, buflen, len, nick_)
		SAVE_STR_PRO( pdata, buflen, len, msg_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_TeamChat_ntf, proo)
		CLONE_VAR_PRO(proo, chattype_)
		CLONE_VAR_PRO(proo, chrid_)
		CLONE_VAR_PRO(proo, nick_)
		CLONE_VAR_PRO(proo, msg_)
	END_CLONE_PRO()

public:
	//0:地图内聊天 1:aoi聊天 2:工会频道聊天 3:好友频道聊天 4:小队频道聊天
	S_INT_8		chattype_;
	// 角色id
	S_INT_32	chrid_;
	// 角色名称
	StaticString<CHR_NICKNAME_LEN>	nick_;
	// 消息
	StaticString<MESSAGE_LEN_MAX>	msg_;
};

/**
* @class Pro_P2PChat_req
* 
* @brief 私聊请求
**/
class Pro_P2PChat_req : public AppProtocol<Pro_P2PChat_req> 
{ 
	typedef AppProtocol<Pro_P2PChat_req> inherit;

public:
	Pro_P2PChat_req();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, chattype_)
		LOAD_INT32_PRO( pdata, totlen, target_chrid_)
		LOAD_STR_PRO( pdata, totlen, msg_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, chattype_)
		SAVE_INT32_PRO( pdata, buflen, len, target_chrid_)
		SAVE_STR_PRO( pdata, buflen, len, msg_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_P2PChat_req, proo)
		CLONE_VAR_PRO(proo, chattype_)
		CLONE_VAR_PRO(proo, target_chrid_)
		CLONE_VAR_PRO(proo, msg_)
	END_CLONE_PRO()

public:
	//0:工会频道聊天 1:好友频道聊天 2:小队频道聊天
	S_INT_8		chattype_;
	//聊天对象
	S_INT_32	target_chrid_;
	// 消息
	StaticString<MESSAGE_LEN_MAX>	msg_;
};

/**
* @class Pro_P2PChat_ntf
* 
* @brief 群聊天广播
**/
class Pro_P2PChat_ntf : public AppProtocol<Pro_P2PChat_ntf> 
{ 
	typedef AppProtocol<Pro_P2PChat_ntf> inherit;

public:
	Pro_P2PChat_ntf();

protected:
	BEGIN_LOAD_PRO( pdata, totlen, ext)
		LOAD_INT8_PRO( pdata, totlen, chattype_)
		LOAD_INT32_PRO( pdata, totlen, chrid_)
		LOAD_STR_PRO( pdata, totlen, nick_)
		LOAD_STR_PRO( pdata, totlen, msg_)
	END_LOAD_PRO()

	BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		SAVE_INT8_PRO( pdata, buflen, len, chattype_)
		SAVE_INT32_PRO( pdata, buflen, len, chrid_)
		SAVE_STR_PRO( pdata, buflen, len, nick_)
		SAVE_STR_PRO( pdata, buflen, len, msg_)
	END_SAVE_PRO()

	BEGIN_CLONE_PRO( Pro_P2PChat_ntf, proo)
		CLONE_VAR_PRO(proo, chattype_)
		CLONE_VAR_PRO(proo, chrid_)
		CLONE_VAR_PRO(proo, nick_)
		CLONE_VAR_PRO(proo, msg_)
	END_CLONE_PRO()

public:
	//0:工会频道聊天 1:好友频道聊天 2:小队频道聊天
	S_INT_8		chattype_;
	// 角色id
	S_INT_32	chrid_;
	// 角色名称
	StaticString<CHR_NICKNAME_LEN>	nick_;
	// 消息
	StaticString<MESSAGE_LEN_MAX>	msg_;
};

PROTOCOL_NAMESPACE_END

#endif	//__PRO_CHAT__H__
