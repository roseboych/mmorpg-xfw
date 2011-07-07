/**
* css application
*
* @category		transcript service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __INSTANCESTORYSERVICEIMPL__H__
#define __INSTANCESTORYSERVICEIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"
#include <reslib/server/service_define.h>
#include "../BaseStoryService.h"
#include "InstCtrlBase.h"

class INSCellOption;

/**
* @class InstanceStoryServiceImpl
* 
* @brief 副本场景服务实现
**/
class InstanceStoryServiceImpl : public BaseStoryService
{
public:
	typedef BaseStoryService inherit;
	typedef NS_STL::map< S_INT_32, Player*>	ONLINEPLAYER_MAP;

public:
	InstanceStoryServiceImpl();
	virtual ~InstanceStoryServiceImpl();

	virtual bool	is_mainstoryservice(){ return false;}
	virtual bool	is_instancestoryservice(){ return true;}

	bool			init_service( StoryMapOption* opt, INSCellOption* cellopt);
	virtual bool	init_service( StoryMapOption* opt);
	virtual void	uninit_service();

	virtual Player* get_player( const BasicProtocol::internal_uuid& uuid);
	virtual Player* player_regist( const BasicProtocol::internal_uuid& uuid, S_INT_32 chrid);
	virtual void	player_unregist( const BasicProtocol::internal_uuid& uuid);
	virtual void	player_unregist( Player* user);

	/**
	* 初始化副本服务
	* 在css收到cts注册成功之后调用
	* @param 保留
	**/
	void			init_instservice( void*);

	void					set_serverstate( TRANSCRIPT_SERVER_STATE ns);
	TRANSCRIPT_SERVER_STATE	get_serverstate(){ return svr_state_;}

	S_INT_32		get_cellid();

protected:
	/**
	* 状态监测定时器
	* @param tnow	时间
	**/
	void transcript_state_ticker( ACE_UINT64 tnow);

protected:
	//副本状态控制
	InstCtrlBase*				state_ctrl_;
	//副本状态
	TRANSCRIPT_SERVER_STATE		svr_state_;

	//在线用户列表
	ONLINEPLAYER_MAP	players_;

	//副本服务器配置
	INSCellOption*		inscell_opt_;

public:
	//--------------------------------------协议处理--------------------------------------
	//进入副本
	virtual void gts_instenter_req( BasicProtocol* p, bool& autorelease);
	virtual void cts_instenter_req( BasicProtocol* p, bool& autorelease);
	virtual void cts_instenter_ack( BasicProtocol* p, bool& autorelease);
	virtual void cts_enterinstovertime_ntf( BasicProtocol* p, bool& autorelease);
	virtual void gts_instenterconfirm_ntf( BasicProtocol* p, bool& autorelease);
	virtual void gts_instquit_req( BasicProtocol* p, bool& autorelease);
};

#endif	//__INSTANCESTORYSERVICEIMPL__H__
