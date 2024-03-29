// MainFrm.h : Schnittstelle der Klasse CMainFrame
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // Nur aus Serialisierung erstellen
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attribute
protected:
	CSplitterWnd m_wndSplitter;
public:

// Vorg�nge
public:

// �berschreibungen
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementierung
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Eingebettete Member der Steuerleiste
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generierte Funktionen f�r die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


