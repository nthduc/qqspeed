// dbgview.h : dbgview Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CdbgviewApp:
// �йش����ʵ�֣������ dbgview.cpp
//

class CdbgviewApp : public CWinApp
{
public:
	CdbgviewApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern CdbgviewApp theApp;
