// MainFrm.h : Schnittstelle der Klasse CMainFrame
//


#pragma once
#include "MousePad.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

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
	CToolBar    m_wndMoveBar;
    CMousePad   m_wndMousePad9x;
    CMousePad   m_wndMousePad12x;
    CMousePad   m_wndMousePad16x;
    CMousePad   m_wndMousePadAlpha12x;
    CMousePad   m_wndMousePadAlpha16x;
	CToolBar    m_wndSudokuBar;
	CToolBar    m_wndNaviBar;

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAnsichtSudokuleiste();
	afx_msg void OnUpdateAnsichtSudokuleiste(CCmdUI *pCmdUI);
	afx_msg void OnAnsichtMousepad();
	afx_msg void OnUpdateAnsichtMousepad(CCmdUI *pCmdUI);
	afx_msg void OnMousepad(UINT nID);
	afx_msg void OnNavigation(UINT nID);
    afx_msg LRESULT OnNotifyEditField(WPARAM, LPARAM);
    afx_msg LRESULT OnSwitchBoard(WPARAM, LPARAM);
	afx_msg void OnAnsichtCursorbar();
	afx_msg void OnUpdateAnsichtCursorbar(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
	BOOL CreateMousePad(CMousePad&, int, UINT);
	CWnd* m_pFocusEdit;
};


