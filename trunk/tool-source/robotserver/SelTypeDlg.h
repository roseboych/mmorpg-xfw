#pragma once

#include "sharelib/Pro_Robot.h"
#include <atlctrls.h>
#include "resource.h"

class CSelTypeDlg : public CAxDialogImpl<CSelTypeDlg>
{
public:
	enum { IDD = IDD_SELTYPE_DLG };

	CSelTypeDlg();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_MSG_MAP(CSelTypeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	TEST_TYPE m_nType;
};
