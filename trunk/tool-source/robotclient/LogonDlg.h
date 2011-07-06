#pragma once

#include "resource.h"
// CLogonDlg dialog

class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonDlg();

// Dialog Data
	enum { IDD = IDD_LOGONDLG };

public:
	CString m_ip;
	int m_port;
	CString m_logon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
