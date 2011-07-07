#include "StdAfx.h"

#include "sharelib/Pro_Robot.h"

#include <vector>
#include "maindlg.h"
#include "OptionUtil.h"
#include "SelTypeDlg.h"

extern OptionUtil g_opt;

using namespace ROBOT_PROTOCOL;

LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if ( 1000 == wParam)
	{
		listener_.do_main( GetTickCount());
	}

	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	listener_.stop_listen();

	return 0;
}

void CMainDlg::onAccept( SOCKET socket)
{
	char text[256];
	sockaddr_in sa;
	int   salen;
	salen = sizeof(sa);
	if( getpeername( socket, (sockaddr*)&sa, &salen ) == 0 )
	{
		sprintf( text, "%s:%d", inet_ntoa( sa.sin_addr), ntohs( sa.sin_port));
	}
	else
	{
		sprintf( text, "peer ip error");
	}

	//MessageBox( text);
}

void CMainDlg::onClosed( SOCKET socket)
{
	S_INT_32 item = FindLstItemBySocket( socket);
	if ( -1 != item)
	{
		lstclients_.SetItemData(item, 0);
		lstclients_.SetItem( item, 0, LVIF_TEXT | LVIF_IMAGE, "0", ICON_UNKNOW, 0, 0, 0);
	}
}

void CMainDlg::onRecvData( char * data, int len, SOCKET socket)
{
}

void CMainDlg::onRecvProtocol( BasicProtocol* p, SOCKET socket)
{
	using namespace ROBOT_PROTOCOL;

	switch( p->iid_) 
	{
	case RBT_CONFIG_REQ:
		{
			Robot_Config_Req* req = reinterpret_cast <Robot_Config_Req*>(p);

			Robot_Config_Ack* ack = new Robot_Config_Ack();
			ack->version_ = ROBOT_VERSION;

			OptionUtil::RobotDesctiption* des = g_opt.find_robot_byname( req->robotname_);
			if ( des)
			{
				if ( des->robotpwd_ != req->robotpwd_)
					ack->ret_ = -2;
				else
				{
					if ( req->version_ != ROBOT_VERSION)
					{
						ack->ret_ = -4;
					}
					else
					{
						ack->robotid_ = des->robotid_;
						ack->startuserid_ = des->startuserid_;
						ack->users_ = des->users_;
						ack->gatewayip_ = des->gatewayip_;
						ack->gatewayport_ = des->gatewayport_;
						ack->ret_ = 0;

						S_INT_32 item = FindLstItemByName( des->robotname_);
						if ( -1 != item)
						{
							char text[256];
							sprintf(text, "%u", socket);
							lstclients_.SetItem(item, 0, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%s", des->robotname_.c_str());
							lstclients_.SetItem(item, 1, LVIF_TEXT, text, 0, 0, 0, 0);

							sockaddr_in sa;
							int   salen;
							salen = sizeof(sa);
							if( getpeername( socket, (sockaddr*)&sa, &salen ) == 0 )
							{
								sprintf( text, "%s", inet_ntoa( sa.sin_addr));
								lstclients_.SetItem(item, 2, LVIF_TEXT, text, 0, 0, 0, 0);
								sprintf( text, "%d", ntohs( sa.sin_port));
								lstclients_.SetItem(item, 3, LVIF_TEXT, text, 0, 0, 0, 0);
							}
							else
							{
								sprintf( text, "peer ip error");
								lstclients_.SetItem(item, 2, LVIF_TEXT, text, 0, 0, 0, 0);
								sprintf( text, "%d", ntohs( sa.sin_port));
								lstclients_.SetItem(item, 3, LVIF_TEXT, text, 0, 0, 0, 0);
							}

							CTime tm = CTime::GetCurrentTime();
							sprintf(text, "%.2d:%.2d:%.2d", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
							lstclients_.SetItem(item, 4, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", des->startuserid_);
							lstclients_.SetItem(item, 5, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", des->users_);
							lstclients_.SetItem(item, 6, LVIF_TEXT, text, 0, 0, 0, 0);
							sprintf(text, "%u", 0);
							lstclients_.SetItem(item, 7, LVIF_TEXT, text, 0, 0, 0, 0);
							lstclients_.SetItemData( item, socket);
							lstclients_.SetItem( item, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
						}
					}
				}
			}
			else
				ack->ret_ = -1;

			listener_.send_protocol(socket, ack);
		}
		break;

	default:
		break;
	}
}

void CMainDlg::InsterLog( char* pFmt, ... )
{
	char text[32];
	CTime tm = CTime::GetCurrentTime();
	sprintf(text, "%.2d:%.2d:%.2d ", tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	va_list args;
	char buf[1024];
	va_start(args, pFmt);
	vsprintf(buf, pFmt, args);
	log_ = WTL::CString(text) + WTL::CString(buf) + "\r\n" + log_;
	::SetWindowText( GetDlgItem(IDC_EDT_LOG),  log_);
}

S_INT_32 CMainDlg::FindLstItemBySocket( SOCKET socket)
{
	S_INT_32 count = lstclients_.GetItemCount();
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		if ( lstclients_.GetItemData( i) == socket)
		{
			return i;
		}
	}

	return -1;
}

S_INT_32 CMainDlg::FindLstItemByName( NS_STL::string robotname)
{
	S_INT_32 count = lstclients_.GetItemCount();
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		char text[256];

		if ( lstclients_.GetItemText( i, 1, text, sizeof(text)) > 0)
		{
			if ( robotname == text)
				return i;
		}
		else
			break;
	}

	return -1;
}


LRESULT CMainDlg::OnBnClickedBtnStartall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSelTypeDlg dlg;
	dlg.DoModal();

	if ( T_TYPE_UNKNOW != dlg.m_nType)
	{
		S_INT_32 count = lstclients_.GetItemCount();
		char text[256];
		for ( S_INT_32 i = 0; i < count; i ++ )
		{
			S_UINT_32 data = lstclients_.GetItemData( i);
			if ( data != 0)
			{
				SOCKET socket = (SOCKET)data;
				Robot_Start_Ack* ack = new Robot_Start_Ack();
				if ( ack)
				{
					ack->startusernum_ = -1;
					ack->logininterval_ = 10;
					ack->testtype_ = dlg.m_nType;
					listener_.send_protocol( socket, ack);
					lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_RUNNING, 0, 0, 0);
					lstclients_.GetItemText( i, 1, text, sizeof(text));
					InsterLog( "start command send to '%s'", text);
				}
			}
		}

		::SetFocus( GetDlgItem( IDC_LST_CLIENTS));
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStopall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	S_INT_32 count = lstclients_.GetItemCount();
	char text[256];
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		S_UINT_32 data = lstclients_.GetItemData( i);
		if ( data != 0)
		{
			SOCKET socket = (SOCKET)data;
			Robot_Stop_Ack* ack = new Robot_Stop_Ack();
			if ( ack)
			{
				listener_.send_protocol( socket, ack);
				lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
				lstclients_.GetItemText( i, 1, text, sizeof(text));
				InsterLog( "stop command send to '%s'", text);
			}
		}
	}

	::SetFocus( GetDlgItem( IDC_LST_CLIENTS));

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStartSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( lstclients_.GetSelectedCount() == 0) return 0;

	CSelTypeDlg dlg;
	dlg.DoModal();

	if ( T_TYPE_UNKNOW != dlg.m_nType)
	{
		S_INT_32 count = lstclients_.GetItemCount();
		char text[256];
		for ( S_INT_32 i = 0; i < count; i ++ )
		{
			if ( LVIS_SELECTED == lstclients_.GetItemState( i, LVIS_SELECTED))
			{
				S_UINT_32 data = lstclients_.GetItemData( i);
				if ( data != 0)
				{
					SOCKET socket = (SOCKET)data;
					Robot_Start_Ack* ack = new Robot_Start_Ack();
					if ( ack)
					{
						ack->startusernum_ = -1;
						ack->logininterval_ = 10;
						ack->testtype_ = dlg.m_nType;
						listener_.send_protocol( socket, ack);
						lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_RUNNING, 0, 0, 0);
						lstclients_.GetItemText( i, 1, text, sizeof(text));
						InsterLog( "start command send to '%s'", text);
					}
				}
			}
		}

		::SetFocus( GetDlgItem( IDC_LST_CLIENTS));
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnStopSel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	S_INT_32 count = lstclients_.GetItemCount();
	char text[256];
	for ( S_INT_32 i = 0; i < count; i ++ )
	{
		if ( LVIS_SELECTED == lstclients_.GetItemState( i, LVIS_SELECTED))
		{
			S_UINT_32 data = lstclients_.GetItemData( i);
			if ( data != 0)
			{
				SOCKET socket = (SOCKET)data;
				Robot_Stop_Ack* ack = new Robot_Stop_Ack();
				if ( ack)
				{
					listener_.send_protocol( socket, ack);
					lstclients_.SetItem( i, 0, LVIF_IMAGE, "", ICON_CONNECTED, 0, 0, 0);
					lstclients_.GetItemText( i, 1, text, sizeof(text));
					InsterLog( "stop command send to '%s'", text);
				}
			}
		}
	}

	::SetFocus( GetDlgItem( IDC_LST_CLIENTS));

	return 0;
}
