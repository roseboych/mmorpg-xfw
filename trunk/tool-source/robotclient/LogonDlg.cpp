#include "stdafx.h"

#include "LogonDlg.h"

// CLogonDlg dialog

IMPLEMENT_DYNAMIC(CLogonDlg, CDialog)
CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
	, m_ip(_T(""))
	, m_port(0)
	, m_logon(_T("robot001"))
{
}

CLogonDlg::~CLogonDlg()
{
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROBOTIP, m_ip);
	DDX_Text(pDX, IDC_ROBOTPORT, m_port);
	DDX_Text(pDX, IDC_LOGONNAME, m_logon);
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CLogonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CLogonDlg::OnBnClickedOk()
{
	UpdateData( TRUE);

	if( m_ip.GetLength() == 0 || m_port == 0 || m_logon.GetLength() == 0)
	{
		MessageBox("pls fill full information include ip/port/robotname");
		return;
	}

	OnOK();
}

