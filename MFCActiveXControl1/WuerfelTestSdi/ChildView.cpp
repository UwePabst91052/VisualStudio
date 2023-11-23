
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "WuerfelTestSdi.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const COLORREF CChildView::s_color_gelb = RGB(255,255,0); 
const COLORREF CChildView::s_color_gruen = RGB(0,255,0); 
const COLORREF CChildView::s_color_rot = RGB(255,0,0); 
const COLORREF CChildView::s_color_schwarz = RGB(0,0,0); 

// CChildView

CChildView::CChildView()
	: m_col_back(RGB(128,128,128))
	, m_col_fore(s_color_rot)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_COLORS_BACKGROUND, &CChildView::OnColorsBackground)
	ON_COMMAND(ID_FOREGROUND_ROT, &CChildView::OnForegroundRot)
	ON_COMMAND(ID_FOREGROUND_GRUEN, &CChildView::OnForegroundGruen)
	ON_COMMAND(ID_FOREGROUND_GELB, &CChildView::OnForegroundGelb)
	ON_COMMAND(ID_FOREGROUND_SCHWARZ, &CChildView::OnForegroundSchwarz)
	ON_COMMAND(ID_EDIT_WURF, &CChildView::OnEditWurf)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CChildView, CView)
	ON_EVENT(CChildView, 1000, 1, OnHabeGewuerfelt, VTS_I4)
END_EVENTSINK_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;

	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, m_col_back);
	m_wuerfel.SetProperty(DISPID_BACKCOLOR, VT_COLOR, m_col_back);
	m_wuerfel.SetProperty(DISPID_FORECOLOR, VT_COLOR, m_col_fore);
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wuerfel.Create(L"WUERFEL_CLASS", L"Wuerfel", WS_VISIBLE | WS_CHILD, CRect(100, 100, 250, 250), this, 1000);	
	return 0;
}


void CChildView::OnColorsBackground()
{
	CColorDialog dlg;

	dlg.DoModal();

	m_col_back = dlg.GetColor();
	Invalidate();
	UpdateWindow();
}


void CChildView::OnForegroundRot()
{
	m_col_fore = s_color_rot;
	Invalidate();
	UpdateWindow();
}


void CChildView::OnForegroundGruen()
{
	m_col_fore = s_color_gruen;
	Invalidate();
	UpdateWindow();
}


void CChildView::OnForegroundGelb()
{
	m_col_fore = s_color_gelb;
	Invalidate();
	UpdateWindow();
}


void CChildView::OnForegroundSchwarz()
{
	m_col_fore = s_color_schwarz;
	Invalidate();
	UpdateWindow();
}


void CChildView::OnEditWurf()
{
	m_wuerfel.WuerfleZahl();
}

void CChildView::OnHabeGewuerfelt(int zahl)
{
	TRACE1("Gewuerfelte Zahl: %d\n", zahl);
}