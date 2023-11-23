
// WuerfelTestSdi.h : main header file for the WuerfelTestSdi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWuerfelTestSdiApp:
// See WuerfelTestSdi.cpp for the implementation of this class
//

class CWuerfelTestSdiApp : public CWinApp
{
public:
	CWuerfelTestSdiApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWuerfelTestSdiApp theApp;
