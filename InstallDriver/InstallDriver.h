
// InstallDriver.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInstallDriverApp:
// �йش����ʵ�֣������ InstallDriver.cpp
//

class CInstallDriverApp : public CWinApp
{
public:
	CInstallDriverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInstallDriverApp theApp;