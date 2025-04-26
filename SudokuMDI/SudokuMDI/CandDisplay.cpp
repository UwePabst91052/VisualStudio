// CandDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "CandDisplay.h"


// CCandDisplay

IMPLEMENT_DYNAMIC(CCandDisplay, CStatic)

CCandDisplay::CCandDisplay()
: mySingleInFields(0)
, mySingleOnBoard(0)
, myTitle1(_T(""))
, myTitle2(_T(""))
, m_backColor(RGB(255,220,157))
{
	myTitle1.LoadString(IDS_STRING_SIF);
	myTitle2.LoadString(IDS_STRING_SOB);
}

CCandDisplay::~CCandDisplay()
{
}


BEGIN_MESSAGE_MAP(CCandDisplay, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCandDisplay message handlers



void CCandDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, m_backColor);

	CString szTitle;
	szTitle.Format(_T("%s % 3d   %s % 3d"), myTitle1, mySingleInFields, myTitle2, mySingleOnBoard);

	dc.DrawText(szTitle, rcClient, DT_LEFT);

	// Do not call CStatic::OnPaint() for painting messages
}

void CCandDisplay::SetSingleCounters(int nSingleInFields, int nSingleOnBoard)
{
	mySingleInFields = nSingleInFields;
	mySingleOnBoard  = nSingleOnBoard;

	UpdateWindow();
}

void CCandDisplay::SetBackgroundColor(COLORREF backColor)
{
	m_backColor = backColor;
}
