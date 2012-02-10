#include "StdAfx.h"

#include "SelRoleTestImpl.h"

#include "../TestCase.h"
#include "../logmacro.h"

USE_PROTOCOL_NAMESPACE

SelRoleTestImpl::SelRoleTestImpl( TestCase *p)
:BasicImpl( p)
{
}

SelRoleTestImpl::~SelRoleTestImpl(void)
{
}

void SelRoleTestImpl::do_lgs( BasicProtocol *p)
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
				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  user:%s  login success...  totletime:%d", p->iid_, (LPCTSTR)user_name_,  losttime));
			}
			else
			{
				test_state_ =TS_LOGON_FAILED;
				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  user:%s  login failed...  totletime:%d",  p->iid_, (LPCTSTR)user_name_, losttime));
			}

			record_time_ =GetMillisecondTime();
		}

		break;
	case SVR_SVRTEAMINFO_NTF:
		{
			if( test_state_ != TS_LOGON_SUCCEED)
				break;

			Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>(p);

			NS_STL::list<Pro_SvrTeamInfo_NTF::svrteaminfo>::iterator iter=ntf->teams_.begin(), eiter =ntf->teams_.end();
			for( ; iter != eiter; ++iter)
			{
				Pro_SvrTeamInfo_NTF::svrteaminfo& sinfo =(*iter);
				if( sinfo.is_validate_ == 0)
					continue;

				{
					Pro_SvrSelTeam_req* req =new Pro_SvrSelTeam_req();
					req->team_index_ =sinfo.server_index_;

					pcase_->send_lgs_protocol( req);

					test_state_ =TS_SVRTEAM_SELECTING;

					record_time_ =GetMillisecondTime();

					break;
				}
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

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  svrselteam ack success totletime:%d",  p->iid_, losttime));
			}
			else
			{
				test_state_ =TS_SELSVRTEAM_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  svrselteam ack failed totletime:%d",  p->iid_, losttime));
			}

		}

		break;
	case SVR_SELSERVERCONFIRM_ACK:
		{
			Pro_SvrSelTeamConfirm_ack* lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);

			if( lack->result_ == 0)
			{
				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select server team success totletime:%d",  p->iid_, losttime));

				test_state_ =TS_LOGONGTS_SUCCEED;
			}
			else
			{
				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select server team failed totletime:%d",  p->iid_, losttime));

				test_state_ =TS_LOGONGTS_FAILED;
			}
		}

		break;
	case SVR_SELTEAMTIMEOUT_NTF:
		{
			MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select server team timeout totletime:%d",  p->iid_, losttime));

			test_state_ =TS_SELSVRTEAM_FAILED;
		}

		break;
	}
}

void SelRoleTestImpl::do_gts( BasicProtocol *p)
{
	ULONGLONG losttime =GetMillisecondTime() - record_time_;

	switch( p->iid_)
	{
	case SVR_SELSERVERCONFIRM_ACK:
		{
			Pro_SvrSelTeamConfirm_ack *lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(p);

			if( lack->result_ == 0)
			{
				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select server team success totletime:%d",  p->iid_, losttime));

				test_state_ =TS_LOGONGTS_SUCCEED;
			}
			else
			{
				pcase_->disconnect_2_gts();

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select server team failed totletime:%d",  p->iid_, losttime));

				test_state_ =TS_LOGONGTS_FAILED;
			}
		}

		break;
	case GAME_CHRLIST_ACK:
		{
			Pro_ChrList_ack* lack =dynamic_cast<Pro_ChrList_ack*>(p);

			chrid_ =NO_INITVALUE;
			for( NS_STL::list<Pro_ChrList_ack::chrinfo>::iterator iter =lack->chrs_.begin(); iter != lack->chrs_.end(); ++iter)
			{
				Pro_ChrList_ack::chrinfo& c =(*iter);
				chrid_ =c.chrid_;
				break;
			}

			if( chrid_ != NO_INITVALUE)
			{
				test_state_ =TS_ROLELIST_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  get character list success totletime:%d",  p->iid_, losttime));
			}
			else
			{
				test_state_ =TS_ROLELIST_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  get character list failed totletime:%d",  p->iid_, losttime));
			}
		}

		break;
	case GAME_CHRSEL_ACK:
		{
			Pro_ChrSel_ack* lack =dynamic_cast<Pro_ChrSel_ack*>(p);

			if( lack->result_ == 0)
			{
				chrid_ =lack->chrid_;

				test_state_ =TS_SELROLE_SUCCEED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select character success totletime:%d",  p->iid_, losttime));
			}
			else
			{
				test_state_ =TS_SELROLE_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d  select character failed totletime:%d",  p->iid_, losttime));
			}
		}

		break;
	case GAME_CHRFIN_NTF:
		{
			Pro_ChrFin_NTF* ack =dynamic_cast<Pro_ChrFin_NTF*>(p);

			MODULE_LOG_DEBUG((MODULE_TEMP, "msgid:%d select channel success totletime:%d",  p->iid_, losttime));
		}

		break;
	}
}

void SelRoleTestImpl::status_check( ULONGLONG now)
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

				MODULE_LOG_DEBUG((MODULE_TEMP, ">>>>>>>>>>>>>>>>>> connect to gts success"));
			}
			else
			{
				test_state_ =TS_CONNECTGTS_FAILED;

				MODULE_LOG_DEBUG((MODULE_TEMP, ">>>>>>>>>>>>>>>>>> connect to gts failed"));
			}
		}

		break;
	case TS_LOGONGTS_SUCCEED:
		{
			Pro_ChrList_req* req =new Pro_ChrList_req();
			pcase_->send_gts_protocol( req);

			test_state_ =TS_ROLE_LISTING;

			record_time_ =GetMillisecondTime();
		}

		break;
	case TS_ROLELIST_SUCCEED:
		{
			Pro_ChrSel_req* req =new Pro_ChrSel_req();
			req->chrid_ =chrid_;

			pcase_->send_gts_protocol( req);

			test_state_ =TS_ROLE_SELECTING;

			record_time_ =GetMillisecondTime();
		}

		break;
	}
}
