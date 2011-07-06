/**
* css application
*
* @category		core
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CSSSVR__H__
#define __CSSSVR__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "css_pre.h"

#include <ace/Thread_Mutex.h>
#include <ace/Auto_Ptr.h>

#include "CSS_Service.h"
#include "CSS2_Service.h"
#include "CTS_Service.h"
#include "DPX_Service.h"
#include "CTSLink.h"
#include "DPXLink.h"
#include "GTSLink.h"
#include "CSSLinkWrap.h"
#include "CSS2Link.h"
#include "CSSLink.h"
#include "AdapterPlayer.h"
#include "ContentServiceThread.h"
#include "BaseStoryService.h"

class CSSoption;
class InstanceStoryServiceImpl;
class MainStoryServiceImpl;

/**
* @class CSSSvr
* 
* @brief 模块用到的所有数据。协议解析等
* 
**/
class CSSSvr
{
	typedef NS_STL::list<BaseStoryService*>	STORYSERVICE_LIST;

public:
	CSSSvr(void);
	virtual ~CSSSvr(void);

public:
	/**
	* 完成服务器链接注册后的回调
	* @param s
	* @param autorelease
	**/
	void fin_regist_service( BasicService* s, bool& autorelease);

	/**
	* 服务断线回调
	* @param s
	**/
	void service_disconnected( BasicService* s);

	virtual void post_exit_cmd() =0;

	virtual ACE_Thread_Mutex& get_mutex() =0;

public:
	/**
	* 初始化服务内容信息
	**/
	void init_svr();
	void uninit_svr();

	inline CTSLink* get_ctslink();
	inline DPXLink* get_dpxlink();
	inline GTSLink* get_gtslink( int ind);
	inline GTSLink* get_gtslinkbyuserglobal( int gid);
	inline AdapterPlayer* get_adapterplayer( int ind);

	/**
	* 获取自身的配置
	* @return
	**/
	CSSOption* get_owneropt();

	/**
	* 从执行队列中获取一个需要处理的内容服务
	**/
	BaseStoryService* pop_storyservice();
	void push_storyservice( BaseStoryService* svr);

	/**
	* 根据地图id获取对应的服务
	* @param mapid
	* @return
	**/
	BaseStoryService* get_storyservicebymap( S_INT_32 mapid);

	/**
	* 发送到rgs
	* @param p	使用人分配，调用函数负责释放
	**/
	void send_to_cts( BasicProtocol* p);
	void send_to_dpx( BasicProtocol* p);
	void broadcast_to_gts( BasicProtocol* p);

protected:
	//close socket but don't release
	void close_all_service();

	/**
	* 通知副本实例开始初始化控制器
	**/
	void notify_instinitctrl();

protected:
	//可以连接的cts
	CTSLink	ctssvr_;

	//可以连接的dpx
	DPXLink	dpxsvr_;

	//可以被连接的gts
	ACE_Auto_Array_Ptr<GTSLink> gtssvr_;
	S_INT_32	gtssvr_nums_;

	//css连接的封装
	CSSLinkWrap	csssvr_;

	//css支持的最大玩家, adapterplayer用来派发协议到对应的contentservice
	ACE_Auto_Array_Ptr<AdapterPlayer>	players_;
	S_INT_32							player_nums_;
	//访问玩家列表用到的锁
	ACE_Thread_Mutex					player_mutex_;

	//内容处理线程池
	ContentServiceThread	dothreads_;

	//支持的主游戏模式
	ACE_Auto_Array_Ptr<MainStoryServiceImpl>		mainstorys_;
	S_INT_32										mainstory_nums_;
	//支持的副本实例
	ACE_Auto_Array_Ptr<InstanceStoryServiceImpl>	inststorys_;
	S_INT_32										inststory_nums_;

	//需要处理的服务内容
	STORYSERVICE_LIST	dosvrs_;
	ACE_Thread_Mutex	svr_mutex_;

	//---------------------------系统维护实现--------------------------------//
public:
	//断线时清除数据
	void svr_ctsdisconnect();
	void svr_dpxdisconnect();
	void svr_gtsdisconnect( GTSLink* l);
	void svr_cssdisconnect( CSSLink* l);
	void svr_css2disconnect( CSS2Link* l);

	//----------------------------协议处理实现--------------------------------//
	//controller
	void controller_profiler_filterctrl( BasicProtocol* p, bool& autorelease);
	void controller_profiler_analysisctrl( BasicProtocol* p, bool& autorelease);

	void gts_netadapter( BasicProtocol* p, bool& autorelease);
	void dpx_netadapter( BasicProtocol* p, bool& autorelease);
	void cts_netadapter( BasicProtocol* p, bool& autorelease);
	void css_netadapter( BasicProtocol* p, bool& autorelease);
};

inline 
CTSLink* CSSSvr::get_ctslink()
{
	return &ctssvr_;
}

inline 
DPXLink* CSSSvr::get_dpxlink()
{
	return &dpxsvr_;
}

inline 
GTSLink* CSSSvr::get_gtslink( int ind)
{
	if( ind < 0 || ind >= gtssvr_nums_)
		return 0;
	return &(gtssvr_[ind]);
}

inline 
GTSLink* CSSSvr::get_gtslinkbyuserglobal( int gid)
{
	for( int ii =0; ii < gtssvr_nums_; ++ii)
	{
		if( gtssvr_[ii].opt_->is_userglobalin( gid))
			return &(gtssvr_[ii]);
	}

	return 0;
}

inline 
AdapterPlayer* CSSSvr::get_adapterplayer( int ind)
{
	if( ind < 0 || ind >= player_nums_)
		return 0;
	return &(players_[ind]);
}

#endif //v
