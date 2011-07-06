/**
* lgs application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include "LGSModule.h"

Player::Player():
#ifdef CORELIB_WIN32
service_( 0)
#else
service_( 0)
#endif
{
	service_.player_ =this;

	status_ =PLAYERSTATUS_NONE;
	have_force_close_ =false;
}

void Player::reset()
{
	service_.reset();

	status_ =PLAYERSTATUS_NONE;
	uuid_.reset();
	login_fail_nums_ =0;
}

void Player::prepare()
{
	//状态变化
	status_ =PLAYERSTATUS_WAITLOGIN;
	//开始计时时间戳
	tstamp_ =ACE_OS::gettimeofday().get_msec();

	login_fail_nums_ =0;
	uuid_.reset();
	uuid_.set_timestamp( tstamp_);

	have_force_close_ =false;
}

bool Player::can_dologin()
{
	if( status_ != PLAYERSTATUS_WAITLOGIN)
		return false;
	//登陆失败超过3次
	if( login_fail_nums_ >= 3)
		return false;

	return true;
}

bool Player::can_doteamreconn()
{
	return status_ == PLAYERSTATUS_WAITLOGIN;
}

void Player::login_begin()
{
	status_ =PLAYERSTATUS_LOGINING;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
}

void Player::login_end( PRO::Pro_Login_ack* ack)
{
	if( ack->result_ == 0)
	{
		uuid_.set_userid( ack->userid_);
		login_fail_nums_ =0;
		tstamp_ =ACE_OS::gettimeofday().get_msec();
		tstamp2_ =tstamp_;

		if( ack->is_lnkkeep_)
		{
			status_ =PLAYERSTATUS_LNKKEEP;
			//如果是断线保持，那么需要更新session
			uuid_.set_timestamp( ack->session_);
		}
		else
		{
			status_ =PLAYERSTATUS_LOGINS;
		}
	}
	else
	{
		status_ =PLAYERSTATUS_WAITLOGIN;
		++login_fail_nums_;
		tstamp_ =ACE_OS::gettimeofday().get_msec();
	}
}

void Player::selteam_begin()
{
	if( status_ != PLAYERSTATUS_LOGINS)
		return;

	status_ =PLAYERSTATUS_WAITSELTEAM;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
}

void Player::selteam_end( bool succ)
{
	if( status_ != PLAYERSTATUS_WAITSELTEAM)
		return;

	if( succ)
	{
		//等待登陆认证
		status_ =PLAYERSTATUS_LOGINTEAM;
		tstamp_ =ACE_OS::gettimeofday().get_msec();
	}
	else
	{
		//退回登陆状态
		status_ =PLAYERSTATUS_LOGINS;
		tstamp_ =ACE_OS::gettimeofday().get_msec();
		tstamp2_ =tstamp_;
	}
}

void Player::selteam_confirm()
{
	if( status_ != PLAYERSTATUS_LOGINTEAM)
		return;

	status_ =PLAYERSTATUS_LOGINTEAMS;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
}

void Player::selteam_timeout()
{
	status_ =PLAYERSTATUS_LOGINS;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
	tstamp2_ =tstamp_;
}

void Player::quitteam_reconn_begin()
{
	status_ =PLAYERSTATUS_QUITTEAMRECONN;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
}

void Player::quitteam_reconn( S_INT_32 uid)
{
	uuid_.set_userid( uid);

	status_ =PLAYERSTATUS_LOGINS;
	login_fail_nums_ =0;
	tstamp_ =ACE_OS::gettimeofday().get_msec();
	tstamp2_ =tstamp_;
}

void Player::main_tick( ACE_UINT64& t)
{
	if( status_ == PLAYERSTATUS_NONE || have_force_close_)
		return;

	bool willreset =false;
	if( login_fail_nums_ >= 3)
		willreset =true;
	if( status_ == PLAYERSTATUS_LOGINTEAMS)
	{
		//检查是否登陆服务器组结束
		willreset =true;
	}
	else if( status_ == PLAYERSTATUS_WAITLOGIN)
	{
		//超过10s没有收到登陆请求
		if( tstamp_ + 10*1000 < t)
			willreset =true;
	}
	else if( status_ == PLAYERSTATUS_LOGINING)
	{
		//超过10s服务器没有响应
		if( tstamp_ + 10*1000 < t)
			willreset =true;
	}
	else if( status_ == PLAYERSTATUS_LOGINS)
	{
		//超过5m没有选择服务器组
		if( tstamp_ + 300*1000 < t)
			willreset =true;
		else if( tstamp2_ + 8*1000 < t)
		{
			//8s发送一次
			tstamp2_ =t;
			notify_svrteaminfo();
		}
	}
	else if( status_ == PLAYERSTATUS_WAITSELTEAM)
	{
		//超过10s服务器没有响应
		if( tstamp_ + 1000*1000 < t)
			willreset =true;
	}
	else if( status_ == PLAYERSTATUS_LOGINTEAM)
	{
		//超过10s没有登陆gts成功
		if( tstamp_ + 1500*1000 < t)
			willreset =true;
	}
	else if( status_ == PLAYERSTATUS_WAITLOGIN)
	{
		//超过20s没有完成认证
		if( tstamp_ + 20*1000)
			willreset =true;
	}
	else if( status_ == PLAYERSTATUS_LNKKEEP)
	{
		//超过20s没有完成认证
		if( tstamp_ + 20*1000)
			willreset =true;
	}

	if( willreset)
	{
		have_force_close_ =true;
		service_.conn_error();
		return;
	}

	//网络激活心跳
	service_.online_tickit( t);
}

void Player::notify_svrteaminfo()
{
	send_protocol( LGSMODULE->construct_svrteam_notify());
}