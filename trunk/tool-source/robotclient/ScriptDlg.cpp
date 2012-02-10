// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ScriptDlg.h"

#include "afxcmn.h"

#include <string>

IMPLEMENT_DYNAMIC(ScriptDlg, CDialog)
ScriptDlg::ScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ScriptDlg::IDD, pParent)
	, run_script_(_T(""))
{
}

ScriptDlg::~ScriptDlg()
{
}

void ScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPPORTCOMBO, support_fun_);
	DDX_Text(pDX, IDC_RUNSCEDIT, run_script_);
	DDX_Text(pDX, IDC_STATUSEDIT, status_);
	DDX_Text(pDX, IDC_RESULTEDIT, run_result_);
	DDX_Control(pDX, IDC_RESULTEDIT, m_resedit);
}

BEGIN_MESSAGE_MAP(ScriptDlg, CDialog)
	ON_BN_CLICKED(IDC_RUNBUTTON, OnBnClickedRunBut)
	ON_LBN_DBLCLK(IDC_SUPPORTCOMBO, OnLbnDblclkSupportcombo)
    ON_BN_CLICKED(IDC_CLEARBTN, OnBnClickedClearbtn)
    ON_MESSAGE( WM_USER+0x200, OnWrite)
END_MESSAGE_MAP()

void ScriptDlg::OnBnClickedRunBut()
{
	 UpdateData(true);
	 
	 if( run_script_.GetLength() > 0)
	 {
         std::string cmd ="run_cmd:";
		 cmd +=(LPCTSTR)run_script_;

         int ret =scrimpl_.RunLuaShellCmd( cmd);
         if( ret != 1)
             AfxMessageBox("脚本error!");
		 else
		 {
             run_result_ =scrimpl_.ret_desc_ + "\r\n" + run_result_;
			 status_ =scrimpl_.get_status().c_str();
			 UpdateData( false);
		 }
	 }
	 else
	 {
		AfxMessageBox("请输入脚本!");
	 }	
}

LRESULT ScriptDlg::OnWrite(WPARAM param,LPARAM lparam)
{
    if( param !=NULL)
        run_result_ =*((CString*)param) + "\r\n" +run_result_;

    //UpdateData( false);
	m_resedit.SetWindowText( run_result_);

    return 0;
}

BOOL ScriptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    this->scrimpl_.set_wnd( this->m_hWnd);

    UpdateData( false);

	//支持的脚本函数
	support_fun_.AddString( "logon('1','1')");
	support_fun_.AddString( "logout()");
	support_fun_.AddString( "selsvrteam(0)");
	support_fun_.AddString( "quitteam()");
	support_fun_.AddString( "getchrlist()");
	support_fun_.AddString( "addonechr('nickname',0,1,1)");
	support_fun_.AddString( "selchr(1)");
	support_fun_.AddString( "delchr(1)");
	support_fun_.AddString( "mapchat('xxxxx')");
	support_fun_.AddString( "teleport(0,0)");
	support_fun_.AddString( "moveto(100,100,100)");
	support_fun_.AddString( "enterinst(0)");
	support_fun_.AddString( "quitinst()");
	support_fun_.AddString( "petfollow(1)");
	support_fun_.AddString( "petback()");

	return TRUE;
}
void ScriptDlg::OnLbnDblclkSupportcombo()
{
	int sel =support_fun_.GetCurSel();
	if( sel == LB_ERR)
		return;

	support_fun_.GetText( sel, run_script_);

	UpdateData( false);
}

void ScriptDlg::OnBnClickedClearbtn()
{
    // TODO: Add your control notification handler code here
    UpdateData( true);
    run_result_ ="";
    UpdateData( false);
}
