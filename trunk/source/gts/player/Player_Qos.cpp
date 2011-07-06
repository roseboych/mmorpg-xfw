/**
* gts application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player_Qos.h"

#include <ace/Numeric_Limits.h>
#include <ace/OS_NS_sys_time.h>

Player_Qos::Player_Qos(void)
:qos_quality_( NO_INITVALUE),
use_time_( 0),
write_bytes_( 0),
start_qos_( false)
{
	read_time_last_ =ACE_OS::gettimeofday();
	start_time_ =read_time_last_;
}

Player_Qos::~Player_Qos(void)
{
}

void Player_Qos::start_write()
{
	start_qos_ =true;
	start_time_ =ACE_OS::gettimeofday();
}

void Player_Qos::write_ready( size_t len)
{
	if( !start_qos_)
		return;

	//累计发送字节数
	write_bytes_ += len;

	//累计发送时间
	ACE_Time_Value now =ACE_OS::gettimeofday();
	now -= start_time_;
	use_time_ += now.msec();

	//满足统计要求
	if( write_bytes_ >= CALCUATE_BYTES_WRITE)
	{
		ACE_UINT64 tv =use_time_*1024 / write_bytes_;
		if( tv > ACE_Numeric_Limits<signed int>::max())
			qos_quality_ =ACE_Numeric_Limits<signed int>::max();
		else
			qos_quality_ =(int)tv;

		use_time_ =0;
		write_bytes_ =0;
	}
	else if( use_time_ > 10000 && write_bytes_)
	{
		//如果超过10s还没有完成1k数据的发送也要进行计算
		ACE_UINT64 tv =use_time_*1024 / write_bytes_;
		if( tv > ACE_Numeric_Limits<signed int>::max())
			qos_quality_ =ACE_Numeric_Limits<signed int>::max();
		else
			qos_quality_ =(int)tv;
	}

	start_qos_ =false;
}

void Player_Qos::read_ready( size_t len)
{
	read_time_last_ =ACE_OS::gettimeofday();
}

int Player_Qos::qos()
{
	return qos_quality_;
}

ACE_UINT64 Player_Qos::last_time()
{
	return start_time_ > read_time_last_ ? start_time_.get_msec():read_time_last_.get_msec();
}
