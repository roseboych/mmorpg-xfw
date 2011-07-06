#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "ThreadWrapper.h"
#include "MsgNotify.h"

#include "sharelib/Pro_Robot.h"

using namespace ROBOT_PROTOCOL;

#define WM_INFONOTIFY  WM_USER+0x100
#define WM_ERRORNOTIFY  WM_USER+0x101

#define MAXCASE_PRETHREAD	20

#define MAX_BUFFER_LEN	1024*8

class CThreadTestDlg : public CDialog, public CSocket
{
public:
	CThreadTestDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_THREADTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	void show_notify( MsgNotify *msg);
	void show_error( MsgNotify *msg);
	void report_state( BasicProtocol* p);

	virtual void OnReceive( int nErrorCode);
	virtual void OnClose( int nErrorCode);

	void OnRecvData( char* data, int len);

protected:
	HICON m_hIcon;

	char buffer_[MAX_BUFFER_LEN];
	int	 read_buffer_pos_;

	ThreadWrapper	*tests_;
	int				thread_num_;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CButton m_startbut;
	CButton m_endbut;

	CListCtrl	m_list;
	int			m_usernum;
	int			m_user;

	afx_msg void OnClose();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void ReportError( char* fmt, ...);
	void StartAutoRobot();
	void StopAutoRobot();

public:
	afx_msg void OnBnClickedScriptbutton();
	
	CString m_gatewayip;
	int		m_port;

	CString m_startplayer;
	CString m_playernums;
	CString m_gwip;
	CString m_robotstatus;

	int     m_robotid;

	CString log_;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
};
