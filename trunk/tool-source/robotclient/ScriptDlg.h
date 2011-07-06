#pragma once

#include "afxwin.h"
#include "CommandTestImpl.h"
#include "ThreadTest.h"
#include "resource.h"
#include <string>

class ScriptDlg : public CDialog
{
	DECLARE_DYNAMIC(ScriptDlg)

public:
	ScriptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ScriptDlg();

// Dialog Data
	enum { IDD = IDD_SCRIPTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRunBut();

	CListBox support_fun_;
	CString run_script_;
	CString status_;
	CString run_result_;

public:
	void setip( std::string ip){
		scrimpl_.lgs_ip_ =ip;
	}
	void setport( int p){
		scrimpl_.lgs_port_ =p;
	}

public:
	CommandTestImpl scrimpl_;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnDblclkSupportcombo();
    afx_msg LRESULT OnWrite( WPARAM param,LPARAM lparam);
    afx_msg void OnBnClickedClearbtn();
	CEdit m_resedit;
};
