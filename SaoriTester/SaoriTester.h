// SaoriTester.h : SAORITESTER �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_SAORITESTER_H__E4B2BE86_2530_48C9_8F0D_4B2D71B97AAA__INCLUDED_)
#define AFX_SAORITESTER_H__E4B2BE86_2530_48C9_8F0D_4B2D71B97AAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SaoriTester.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSaoriTesterApp : public CWinApp
{
public:
	CSaoriTesterApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSaoriTesterApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CSaoriTesterApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SAORITESTER_H__E4B2BE86_2530_48C9_8F0D_4B2D71B97AAA__INCLUDED_)
