// SudokoEdit.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokoEdit.h"
#include ".\sudokoedit.h"

static UINT   g_nId = 0xffffffff;

// CSudokoEdit

IMPLEMENT_SERIAL(CSudokoEdit, CEdit, VERSIONABLE_SCHEMA | 1)

CSudokoEdit::CSudokoEdit()
: m_bSelected(false)
, myNumber(0)
, myOldNumber(0)
, myScanNumber(0)
, m_pParent(NULL)
, m_nId(0)
, myInputState(isEmpty)
, m_nTimerId(5)
, myBoardDim(9)
, m_bSuspPaint(true)
, m_bDisplayAlpha(TRUE)
, m_bSolveActive(false)
, m_backColor(RGB(255,220,157))
{
}

CSudokoEdit::~CSudokoEdit()
{
}


BEGIN_MESSAGE_MAP(CSudokoEdit, CEdit)
	//{{AFX_MSG_MAP(CSudokuEdit)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CSudokoEdit-Meldungshandler


BOOL CSudokoEdit::Create(const RECT& rect, CWnd* pParentWnd, int nBoardDim, UINT nID)
{
	DWORD dwStyle = WS_CHILD | ES_READONLY | ES_CENTER | ES_NUMBER;
	BOOL bRet = CEdit::Create(dwStyle, rect, pParentWnd, nID);

	m_pParent = pParentWnd;
	m_rcClient = rect;
    myBoardDim = nBoardDim;
	m_nId = nID;

	return bRet;
}


void CSudokoEdit::OnPaint()
{
	CPaintDC  dc(this);

    if (m_bSuspPaint) return;

	CRect rcClient;
	GetClientRect(&rcClient);

    dc.SetBkColor(m_backColor);

    if (myNumber && (myScanNumber == myNumber))
    {
        dc.SetTextColor(RGB(255,0,0));
    }
    else if (m_bSolveActive)
	{
        dc.SetTextColor(RGB(0,0,255));
	}
	else
    {
        dc.SetTextColor(RGB(0,0,0));
    }

	CFont* pFont = dc.GetCurrentFont();
	CFont ftNumbers;
	LOGFONT lfNumbers;
	if (pFont) pFont->GetLogFont(&lfNumbers);
	lfNumbers.lfHeight = m_rcClient.Height() - 4;
	lfNumbers.lfWidth  = lfNumbers.lfHeight / 2;
	wcscpy_s((wchar_t*)&lfNumbers.lfFaceName, 6, _T("Arial"));
	ftNumbers.CreateFontIndirect(&lfNumbers);
	pFont = dc.SelectObject(&ftNumbers);

	if (myNumber)
	{
		CString szNumber;
        if (myBoardDim > 8 && m_bDisplayAlpha)
        {
		    szNumber.Format(_T("%c"), myNumber + 'A' - 1);
        }
        else
        {
		    szNumber.Format(_T("%.1d"), myNumber);
        }
		dc.DrawText(szNumber, -1, rcClient, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
	}
    else if (myInputState == isEnteredOne)
    {
		dc.DrawText(_T("1_"), 2, rcClient, DT_SINGLELINE | DT_CENTER |DT_VCENTER);
    }

    if (g_nId == m_nId)
    {
        TRACE1("OnPaint: m_bSelected = %s\n", m_bSelected ? L"true" : L"false");
    }
	if (m_bSelected)
	{
		dc.DrawFocusRect(&rcClient);
	}

	dc.SelectObject(pFont);

	// CEdit::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

void CSudokoEdit::OnSetFocus(CWnd* pOldWnd)
{
	m_bSelected = true;
    g_nId = m_nId;
	AfxGetMainWnd()->SendMessage(WM_NOTIFY_EDIT_FIELD, (WPARAM)this, (LPARAM)m_nId);
    pOldWnd->Invalidate();
	Invalidate();
	UpdateWindow();
}

void CSudokoEdit::OnKillFocus(CWnd* pNewWnd)
{
	m_bSelected = false;
    g_nId = 0xffffffff;

    if (myInputState == isEnteredOne)
    {
        SignalNewNumber(1, false);
        KillTimer(m_nTimerId);
    }

	AfxGetMainWnd()->SendMessage(WM_NOTIFY_EDIT_FIELD, (WPARAM)NULL);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokoEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (::isdigit(nChar))
	{
		char cDigit[2];
		cDigit[0] = (char) nChar;
		cDigit[1] = 0;

        EvaluateDigit(cDigit);
	}
	else if (::isalpha(nChar))
	{
		int nNumber = ::toupper(nChar) - 'A' + 1;
		SignalNewNumber(nNumber, false);
	}
}

void CSudokoEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bSend = false;
	WPARAM wParam;
	LPARAM lParam = m_nId;
	UINT uiMsg;
	switch (nChar)
	{
	case VK_DELETE:
        myInputState = isEmpty;
		uiMsg = WM_NEW_NUMBER;
		wParam = 0;
		bSend = true;
		Invalidate();
		UpdateWindow();
		break;
	case VK_RIGHT:
		uiMsg = WM_MOVE_CURSOR;
		wParam = MOVE_DIR_RIGHT;
		bSend = true;
		break;
	case VK_DOWN:
		uiMsg = WM_MOVE_CURSOR;
		wParam = MOVE_DIR_DOWN;
		bSend = true;
		break;
	case VK_LEFT:
		uiMsg = WM_MOVE_CURSOR;
		wParam = MOVE_DIR_LEFT;
		bSend = true;
		break;
	case VK_UP:
		uiMsg = WM_MOVE_CURSOR;
		wParam = MOVE_DIR_UP;
		bSend = true;
		break;
	default:
		break;
	}
	if (bSend)
	{
		m_pParent->SendMessage(uiMsg, wParam, lParam);
	}

    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSudokoEdit::Serialize(CArchive &ar)
{
    if (ar.IsStoring())
    {
        ar << m_nId
           << m_rcClient
           << myNumber
           << myOldNumber;
    }
    else
    {
        ar >> m_nId
           >> m_rcClient
           >> myNumber
           >> myOldNumber;
    }
}

void CSudokoEdit::OnTimer(UINT nIDEvent) 
{
    if (m_nTimerId == nIDEvent)
    {
        SignalNewNumber(1, false);
        KillTimer(nIDEvent);
    }
	
	CEdit::OnTimer(nIDEvent);
}

void CSudokoEdit::EvaluateDigit(char* cDigit)
{
    int nNumber = ::atoi(cDigit);
    switch (myInputState)
    {
    case isEmpty:
    case isValidNumber:
        if ((nNumber == 1) && (myBoardDim > 9))
        {
            myInputState = isEnteredOne;
            SetTimer(m_nTimerId, 1500, NULL);
            Invalidate();
            UpdateWindow();
        }
        else
        {
            SignalNewNumber(nNumber, false);
        }
        break;
    case isEnteredOne:
        nNumber += 10;
        SignalNewNumber(nNumber, false);
        KillTimer(m_nTimerId);
        break;
    }
}

void CSudokoEdit::SignalNewNumber(int nNumber, bool bSolveActive)
{
	myInputState = isValidNumber;
	myOldNumber = myNumber;
	myNumber = nNumber;
	m_pParent->SendMessage(WM_NEW_NUMBER, (WPARAM)nNumber, (LPARAM)m_nId);

	m_bSolveActive = bSolveActive;
}

void CSudokoEdit::ShowWindow(int nCmdShow)
{
    CEdit::ShowWindow(nCmdShow);
    m_bSuspPaint = nCmdShow == SW_HIDE ? true : false;
}

void CSudokoEdit::SetProperties(BOOL bDisplayAlpha, CRect rcEdit, COLORREF backColor)
{
	m_bDisplayAlpha = bDisplayAlpha;
	m_rcClient = rcEdit;
	m_backColor = backColor;
}
