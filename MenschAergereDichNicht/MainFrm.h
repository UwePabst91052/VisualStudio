// MainFrm.h : Schnittstelle der Klasse CMainFrame
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // Nur aus Serialisierung erstellen
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attribute
public:

// Vorgänge
public:

// Überschreibungen
public:
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
	CToolBar    m_wndSimulationBar;

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAnsichtSimulation();
	afx_msg void OnUpdateAnsichtSimulation(CCmdUI *pCmdUI);
private:
	BOOL m_bAnsichtSimulation;
public:
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
};


