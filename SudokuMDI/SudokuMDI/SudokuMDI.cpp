// SudokuMDI.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "SudokuSquareDoc.h"
#include "SudokuDuoDoc.h"
#include "SudokuSamuraiDoc.h"
#include "SudokuMDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuMDIApp

BEGIN_MESSAGE_MAP(CSudokuMDIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSudokuMDIApp::OnAppAbout)
	// Dateibasierte Standarddokumentbefehle
	ON_COMMAND(ID_FILE_NEW, &CSudokuMDIApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CSudokuMDIApp::OnFileOpen)
	// Standarddruckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW, &CSudokuMDIApp::OnFileNew)
	ON_COMMAND(ID_SQUARE_8X8, &CSudokuMDIApp::OnSquare8x8)
	ON_COMMAND(ID_SQUARE_9X9, &CSudokuMDIApp::OnSquare9x9)
	ON_COMMAND(ID_SQUARE_12X12, &CSudokuMDIApp::OnSquare12x12)
	ON_COMMAND(ID_SQUARE_16X16, &CSudokuMDIApp::OnSquare16x16)
	ON_COMMAND(ID_NEU_DUO, &CSudokuMDIApp::OnNeuDuo)
	ON_COMMAND(ID_NEU_SAMURAI, &CSudokuMDIApp::OnNeuSamurai)
    ON_COMMAND(ID_FILE_OPEN, &CSudokuMDIApp::OnFileOpen)
    ON_COMMAND(ID_DATEI_UNINSTALL, &CSudokuMDIApp::OnDateiUninstall)
END_MESSAGE_MAP()


// CSudokuMDIApp-Erstellung

CSudokuMDIApp::CSudokuMDIApp()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CSudokuMDIApp-Objekt

CSudokuMDIApp theApp;


// CSudokuMDIApp-Initialisierung

BOOL CSudokuMDIApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// OLE-Bibliotheken initialisieren
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));
	LoadStdProfileSettings(4);  // Standard INI-Dateioptionen laden (einschließlich MRU)
	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_SudokuSquareTYPE,
		RUNTIME_CLASS(CSudokuSquareDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CSudokuMDIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_SudokuDuoTYPE,
		RUNTIME_CLASS(CSudokuDuoDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CSudokuMDIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_SudokuSamuraiTYPE,
		RUNTIME_CLASS(CSudokuSamuraiDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CSudokuMDIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Haupt-MDI-Rahmenfenster erstellen
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// Rufen Sie DragAcceptFiles nur auf, wenn eine Suffix vorhanden ist.
	//  In einer MDI-Anwendung ist dies unmittelbar nach dem Festlegen von m_pMainWnd erforderlich
	// Öffnen mit Drag  Drop aktivieren
	m_pMainWnd->DragAcceptFiles();

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle. Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	pMainFrame->SetWindowPos(NULL, 0, 0, 1014, 738, SWP_SHOWWINDOW | SWP_NOZORDER);
	pMainFrame->ShowWindow(SW_SHOW);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CSudokuMDIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CSudokuMDIApp-Meldungshandler


void CSudokuMDIApp::OnFileNew()
{
	// TODO: Add your command handler code here
}

void CSudokuMDIApp::OnSquare8x8()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuSquare"))
		{
			CSudokuSquareDoc::SetBoardType(btSquare8x8);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnSquare9x9()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuSquare"))
		{
			CSudokuSquareDoc::SetBoardType(btSquare9x9);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnSquare12x12()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuSquare"))
		{
			CSudokuSquareDoc::SetBoardType(btSquare12x12);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnSquare16x16()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuSquare"))
		{
			CSudokuSquareDoc::SetBoardType(btSquare16x16);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnNeuDuo()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuDuo"))
		{
			CSudokuSquareDoc::SetBoardType(btDuo);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnNeuSamurai()
{
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CString szTitle;
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		pTemplate->GetDocString(szTitle, CDocTemplate::docName);
		if (szTitle == _T("SudokuSamurai"))
		{
			CSudokuSquareDoc::SetBoardType(btSamurai);
			pTemplate->OpenDocumentFile(NULL);
		}
	}
}

void CSudokuMDIApp::OnFileOpen()
{
    CWinApp::OnFileOpen();
}

void CSudokuMDIApp::OnDateiUninstall()
{
    UnregisterShellFileTypes();
}
