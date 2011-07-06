#include "stdafx.h"

#include "ThreadTest.h"
#include "ThreadTestDlg.h"
#include "winsock2.h"
#include "LogonDlg.h"
#include "ScriptDlg.h"

CString g_ip;
int		g_port;

CWnd	*g_mainwnd =0;
TEST_TYPE g_testtype = T_TYPE_UNKNOW;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CThreadTestDlg::CThreadTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadTestDlg::IDD, pParent)
	, m_user(0),
	tests_( 0),
	thread_num_( 0)
	, m_port(0)
	, m_usernum(1)	
	, m_gatewayip(_T(""))
	, m_startplayer(_T(""))
	, m_playernums(_T(""))
	, m_gwip(_T(""))
	, m_robotstatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_port);
	DDX_Control(pDX, IDOK, m_startbut);
	DDX_Control(pDX, IDCANCEL, m_endbut);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT2, m_gatewayip);
	DDX_Text(pDX, IDC_STARTNUM, m_startplayer);
	DDX_Text(pDX, IDC_CNTNUM, m_playernums);
	DDX_Text(pDX, IDC_GWIP, m_gwip);
	DDX_Text(pDX, IDC_ROBOTSTATUS, m_robotstatus);
}

BEGIN_MESSAGE_MAP(CThreadTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SCRIPTBUTTON, OnBnClickedScriptbutton)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CThreadTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	char filename[260];
	GetModuleFileName(NULL, filename, sizeof(filename));
	(_tcsrchr(filename,'\\'))[1] = 0;
	strcat( filename, "robotclient.ini");

	char text[256];
	::GetPrivateProfileString( "run", "lgsip", "127.0.0.1", text, sizeof(text), filename);
	m_gatewayip = text;
	m_port = ::GetPrivateProfileInt( "run", "lgsport", 18000, filename);

	m_user =0;

	UpdateData( FALSE);

	m_list.InsertColumn( 0, "user", LVCFMT_LEFT, 100);
	m_list.InsertColumn( 1, "msg", LVCFMT_LEFT, 500);	
	
	return TRUE;
}

void CThreadTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CThreadTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CThreadTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CThreadTestDlg::OnBnClickedOk()
{
	m_list.DeleteAllItems();

	CLogonDlg dlg;

	char filename[260];
	GetModuleFileName(NULL, filename, sizeof(filename));
	(_tcsrchr(filename,'\\'))[1] = 0;
	strcat( filename, "robotclient.ini");

	char text[256];
	::GetPrivateProfileString( "run", "robotserverip", "127.0.0.1", text, sizeof(text), filename);
	dlg.m_ip = text;
	dlg.m_port = ::GetPrivateProfileInt( "run", "robotserverport", 12345, filename);
	::GetPrivateProfileString( "run", "logonuser", "robot___", text, sizeof(text), filename);
	dlg.m_logon = text;

	if( dlg.DoModal() != IDOK)
		return;

	::WritePrivateProfileString( "run", "robotserverip", dlg.m_ip, filename);
	CString robotserverport;
	robotserverport.Format( "%u", dlg.m_port);
	::WritePrivateProfileString( "run", "robotserverport", robotserverport, filename);
	::WritePrivateProfileString( "run", "logonuser", dlg.m_logon, filename);


	if( this->m_hSocket != NULL)
		CSocket::Close();

	CSocket::Create();
	if( !CSocket::Connect( (LPCTSTR)dlg.m_ip, dlg.m_port))
	{
		MessageBox("connect to robot server failed!");
		CSocket::Close();
		return;
	}

	read_buffer_pos_ =0;

	m_startbut.EnableWindow( FALSE);

	Robot_Config_Req* req = new Robot_Config_Req();
	if ( req)
	{
		req->version_ = ROBOT_VERSION;
		req->robotname_ = dlg.m_logon.GetBuffer(0);
		req->robotpwd_ = "123456";
		char data[8192];
		int len = 0;
		req->data( data, 8192, len, (void*)0, false);

		CSocket::Send( &len, sizeof(int));
		CSocket::Send( data, len);
		delete req;
	}
}

void CThreadTestDlg::OnReceive( int nErrorCode)
{
	int len =CSocket::Receive( buffer_+read_buffer_pos_, MAX_BUFFER_LEN - read_buffer_pos_);
	if( len > 0)
		read_buffer_pos_ += len;

	if( read_buffer_pos_ >= sizeof(int))
	{
		int plen =*((int*)buffer_);
		if( plen <= read_buffer_pos_ - sizeof( int))
		{
			read_buffer_pos_ -= sizeof( int);

			OnRecvData( buffer_ + sizeof(int), plen);

			if( read_buffer_pos_ - plen > 0)
				memmove( buffer_, buffer_ + plen + sizeof(int), read_buffer_pos_ - plen);

			read_buffer_pos_ -= plen;
		}
	}
}

void CThreadTestDlg::OnClose( int nErrorCode)
{
	OnBnClickedCancel();

	m_robotstatus ="stop test....";
	UpdateData( FALSE);
}

void CThreadTestDlg::OnBnClickedCancel()
{
	for( int s =0; s < thread_num_; ++s)
		tests_[s].stop_thread();

	if( tests_)
		delete[] tests_;
	tests_ =0;
	thread_num_ =0;

	m_startbut.EnableWindow( TRUE);
}

void CThreadTestDlg::OnClose()
{
	CDialog::OnCancel();
}

LRESULT CThreadTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message)
	{
	case WM_INFONOTIFY:
		show_notify( (MsgNotify*)wParam);
		break;

	case WM_ERRORNOTIFY:
		show_error( (MsgNotify*)wParam);
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CThreadTestDlg::show_notify( MsgNotify *msg)
{
	int ss =m_list.GetItemCount();
	for( int i =0; i < ss; ++i)
	{
		int iid =(int)m_list.GetItemData( i);
		if( iid == msg->userid_)
		{
			m_list.SetItemText( i, 1, msg->msg_);
			delete msg;
			return;
		}
	}

	CString str;
	str.Format( "%d", msg->userid_);
	long lid =m_list.InsertItem( m_list.GetItemCount(), str);
	m_list.SetItemText( lid, 1, msg->msg_);
	m_list.SetItemData( lid, msg->userid_);

	delete msg;
}

void CThreadTestDlg::show_error( MsgNotify *msg)
{
	ReportError( "user:%d %s", msg->userid_, msg->msg_);
	delete msg;
}

void CThreadTestDlg::report_state( BasicProtocol* p)
{
	if ( CAsyncSocket::FromHandle(m_hSocket) != NULL )
	{
		char data[8192];
		int len = 0;
		p->data( data, 8192, len, (void*)0, false);
		CSocket::Send( &len, sizeof(int));
		CSocket::Send( data, len);
	}
}

void CThreadTestDlg::OnBnClickedScriptbutton()
{
	UpdateData( true);

	char filename[260];
	GetModuleFileName(NULL, filename, sizeof(filename));
	(_tcsrchr(filename,'\\'))[1] = 0;
	strcat( filename, "robotclient.ini");

	::WritePrivateProfileString( "run", "lgsip", m_gatewayip, filename);
	CString gatewayport;
	gatewayport.Format( "%u", m_port);
	::WritePrivateProfileString( "run", "lgsport", gatewayport, filename);

	ScriptDlg dlg;
	dlg.setip( (LPCTSTR)m_gatewayip);
	dlg.setport( m_port);

	dlg.DoModal();
}

void CThreadTestDlg::OnRecvData( char* data, int len)
{
	BasicProtocol* p = RobotProFactory::data_protocol( data, len);
	if ( p)
	{
		switch( p->iid_)
		{
		case RBT_CONFIG_ACK:
			{
				Robot_Config_Ack* ack = reinterpret_cast<Robot_Config_Ack*>(p);
				if ( 0 == ack->ret_)
				{
					m_robotid = ack->robotid_;
					m_gwip = ack->gatewayip_.c_str();
					m_port = ack->gatewayport_;
					m_startplayer.Format( "%d", ack->startuserid_);
					m_playernums.Format( "%d", ack->users_);
					m_user = ack->startuserid_;
					m_usernum = ack->users_;
					m_robotstatus = "config success";
					UpdateData( FALSE);
				}
				else
				{
					ReportError( "get config failed. ret = %d", ack->ret_);
					m_startbut.EnableWindow( TRUE);
				}
			}
			break;

		case RBT_START_ACK:
			{
				Robot_Start_Ack* ack = reinterpret_cast<Robot_Start_Ack*>(p);
				g_testtype = (TEST_TYPE)ack->testtype_;
				m_robotstatus = "begin to login";
				UpdateData( FALSE);
				StartAutoRobot();
			}
			break;

		case RBT_STOP_ACK:
			{
				Robot_Stop_Ack* ack = reinterpret_cast<Robot_Stop_Ack*>(p);
				m_robotstatus = "stop login";
				UpdateData( FALSE);
				StopAutoRobot();
			}
			break;
		}
	}

	delete p;
}

void CThreadTestDlg::ReportError( char* pFmt, ...)
{
	char text[32];
	CTime tm = CTime::GetCurrentTime();
	sprintf(text, "%.2d %.2d:%.2d:%.2d ", tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	va_list args;
	char buf[1024];
	va_start(args, pFmt);
	vsprintf(buf, pFmt, args);
	log_ = CString(text) + CString(buf) + "\r\n" + log_;
	GetDlgItem(IDC_EDT_LOG)->SetWindowText( log_);
}

void CThreadTestDlg::StartAutoRobot()
{
	g_ip = m_gwip;
	g_port = m_port;

	if( tests_)
		delete[] tests_;
	thread_num_ = m_usernum / MAXCASE_PRETHREAD;
	if( m_usernum - thread_num_*MAXCASE_PRETHREAD > 0)
		++thread_num_;

	tests_ =new ThreadWrapper[thread_num_];
	int pos =0;
	for( int i=0 ; i < thread_num_ ; ++i)
	{
		tests_[i].set_thread( m_user+pos, (pos + MAXCASE_PRETHREAD - m_usernum) > 0 ? (m_usernum-pos):MAXCASE_PRETHREAD);
		tests_[i].start_thread();

		pos += MAXCASE_PRETHREAD;
		if( pos >= m_usernum)
			break;
	}

	SetTimer( 1000, 1000, NULL);
}

void CThreadTestDlg::StopAutoRobot()
{
	for( int s =0; s < thread_num_; ++s)
		tests_[s].set_cancel();

	for( int s =0; s < thread_num_; ++s)
		tests_[s].stop_thread();

	if( tests_)
		delete[] tests_;
	tests_ =0;
	thread_num_ =0;
}

void CThreadTestDlg::OnTimer(UINT nIDEvent)
{
	if ( 1000 == nIDEvent )
	{
		if ( 0 == tests_)
			KillTimer( nIDEvent);
	}

	__super::OnTimer(nIDEvent);
}

void CThreadTestDlg::OnDestroy()
{
	__super::OnDestroy();

	if( tests_)
		delete[] tests_;
	tests_ =0;
}
