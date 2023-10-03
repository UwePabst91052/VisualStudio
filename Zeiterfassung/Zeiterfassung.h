// Zeiterfassung.h : Hauptheaderdatei für die Zeiterfassung-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole

#define WM_NEXT_NOTE    WM_USER * 14
#define WM_PREV_NOTE    WM_USER + 15
#define WM_SWITCH_WD    WM_USER + 16

// CZeiterfassungApp:
// Siehe Zeiterfassung.cpp für die Implementierung dieser Klasse
//

class CZeiterfassungApp : public CWinApp
{
public:
	CZeiterfassungApp();


// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
};

extern CZeiterfassungApp theApp;