// SaoriTester.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "SaoriTester.h"
#include "SaoriTesterDlg.h"

#include <objbase.h>
#include <objidl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp

BEGIN_MESSAGE_MAP(CSaoriTesterApp, CWinApp)
	//{{AFX_MSG_MAP(CSaoriTesterApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp �N���X�̍\�z

CSaoriTesterApp::CSaoriTesterApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSaoriTesterApp �I�u�W�F�N�g

CSaoriTesterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp �N���X�̏�����

BOOL CSaoriTesterApp::InitInstance()
{
	AfxOleInit();
	::CoInitializeSecurity(NULL, -1, NULL, NULL,RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,NULL, EOAC_NONE, NULL);

	free((void*)m_pszProfileName);

	char myPath[MAX_PATH];
	::GetModuleFileName(NULL,myPath,sizeof(myPath));
	char *lastPeriod = (char*)_mbsrchr((unsigned char*)myPath,'.');
	strcpy(lastPeriod,".ini");

	m_pszProfileName = (const char*)malloc(strlen(myPath) + 1);
	strcpy((char*)m_pszProfileName,myPath);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CSaoriTesterDlg *pDlg = new CSaoriTesterDlg(cmdInfo.m_strFileName);
	m_pMainWnd = pDlg;

	return TRUE;
}

int CSaoriTesterApp::ExitInstance() 
{
	::CoUninitialize();

	return CWinApp::ExitInstance();
}
