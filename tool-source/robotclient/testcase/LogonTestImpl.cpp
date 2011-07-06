#include "StdAfx.h"

#include "LogonTestImpl.h"

#include "TestCase.h"

#include "logmacro.h"

USE_PROTOCOL_NAMESPACE

LogonTestImpl::LogonTestImpl( TestCase *p)
:BasicImpl( p)
{
}

LogonTestImpl::~LogonTestImpl(void)
{
}

void LogonTestImpl::do_lgs( BasicProtocol *p)
{
	ULONGLONG losttime =GetMillisecondTime() - record_time_;

	switch( p->iid_)
	{
	case GAME_LOGIN_ACK:
		{
			this->recv_logonack( p);

			Pro_Login_ack* ack =dynamic_cast<Pro_Login_ack*>( p);

			if( islogon())
			{
				test_state_ =TS_LOGON_SUCCEED;
				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%s) $%d$ login success", p->iid_, (LPCTSTR)user_name_,  losttime));
			}
			else
			{
				test_state_ =TS_LOGON_FAILED;
				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%s) $%d$ login failed",  p->iid_, (LPCTSTR)user_name_, losttime));

				reconn_time_ =GetMillisecondTime();
			}

			record_time_ =GetMillisecondTime();
		}

		break;
	case SVR_SVRTEAMINFO_NTF:
		{
			Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>(p);

			test_state_ =TS_SVRTEAM_SELECTING;

			reconn_time_ =GetMillisecondTime();
		}

		break;
	}
}

void LogonTestImpl::do_gts( BasicProtocol *p)
{
	ULONGLONG losttime =GetMillisecondTime() - record_time_;
}

void LogonTestImpl::status_check( ULONGLONG now)
{
	switch( test_state_)
	{
	case TS_LOGON_FAILED:
		if( reconn_time_ + 1000 < now)
			reconnect_2_lgs();

		break;
	case TS_SVRTEAM_SELECTING:
		if( reconn_time_ + 1000 < now)
			reconnect_2_lgs();

		break;
	}
}

