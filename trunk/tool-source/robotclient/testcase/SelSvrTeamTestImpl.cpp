#include "StdAfx.h"

#include "SelSvrTeamTestImpl.h"

#include "../TestCase.h"
#include "../logmacro.h"

USE_PROTOCOL_NAMESPACE

SelSvrTeamTestImpl::SelSvrTeamTestImpl( TestCase *p)
:BasicImpl( p),
reconn_userid_( NO_INITVALUE)
{
}

SelSvrTeamTestImpl::~SelSvrTeamTestImpl(void)
{
}

void SelSvrTeamTestImpl::logon()
{
	if( test_state_ != TS_NOLOGON)
		return;

	if( reconn_userid_ != NO_INITVALUE)
	{
		Pro_SvrQuitTeamReconn_req* req =new Pro_SvrQuitTeamReconn_req();
		req->userid_ =reconn_userid_;
		req->token_ =reconn_token_;

		pcase_->send_lgs_protocol( req);

		test_state_ = TS_RELOGONING;

		record_time_ =GetMillisecondTime();
	}
	else
	{
		BasicImpl::logon();
	}
}

void SelSvrTeamTestImpl::do_lgs( BasicProtocol *p)
{
	ULONGLONG losttime =GetMillisecondTime() - record_time_;

	switch( p->iid_)
	{
	case GAME_LOGIN_ACK:
		{
			this->recv_logonack( p);

			Pro_Login_ack* ack =dynamic_cast<Pro_Login_ack*>( p);

			if( islogon())
				test_state_ =TS_LOGON_SUCCEED;
			else
				test_state_ =TS_LOGON_FAILED;

			record_time_ =GetMillisecondTime();
		}

		break;
	case SVR_SVRTEAMINFO_NTF:
		{
			Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>(p);

			NS_STL::list<Pro_SvrTeamInfo_NTF::svrteaminfo>::iterator iter=ntf->teams_.begin(), eiter =ntf->teams_.end();
			for( ; iter != eiter; ++iter)
			{
				Pro_SvrTeamInfo_NTF::svrteaminfo& sinfo =(*iter);
				if( sinfo.is_validate_ == 0)
					continue;

				Pro_SvrSelTeam_req* req =new Pro_SvrSelTeam_req();
				req->team_index_ =sinfo.server_index_;

				pcase_->send_lgs_protocol( req);

				test_state_ =TS_SVRTEAM_SELECTING;

				record_time_ =GetMillisecondTime();

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ recv svrteam information and send select svrteam request",  p->iid_, userid_, losttime));

				break;
			}

		}

		break;
	case SVR_SELSERVER_ACK:
		{
			Pro_SvrSelTeam_ack* lack =dynamic_cast<Pro_SvrSelTeam_ack*>(p);
			if( lack->result_ == 0)
			{
				proxy_index_ =lack->proxy_index_;
				gts_ip_ =lack->gts_ip_.c_str();
				gts_port_ =lack->gts_port_;
				token_ =lack->token_;

				test_state_ =TS_SELSVRTEAM_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam success step(1)-SVR_SELSERVER_ACK",  p->iid_, userid_, losttime));
			}
			else
			{
				test_state_ =TS_SELSVRTEAM_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam failed step(1)-SVR_SELSERVER_ACK ret:%d",  
					p->iid_, userid_, losttime, lack->result_));
			}

		}

		break;
	case SVR_SELSERVERCONFIRM_ACK:
		{
			Pro_SvrSelTeamConfirm_ack* lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);

			if( lack->result_ != 0)
			{
				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam failed step(2)-SVR_SELSERVERCONFIRM_ACK",  p->iid_, userid_, losttime));

				test_state_ =TS_LOGONGTS_FAILED;
			}
		}

		break;
	case SVR_SELTEAMTIMEOUT_NTF:
		{
			MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam timeout",  p->iid_, userid_, losttime));

			test_state_ =TS_SELSVRTEAM_FAILED;
		}

		break;
	case SVR_QUITSERVERRECONN_ACK:
		{
			Pro_SvrQuitTeamReconn_ack* ack =dynamic_cast<Pro_SvrQuitTeamReconn_ack*>(p);
			if( ack->result_ == 0)
			{
				this->userid_ =ack->userid_;
				this->reconn_userid_ =NO_INITVALUE;
				this->reconn_token_ =0;

				test_state_ =TS_LOGON_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ quitserver and reconnect to lgs success",  p->iid_, userid_, losttime));
			}
			else
			{
				test_state_ =TS_RELOGONING_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ quitserver and reconnect to lgs failed",  p->iid_, reconn_userid_, losttime));
			}
		}

		break;
	}
}

void SelSvrTeamTestImpl::do_gts( BasicProtocol *p)
{
	ULONGLONG losttime =GetMillisecondTime() - record_time_;

	switch( p->iid_)
	{
	case SVR_SELSERVERCONFIRM_ACK:
		{
			Pro_SvrSelTeamConfirm_ack *lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);

			if( lack->result_ == 0)
			{
				pcase_->disconnect_2_lgs();

				test_state_ =TS_LOGONGTS_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam success step(2)-SVR_SELSERVERCONFIRM_ACK",  p->iid_, userid_, losttime));
			}
			else
			{
				pcase_->disconnect_2_gts();

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ select svrteam failed step(2)-SVR_SELSERVERCONFIRM_ACK",  p->iid_, userid_, losttime));

				test_state_ =TS_LOGONGTS_FAILED;
			}
		}

		break;
	case SVR_QUITSERVER_ACK:
		{
			Pro_SvrQuitTeam_ack *ack =dynamic_cast<Pro_SvrQuitTeam_ack*>(p);

			if( ack->result_ == 0)
			{
				this->reconn_token_ =ack->token_;
				this->reconn_userid_ =userid_;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ quit svrteam success and wait to reconnect",  p->iid_, reconn_userid_, losttime));

				test_state_ =TS_SVRTEAMQUIT_SUCCESS;
			}
			else
			{
				test_state_ =TS_SVRTEAMQUIT_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#%d (%d) $%d$ quit svrteam failed",  p->iid_, userid_, losttime));
			}
		}

		break;
	}
}

void SelSvrTeamTestImpl::status_check( ULONGLONG now)
{
	switch( test_state_)
	{
	case TS_SELSVRTEAM_SUCCEED:
		{
			int err =0;
			if( pcase_->connect_2_gts( err))
			{
				Pro_SvrSelTeamConfirm_req* req =new Pro_SvrSelTeamConfirm_req();
				req->proxy_index_ =proxy_index_;
				req->token_ =token_;
				pcase_->send_gts_protocol( req);

				test_state_ =TS_CONNECTGTS_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "#0 (%d) --------------------------------------- connect to gts success", userid_));
			}
			else
			{
				test_state_ =TS_CONNECTGTS_FAILED;
				MODULE_LOG_DEBUG((MODULE_TEMP, "#0 (%d) --------------------------------------- connect to gts failed err:%d", userid_, err));
			}

			tttmp_ =GetMillisecondTime();
		}

		break;
	case TS_LOGONGTS_SUCCEED:
		{
			Pro_SvrQuitTeam_req* req =new Pro_SvrQuitTeam_req();
			pcase_->send_gts_protocol( req);

			tttmp_ =record_time_ =GetMillisecondTime();

			test_state_ =TS_SVRTEAM_QUITING;
		}

		break;

	case TS_SVRTEAM_QUITING:
		{
			if( tttmp_ + 10*1000 < now)
			{
				MODULE_LOG_DEBUG((MODULE_TEMP, "#0 (%d) --------------------------------------- Pro_SvrQuitTeam_req not ack", userid_));

				this->reconnect_2_lgs();
				pcase_->disconnect_2_gts();
				this->reconn_token_ =0;
				this->reconn_userid_ =NO_INITVALUE;

				//test_state_ =TS_SVRTEAMQUIT_FAILED;
			}
		}

		break;
	case TS_SVRTEAMQUIT_SUCCESS:
		{
			if( tttmp_ + 1000 < now)
			{
				pcase_->disconnect_2_gts();
				this->reconnect_2_lgs();
			}
		}

		break;
	}
}
