#pragma once

// MFCActiveXControl1.h : main header file for MFCActiveXControl1.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols


// CMFCActiveXControl1App : See MFCActiveXControl1.cpp for implementation.

class CMFCActiveXControl1App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

