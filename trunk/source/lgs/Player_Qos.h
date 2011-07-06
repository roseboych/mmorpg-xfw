/**
* lgs application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PLAYER_QOS__H___
#define __PLAYER_QOS__H___

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "lgs_pre.h"

#include <ace/Time_Value.h>
#include <corelib/net/Basic_Qos.h>

//qos计算阀值
#define CALCUATE_BYTES_WRITE	1024	//1k

/**
* @class Player_Qos
* 
* @brief lgs服务器用来计算和用户的tcp通讯质量
* 
**/
class Player_Qos : public Basic_Qos
{
public:
	Player_Qos(void);
	virtual ~Player_Qos(void);

	virtual void start_write();
	virtual void write_ready( size_t len);
	virtual void read_ready( size_t len);

	virtual int qos();

	virtual ACE_UINT64 last_time();
private:
	//最后一次读的时间
	ACE_Time_Value	read_time_last_;
	//最后一次写开始计时时间
	ACE_Time_Value	start_time_;
	//花费的时间
	ACE_UINT64		use_time_;
	//已写字节数
	S_INT_32		write_bytes_;
	//qos值，如果为NO_INITVALUE,表示还未计算过.单位：毫秒/kbytes
	int qos_quality_;

	bool	start_qos_;
};

#endif	//__PLAYER_QOS__H___
