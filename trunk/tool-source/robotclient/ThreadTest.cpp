#include "stdafx.h"
#include "ThreadTest.h"
#include "ThreadTestDlg.h"
#include "logmacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CWnd *g_mainwnd;

BEGIN_MESSAGE_MAP(CThreadTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CThreadTestApp::CThreadTestApp()
{
}


CThreadTestApp theApp;

BOOL CThreadTestApp::InitInstance()
{
	MODULE_LOG_INIT(("run.properties"));

	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("tps function tester"));

	CThreadTestDlg dlg;
	m_pMainWnd = &dlg;
	g_mainwnd =m_pMainWnd;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}

int CThreadTestApp::ExitInstance()
{
	MODULE_LOG_SHUTDOWN;

	return CWinApp::ExitInstance();
}
