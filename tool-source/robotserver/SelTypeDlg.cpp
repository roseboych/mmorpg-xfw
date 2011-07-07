// SelTypeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelTypeDlg.h"

CSelTypeDlg::CSelTypeDlg() : 
m_nType( T_TYPE_UNKNOW)
{
}

LRESULT CSelTypeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CButton btn = ::GetDlgItem( m_hWnd, IDC_RDO_LOGIN_LOGOUT);
	btn.SetCheck( BST_CHECKED);

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
}

LRESULT CSelTypeDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton btn = ::GetDlgItem( m_hWnd, IDC_RDO_LOGIN_LOGOUT);
	if ( btn.GetCheck() == BST_CHECKED)
		m_nType = T_LOGIN_LOGOUT;

	btn = ::GetDlgItem( m_hWnd, IDC_RDO_SVTREAM_LOGIN);
	if ( btn.GetCheck() == BST_CHECKED)
		m_nType = T_LOGIN_SELSVRTEAM;

	btn = ::GetDlgItem( m_hWnd, IDC_RDO_ROLE_SELECT);
	if ( btn.GetCheck() == BST_CHECKED)
		m_nType = T_LOGIN_SELROLE;

	EndDialog(0);

	return TRUE;
}
