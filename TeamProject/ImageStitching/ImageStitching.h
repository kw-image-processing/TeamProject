
// ImageStitching.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageStitchingApp:
// �� Ŭ������ ������ ���ؼ��� ImageStitching.cpp�� �����Ͻʽÿ�.
//

class CImageStitchingApp : public CWinApp
{
public:
	CImageStitchingApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageStitchingApp theApp;