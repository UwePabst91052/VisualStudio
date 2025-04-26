// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "SudokuMDI.h"

#include "MainFrm.h"
#include "SudokuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_ANSICHT_SUDOKULEISTE, &CMainFrame::OnAnsichtSudokuleiste)
	ON_UPDATE_COMMAND_UI(ID_ANSICHT_SUDOKULEISTE, &CMainFrame::OnUpdateAnsichtSudokuleiste)
	ON_COMMAND(ID_ANSICHT_MOUSEPAD, &CMainFrame::OnAnsichtMousepad)
	ON_UPDATE_COMMAND_UI(ID_ANSICHT_MOUSEPAD, &CMainFrame::OnUpdateAnsichtMousepad)
	ON_COMMAND(ID_ANSICHT_CURSORBAR, &CMainFrame::OnAnsichtCursorbar)
	ON_UPDATE_COMMAND_UI(ID_ANSICHT_CURSORBAR, &CMainFrame::OnUpdateAnsichtCursorbar)
    ON_UPDATE_COMMAND_UI(ID_ANSICHT_MOVEBOARD, OnUpdateControlBarMenu)
    ON_COMMAND_EX(ID_ANSICHT_MOVEBOARD, OnBarCheck)
    ON_COMMAND_RANGE(ID_MOUSEPAD_1, ID_MOUSEPAD_16, OnMousepad)
    ON_COMMAND_RANGE(ID_NAV_LEFT, ID_NAV_RIGHT, OnNavigation)
    ON_MESSAGE(WM_NOTIFY_EDIT_FIELD, OnNotifyEditField)
    ON_MESSAGE(WM_SWITCH_BOARD, OnSwitchBoard)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT BASED_CODE mousepad[] =
{
	// same order as in the bitmap 'toolbar1.bmp'
	ID_MOUSEPAD_1,
	ID_MOUSEPAD_2,
	ID_MOUSEPAD_3,
	ID_MOUSEPAD_4,
	ID_MOUSEPAD_5,
	ID_MOUSEPAD_6,
	ID_MOUSEPAD_7,
	ID_MOUSEPAD_8,
	ID_MOUSEPAD_9,
	ID_MOUSEPAD_10,
	ID_MOUSEPAD_11,
	ID_MOUSEPAD_12,
	ID_MOUSEPAD_13,
	ID_MOUSEPAD_14,
	ID_MOUSEPAD_15,
	ID_MOUSEPAD_16,
};

// CMainFrame-Erstellung/Zerstörung

CMainFrame::CMainFrame()
:
 m_pFocusEdit(NULL)
{
	// TODO: Hier Code für die Memberinitialisierung einfügen
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Fehler beim Erstellen der Symbolleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	if (!m_wndMoveBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
	    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ID_ANSICHT_MOVEBOARD) ||
		!m_wndMoveBar.LoadToolBar(IDR_MOVEBAR))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
 
	if (!m_wndNaviBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
	    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), ID_ANSICHT_CURSORBAR) ||
		!m_wndNaviBar.LoadToolBar(IDR_NAVIBAR))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	if (!m_wndSudokuBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0),ID_ANSICHT_SUDOKULEISTE) ||
		!m_wndSudokuBar.LoadToolBar(IDR_SudokuSquareTYPE))
	{
		TRACE0("Fehler beim Erstellen der Symbolleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Fehler beim Erstellen der Statusleiste.\n");
		return -1;      // Fehler beim Erstellen
	}
	EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndSudokuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf(&m_wndSudokuBar, &m_wndToolBar);

    CreateMousePad(m_wndMousePad9x,   9, IDB_BMP_MOUSEPAD);
    CreateMousePad(m_wndMousePad12x, 12, IDB_BMP_MOUSEPAD);
    CreateMousePad(m_wndMousePad16x, 16, IDB_BMP_MOUSEPAD);
    CreateMousePad(m_wndMousePadAlpha12x, 12, IDB_BMP_MOUSEPAD_ALPHA);
    CreateMousePad(m_wndMousePadAlpha16x, 16, IDB_BMP_MOUSEPAD_ALPHA);

	m_wndNaviBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT);
	m_wndNaviBar.SetWindowText(_T("Navigation"));
	FloatControlBar(&m_wndNaviBar, CPoint(400,300));
	
	m_wndMoveBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMoveBar.SetWindowText(_T("Move Board"));
	DockControlBar(&m_wndMoveBar);
    DockControlBarLeftOf(&m_wndMoveBar, &m_wndSudokuBar);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX /*| WS_MAXIMIZE*/ | WS_SYSMENU;

	return TRUE;
}

LRESULT CMainFrame::OnNotifyEditField(WPARAM wParam, LPARAM lParam)
{
    CWnd* pWnd = (CWnd*)wParam;
	m_pFocusEdit = pWnd;
    if (m_wndMousePad9x.IsVisible())
    {
        m_wndMousePad9x.UpdateEditField(pWnd);
    }
    else if (m_wndMousePad12x.IsVisible() ||
		     m_wndMousePadAlpha12x.IsVisible())
    {
        m_wndMousePad12x.UpdateEditField(pWnd);
    }
    else if (m_wndMousePad16x.IsVisible() ||
		     m_wndMousePadAlpha16x.IsVisible())
    {
        m_wndMousePad16x.UpdateEditField(pWnd);
    }
    return TRUE;
}


void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

BOOL CMainFrame::CreateMousePad(CMousePad& wndMousePad, int noOfKeys, UINT nBitmapId)
{
	if (!wndMousePad.Create(this, WS_CHILD | CBRS_SIZE_FIXED |
		CBRS_TOP | CBRS_TOOLTIPS, ID_ANSICHT_MOUSEPAD) ||
		!wndMousePad.LoadBitmap(nBitmapId) ||
		!wndMousePad.SetButtons(mousepad,
		  noOfKeys))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;       // fail to create
	}

	wndMousePad.SetWindowText(_T("Mousepad"));
	wndMousePad.EnableDocking(0);

	// Create the Mousepad.  We are using hardcoded numbers for ease here
	// normally the location would be read in from an ini file.
	CPoint pt(GetSystemMetrics(SM_CXSCREEN) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 3);

    wndMousePad.InitLayout();
	FloatControlBar(&wndMousePad, pt);

	return TRUE;
}

// CMainFrame-Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame-Meldungshandler




void CMainFrame::OnAnsichtSudokuleiste()
{
	BOOL bVisible = m_wndSudokuBar.IsVisible();
	m_wndSudokuBar.ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
}

void CMainFrame::OnUpdateAnsichtSudokuleiste(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndSudokuBar.IsVisible());
}


void CMainFrame::OnAnsichtMousepad()
{
    CMousePad* pNewMousePad = &m_wndMousePad9x;

	CFrameWnd* pFrame = GetActiveFrame();
    CSudokuDoc* pDoc = (CSudokuDoc*)pFrame->GetActiveDocument();
	SudokuProps_t BoardProperties;
	if (!pDoc) return;
	pDoc->GetBoardProperties(BoardProperties);

	BoardType_t boardType = pDoc->GetBoardType();
    if (boardType == btSquare12x12)
    {
		if (BoardProperties.myDisplayAlpha)
			pNewMousePad = &m_wndMousePadAlpha12x;
		else
			pNewMousePad = &m_wndMousePad12x;
    }
    else if (boardType == btSquare16x16)
    {
		if (BoardProperties.myDisplayAlpha)
			pNewMousePad = &m_wndMousePadAlpha16x;
		else
			pNewMousePad = &m_wndMousePad16x;
    }

	BOOL bVisible = pNewMousePad->IsVisible();

	ShowControlBar(pNewMousePad, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateAnsichtMousepad(CCmdUI *pCmdUI)
{
	BOOL bVisible = (m_wndMousePad9x.IsVisible() || 
		             m_wndMousePad12x.IsVisible() || 
					 m_wndMousePadAlpha12x.IsVisible() ||
					 m_wndMousePad16x.IsVisible() ||
					 m_wndMousePadAlpha16x.IsVisible());
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnMousepad(UINT nID)
{
    char c = (char)(nID - ID_MOUSEPAD_1);
    if (m_wndMousePad9x.IsVisible())
    {
        c += 0x31;
        m_wndMousePad9x.MousePadClicked(c);
    }
    else if (m_wndMousePad12x.IsVisible() ||
             m_wndMousePad16x.IsVisible() ||
			 m_wndMousePadAlpha12x.IsVisible() ||
			 m_wndMousePadAlpha16x.IsVisible())
    {
        c += 0x41;
        m_wndMousePad12x.MousePadClicked(c);
    }
}

void CMainFrame::OnNavigation(UINT nID)
{
	WPARAM wParam;

	if (m_pFocusEdit)
	{
		switch (nID)
		{
		case ID_NAV_LEFT:
			wParam = VK_LEFT;
			break;
		case ID_NAV_UP:
			wParam = VK_UP;
			break;
		case ID_NAV_DOWN:
			wParam = VK_DOWN;
			break;
		case ID_NAV_RIGHT:
			wParam = VK_RIGHT;
			break;
		}
		m_pFocusEdit->SendMessage(WM_KEYDOWN, wParam);
	}
}

LRESULT CMainFrame::OnSwitchBoard(WPARAM wParam, LPARAM)
{
    CMousePad* pCurMousePad = NULL;
	CFrameWnd* pFrame = GetActiveFrame();
	SudokuProps_t BoardProperties;
    CSudokuDoc* pDoc = (CSudokuDoc*)pFrame->GetActiveDocument();
	if (!pDoc) return FALSE;

    pCurMousePad =  m_wndMousePad9x.IsVisible()  ? &m_wndMousePad9x :
                    m_wndMousePad12x.IsVisible() ? &m_wndMousePad12x :
                    m_wndMousePadAlpha12x.IsVisible() ? &m_wndMousePadAlpha12x :
                    m_wndMousePad16x.IsVisible() ? &m_wndMousePad16x :
					m_wndMousePadAlpha16x.IsVisible() ? &m_wndMousePadAlpha16x : NULL;;

    CMousePad* pNewMousePad = &m_wndMousePad9x;
	BoardType_t boardType = pDoc->GetBoardType();
	pDoc->GetBoardProperties(BoardProperties);
    if (boardType == btSquare12x12)
    {
		if (BoardProperties.myDisplayAlpha)
			pNewMousePad = &m_wndMousePadAlpha12x;
		else
			pNewMousePad = &m_wndMousePad12x;
    }
    else if (boardType == btSquare16x16)
    {
		if (BoardProperties.myDisplayAlpha)
			pNewMousePad = &m_wndMousePadAlpha16x;
		else
			pNewMousePad = &m_wndMousePad16x;
    }

    if (pCurMousePad && (pCurMousePad != pNewMousePad))
    {
        ShowControlBar(pCurMousePad, FALSE, FALSE);
	    ShowControlBar(pNewMousePad, TRUE, FALSE);
    }

	RecalcLayout();
    return TRUE;
}

void CMainFrame::OnAnsichtCursorbar()
{
	BOOL bVisible = m_wndNaviBar.IsVisible();
	ShowControlBar(&m_wndNaviBar, !bVisible, FALSE);
}

void CMainFrame::OnUpdateAnsichtCursorbar(CCmdUI *pCmdUI)
{
	BOOL bVisible = m_wndNaviBar.IsVisible();
	pCmdUI->SetCheck(bVisible);
}
