// Zeiterfassung.h : Hauptheaderdatei f�r die Zeiterfassung-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei f�r PCH einschlie�en"
#endif

#include "resource.h"       // Hauptsymbole

#define WM_NEXT_NOTE    WM_USER * 14
#define WM_PREV_NOTE    WM_USER + 15
#define WM_SWITCH_WD    WM_USER + 16

// CZeiterfassungApp:
// Siehe Zeiterfassung.cpp f�r die Implementierung dieser Klasse
//

class CZeiterfassungApp : public CWinApp
{
public:
	CZeiterfassungApp();


// �berschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
};

extern CZeiterfassungApp theApp;