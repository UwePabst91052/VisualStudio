// SudokuScore.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuScore.h"
#include "SudokuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCORE_VERT_INDENT 40

/////////////////////////////////////////////////////////////////////////////
// CSudokuScore

CSudokuScore::CSudokuScore()
 :
 myBoardDim(9)
,myCandComplete(9)
,m_bSortDisplayLines(FALSE)
,m_bAllowMultiSelect(FALSE)
,m_bFocusIsOn(false)
,myDisplayAlpha(FALSE)
,m_pBrush(NULL)
,m_backColor(RGB(255,220,157))
,m_fillColor(RGB(0,200,200))
 {
    for (int i=0; i<16; i++)
    {
        myDisplayLines[i].ucCand = i+1;
        myDisplayLines[i].nCount = 0;
        myDisplayLines[i].bCompleteFlag = false;
        myDisplayLines[i].bFocus = false;
    }
}

CSudokuScore::~CSudokuScore()
{
	if (m_pBrush) delete m_pBrush;
}


BEGIN_MESSAGE_MAP(CSudokuScore, CWnd)
	//{{AFX_MSG_MAP(CSudokuScore)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
    ON_MESSAGE(WM_NOTIFY_SORT_BUTTON, OnSortButtonClicked)
    ON_MESSAGE(WM_NOTIFY_MULTI_SELECT, OnMultiSelectButtonClicked)

ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSudokuScore 

BOOL CSudokuScore::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
    int      nVertPos = 0;

	m_pBrush = new CBrush;
    m_pBrush->CreateSolidBrush(m_fillColor);

    m_rcSortButton = CRect(20,0,rect.right, 20);
    m_rcMultiSelectButton = CRect(20,20,rect.right, 40);
    BOOL bRet  = CWnd::Create(NULL, L"Scorelist", dwStyle, rect, pParentWnd, nID);
         bRet &= mySortCheckBox.Create(L"Sort", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFT, 
                                       m_rcSortButton, this, 
                                       IDC_BUTTON_CHECK_SORT) &
                 myMultiSelectCheckBox.Create(L"Multi", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFT, 
                                              m_rcMultiSelectButton, this, 
                                              IDC_BUTTON_CHECK_MULTI_SELECT);
    mySortCheckBox.SetCheck(m_bSortDisplayLines);
    myMultiSelectCheckBox.SetCheck(m_bAllowMultiSelect);

    CString szMsg;
    OutputDebugString(_T("CSudokuScore::Create()\n"));
    szMsg.Format(_T("Create returns %s\n"), bRet ? L"TRUE" : L"FALSE");
    OutputDebugString(szMsg);
    szMsg.Format(_T("this = 0x%x, mySortCheckBox.m_hWnd = 0x%x\n\n"), this, mySortCheckBox.m_hWnd);
    OutputDebugString(szMsg);

    return bRet;
}

void CSudokuScore::UpdateScore(int *pCandCounts)
{
    CString szLabel;
	int i;

    // copy new count values
    for (i=0; i<myBoardDim; i++)
    {
        int nCand = myDisplayLines[i].ucCand;
        myDisplayLines[i].nCount = pCandCounts[nCand-1];
    }

    // update label and check candidate completion
    for (i=0; i<myBoardDim; i++)
    {
        // check candidate completion
        if (myDisplayLines[i].nCount == myCandComplete)
        {
            myDisplayLines[i].bCompleteFlag = true;
        }
        else
        {
            myDisplayLines[i].bCompleteFlag = false;
        }

        // deselect scan candidates on sudoku board if candidate is completed
        if (myDisplayLines[i].bCompleteFlag && myDisplayLines[i].bFocus)
        {
            myDisplayLines[i].bFocus = false;
            m_bFocusIsOn = false;
			CView* pView = (CView*)GetParent();
			CSudokuDoc* pDoc = (CSudokuDoc*)pView->GetDocument();
            pDoc->SelectScan(myDisplayLines[i].ucCand, false, m_bAllowMultiSelect);
            SelectCandidatesScan(myDisplayLines[i].ucCand, false);
        }
    }

    Invalidate();
    UpdateWindow();
}

int CSudokuScore::GetCount(int nIndex)
{
    return myDisplayLines[nIndex].nCount;
}

void CSudokuScore::SelectCandidatesScan(int c, bool bSelect)
{
    myDisplayLines[c-1].bFocus = bSelect;
    Invalidate();
    UpdateWindow();
}

void CSudokuScore::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    int nVertPos = SCORE_VERT_INDENT;
    int nIndex;
	
    CFont* pOldFont = dc.GetCurrentFont();
    LOGFONT  lf;
    pOldFont->GetLogFont(&lf);
    // create label font
    lf.lfHeight = 16;
    lf.lfWidth = 9;
    wcscpy_s(lf.lfFaceName, 6, L"Arial");
    CFont  ftLabel;
    ftLabel.CreateFontIndirect(&lf);
    // create candidate font
    CFont  ftCandidate;
    lf.lfHeight = 24;
    lf.lfWidth = 13;
    ftCandidate.CreateFontIndirect(&lf);

    // create completion pen
    CPen penCross(PS_SOLID, 2, RGB(0,0,0));
    CPen* pOldPen;

    CString szLabel;
    CString szCand;
    CRect rcClient;
    CPoint ptFocus[5];
    GetClientRect(&rcClient);

    dc.FillSolidRect(&rcClient, m_backColor);

    // get extensions of candidate rectangle
    pOldFont = dc.SelectObject(&ftCandidate);
    CSize szText = dc.GetTextExtent(L"12");
    CRect rcCross(10, nVertPos, szText.cx+10, nVertPos+szText.cy);

    // calculate focus rectangle, vertical moving offset and label length
    CRect rcFocus(5, nVertPos, rcClient.Width()-10, nVertPos+szText.cy);
    m_nVertOffset = szText.cy + 5;        
    int nLabelSize = rcClient.Width() - szText.cx - 10;
    int nCandSum = 0;
    int nScoreHeight = nVertPos + (myBoardDim+2) * m_nVertOffset;
    if (nScoreHeight < rcClient.Height())
    {
        rcClient.DeflateRect(0, 0, 0, rcClient.Height() - nScoreHeight);
    }
    dc.FillSolidRect(&rcClient, m_fillColor);

    // sort the score list
    if (m_bSortDisplayLines && !m_bFocusIsOn)
    {
    #if 0
        QuickSort(myDisplayLines, myBoardDim);
    #else
        BubbleSort(myDisplayLines, myBoardDim);
    #endif
    }

    // draw score list
    for (nIndex = 0; nIndex < myBoardDim; nIndex++)
    {
        // draw candidate
        dc.SetTextColor(RGB(255,0,0));
        if (myBoardDim > 8 && myDisplayAlpha)
        {
            szCand.Format(L"%c", myDisplayLines[nIndex].ucCand + L'A' - 1);
        }
        else
        {
            szCand.Format(L"%d", myDisplayLines[nIndex].ucCand);
        }
        dc.DrawText(szCand, CRect(10, nVertPos, szText.cx+10, nVertPos+szText.cy), DT_CENTER | DT_VCENTER);

        // draw label
        dc.SetTextColor(RGB(0,0,0));
        dc.SelectObject(&ftLabel);
        szLabel.Format(L" = %d", myDisplayLines[nIndex].nCount);
        dc.DrawText(szLabel, CRect(szText.cx+10, nVertPos, szText.cx+nLabelSize+10, nVertPos+szText.cy), DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // draw focus if active
        if (myDisplayLines[nIndex].bFocus )
        {
            ptFocus[0] = CPoint(rcFocus.left,  rcFocus.top);
            ptFocus[1] = CPoint(rcFocus.right, rcFocus.top);
            ptFocus[2] = CPoint(rcFocus.right, rcFocus.bottom);
            ptFocus[3] = CPoint(rcFocus.left,  rcFocus.bottom);
            ptFocus[4] = CPoint(rcFocus.left,  rcFocus.top);
            dc.Polyline(ptFocus, 5);
        }
        rcFocus.OffsetRect(0, m_nVertOffset);

        // draw completion crossing if necessary
        if (myDisplayLines[nIndex].bCompleteFlag)
        {
            pOldPen = dc.SelectObject(&penCross);
            CRect rcTemp(rcCross);
            rcTemp.OffsetRect(0, nIndex * m_nVertOffset);
            dc.MoveTo(rcTemp.left, rcTemp.bottom);
            dc.LineTo(rcTemp.right, rcTemp.top);
            dc.MoveTo(rcTemp.left, rcTemp.top);
            dc.LineTo(rcTemp.right, rcTemp.bottom);
            dc.SelectObject(pOldPen);
        }

        dc.SelectObject(&ftCandidate);
        nVertPos += m_nVertOffset;
        nCandSum += myDisplayLines[nIndex].nCount;
    }

    // draw candidate sum
    dc.MoveTo(0, nVertPos);
    dc.LineTo(rcClient.right, nVertPos);
    dc.MoveTo(0, nVertPos+2);
    dc.LineTo(rcClient.right, nVertPos+2);
    rcFocus.top = nVertPos+6;
    rcFocus.bottom = rcFocus.top + szText.cy;
    szLabel.Format(L"%d", nCandSum);
    dc.DrawText(szLabel, rcFocus, DT_CENTER);

    // restore DC
    dc.SelectObject(pOldFont);
    ftLabel.DeleteObject();

    OutputDebugString(_T("CSudokuScore::OnPaint()\n"));
    CString szMsg;
    szMsg.Format(_T("this = 0x%x, mySortCheckBox.m_hWnd = 0x%x\n\n"), this, mySortCheckBox.m_hWnd);
    OutputDebugString(szMsg);

    // update sort checkbox
    if (mySortCheckBox.m_hWnd)
    {
        mySortCheckBox.Invalidate();
        mySortCheckBox.UpdateWindow();
    }
    // update multi select checkbox
    if (myMultiSelectCheckBox.m_hWnd)
    {
        myMultiSelectCheckBox.Invalidate();
        myMultiSelectCheckBox.UpdateWindow();
    }

	// Kein Aufruf von CWnd::OnPaint() für Zeichnungsnachrichten
}

void CSudokuScore::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::SetFocus();
    int nOldFocusIndex;

    // calculate old focus index
    for (nOldFocusIndex = 0; 
         !myDisplayLines[nOldFocusIndex].bFocus && nOldFocusIndex < myBoardDim;
         nOldFocusIndex++);
    // calculate new focus index
    int nFocusIndex = (point.y - SCORE_VERT_INDENT) / m_nVertOffset;


    // check completion
    if (myDisplayLines[nFocusIndex].bCompleteFlag)
    {
        // no action
        return;
    }
    else
    {
        if (nOldFocusIndex < myBoardDim && 
            nOldFocusIndex != nFocusIndex &&
            !m_bAllowMultiSelect)
        {
            // clear old focus if one
            myDisplayLines[nOldFocusIndex].bFocus = false;
        }
        if (!myDisplayLines[nFocusIndex].bFocus)
        {
            // set new focus
            myDisplayLines[nFocusIndex].bFocus = true;
            m_bFocusIsOn = true;
        }
        else
        {
            // clear focus
            myDisplayLines[nFocusIndex].bFocus = false;
            m_bFocusIsOn = false;
            for (int i=0; i<myBoardDim; i++)
            {
                m_bFocusIsOn |= myDisplayLines[i].bFocus;
            }
        }
    }

    // select scan candidates on sudoku board
	CView* pView = (CView*)GetParent();
	CSudokuDoc* pDoc = (CSudokuDoc*)pView->GetDocument();
    pDoc->SelectScan(myDisplayLines[nFocusIndex].ucCand, myDisplayLines[nFocusIndex].bFocus, m_bAllowMultiSelect);

    Invalidate();
    UpdateWindow();

	CWnd::OnLButtonDown(nFlags, point);
}

void CSudokuScore::ResizeScoreWindow(int nCandComplete, int nBoardDim)
{
	myCandComplete = nCandComplete;
    myBoardDim = nBoardDim;
    Invalidate();
    UpdateWindow();
}

void CSudokuScore::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nFocusIndex;

    for (nFocusIndex = 0; 
         !myDisplayLines[nFocusIndex].bFocus && nFocusIndex < myBoardDim;
         nFocusIndex++);

	if (nFocusIndex < myBoardDim)
	{
        myDisplayLines[nFocusIndex].bFocus = false;
		if (VK_DOWN == nChar)
		{
			do
			{
				nFocusIndex++;
				if (nFocusIndex >= myBoardDim)
				{
					nFocusIndex = 0;
				}
			}
            while(myDisplayLines[nFocusIndex].bCompleteFlag);
		}
		else if (VK_UP == nChar)
		{
			do
			{
				nFocusIndex--;
				if (nFocusIndex < 0)
				{
					nFocusIndex = myBoardDim - 1;
				}
			}
            while(myDisplayLines[nFocusIndex].bCompleteFlag);
		}
        myDisplayLines[nFocusIndex].bFocus = true;

		CView* pView = (CView*)GetParent();
		CSudokuDoc* pDoc = (CSudokuDoc*)pView->GetDocument();

        // select scan candidates on sudoku board
        pDoc->SelectScan(myDisplayLines[nFocusIndex].ucCand, myDisplayLines[nFocusIndex].bFocus, m_bAllowMultiSelect);

        Invalidate();
        UpdateWindow();
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSudokuScore::BubbleSort(DisplayLine_t* pDisplayLines, int len)
{
	for(int i=0; i < len; i++)
	{
		for(int j=1; j < len-i; j++)
        {
            if(pDisplayLines[j].nCount > pDisplayLines[j-1].nCount)
            {
                swap (pDisplayLines[j], pDisplayLines[j-1]);
            }
        }
	}
}

void CSudokuScore::QuickSort(DisplayLine_t* pDisplayLines, int len)
{
    int pivot;
    if (len <= 1)
    {
        return;
    }
    pivot = Partion(pDisplayLines, len);
    QuickSort(pDisplayLines, pivot);
    QuickSort(pDisplayLines + pivot + 1, len - pivot - 1);
}

// helper function for quick sort function
int CSudokuScore::Partion(DisplayLine_t* pDisplayLines, int len)
{
    DisplayLine_t temp = pDisplayLines[0];
    int i = 0;
    int j = len - 1;

    while (i < j)
    {

        while (pDisplayLines[j].nCount > temp.nCount && i < j)
        {
            j--;
        }
        if (i >= j)
        {
            break;
        }

        while (pDisplayLines[i].nCount < temp.nCount && i < j)
        {
            i++;
        }
        if (i >= j)
        {
            break;
        }
        swap(pDisplayLines[i], pDisplayLines[j]);
        break;
    }

    swap(pDisplayLines[i], temp);

    return i;
}

HBRUSH CSudokuScore::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

   // Are we painting the IDC_MYSTATIC control? We can use
   // CWnd::GetDlgCtrlID() to perform the most efficient test.
   if (pWnd->GetDlgCtrlID() == IDC_BUTTON_CHECK_SORT ||
       pWnd->GetDlgCtrlID() == IDC_BUTTON_CHECK_MULTI_SELECT)
   {
      // Set the text color to red
      pDC->SetTextColor(RGB(0, 0, 0));

      // Set the background mode for text to transparent 
      // so background will show thru.
      pDC->SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = (HBRUSH)(*m_pBrush);
   }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

LRESULT CSudokuScore::OnSortButtonClicked(WPARAM, LPARAM)
{
    if (m_bSortDisplayLines = mySortCheckBox.GetCheck())
    {
        BubbleSort(myDisplayLines, myBoardDim);
    }
    else
    {
        UndoSort(myDisplayLines, myBoardDim);
    }

    Invalidate();
    UpdateWindow();

    return TRUE;
}

LRESULT CSudokuScore::OnMultiSelectButtonClicked(WPARAM, LPARAM)
{
    m_bAllowMultiSelect = myMultiSelectCheckBox.GetCheck();

	CView* pView = (CView*)GetParent();
	CSudokuDoc* pDoc = (CSudokuDoc*)pView->GetDocument();
    for (int i=0; i<myBoardDim; i++)
    {
        myDisplayLines[i].bFocus = false;
        pDoc->SelectScan(myDisplayLines[i].ucCand, false, TRUE);
    }

    Invalidate();
    UpdateWindow();

    return TRUE;
}

void CSudokuScore::UndoSort(DisplayLine_t* pDisplayLines, int len)
{
	for(int i=0; i < len; i++)
	{
		for(int j=1; j < len-i; j++)
        {
            if(pDisplayLines[j].ucCand < pDisplayLines[j-1].ucCand)
            {
                swap (pDisplayLines[j], pDisplayLines[j-1]);
            }
        }
	}
}

void CSudokuScore::SetProperties(bool bDisplayAlpha)
{
	myDisplayAlpha = bDisplayAlpha;
    Invalidate();
    UpdateWindow();
}

void CSudokuScore::SetColors(COLORREF backColor, COLORREF fillColor)
{
	m_backColor = backColor;
	m_fillColor = fillColor;
	if (m_pBrush) delete m_pBrush;
	m_pBrush = new CBrush();
	m_pBrush->CreateSolidBrush(m_fillColor);
}

void CSudokuScore::GetColors(COLORREF& backColor, COLORREF& fillColor)
{
	backColor = m_backColor;
	fillColor = m_fillColor;
}
