
// WuerfelTest.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CWuerfelTestApp:
// Siehe WuerfelTest.cpp für die Implementierung dieser Klasse
//

class CWuerfelTestApp : public CWinApp
{
public:
	CWuerfelTestApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CWuerfelTestApp theApp;