
// WuerfelTest.h: Hauptheaderdatei f�r die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei f�r PCH einschlie�en"
#endif

#include "resource.h"		// Hauptsymbole


// CWuerfelTestApp:
// Siehe WuerfelTest.cpp f�r die Implementierung dieser Klasse
//

class CWuerfelTestApp : public CWinApp
{
public:
	CWuerfelTestApp();

// �berschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CWuerfelTestApp theApp;