/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MAINSTORYSERVICEIMPL__H__
#define __MAINSTORYSERVICEIMPL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"

#include <ace/Auto_Ptr.h>

#include "../BaseStoryService.h"

/**
* @class MainStoryServiceImpl
* 
* @brief 主场景服务实现
**/
class MainStoryServiceImpl : public BaseStoryService
{
	typedef BaseStoryService inherit;
public:
	MainStoryServiceImpl();
	virtual ~MainStoryServiceImpl();

	virtual bool is_mainstoryservice(){ return true;}
	virtual bool is_instancestoryservice(){ return false;}

	virtual bool init_service( StoryMapOption* opt);
	virtual void uninit_service();

	virtual Player* get_player( const BasicProtocol::internal_uuid& uuid);
	virtual Player* player_regist( const BasicProtocol::internal_uuid& uuid, S_INT_32 chrid);
	virtual void	player_unregist( const BasicProtocol::internal_uuid& uuid);
	virtual void	player_unregist( Player* user);

public:
	//支持的最大玩家
	ACE_Auto_Array_Ptr<Player>	players_;
	S_INT_32					player_nums_;

public:
	//--------------------------------------协议处理--------------------------------------
	//进入副本
	virtual void gts_instenter_req( BasicProtocol* p, bool& autorelease);
	virtual void cts_instenter_ack( BasicProtocol* p, bool& autorelease);
	virtual void cts_enterinstovertime_ntf( BasicProtocol* p, bool& autorelease);
	virtual void gts_instenterconfirm_ntf( BasicProtocol* p, bool& autorelease);
};

#endif	//__MAINSTORYSERVICEIMPL__H__
