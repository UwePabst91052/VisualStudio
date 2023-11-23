
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "cdmfcactivexcontrol1.h"


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	void OnHabeGewuerfelt(int zahl);
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CDMFCActiveXControl1 m_wuerfel;
private:
	static const COLORREF s_color_rot;
	static const COLORREF s_color_gruen;
	static const COLORREF s_color_gelb;
	static const COLORREF s_color_schwarz;
	COLORREF m_col_back;
	COLORREF m_col_fore;
public:
	afx_msg void OnColorsBackground();
	afx_msg void OnForegroundRot();
	afx_msg void OnForegroundGruen();
	afx_msg void OnForegroundGelb();
	afx_msg void OnForegroundSchwarz();
	afx_msg void OnEditWurf();
};

