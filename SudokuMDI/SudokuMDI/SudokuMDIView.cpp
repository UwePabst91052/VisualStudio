// SudokuMDIView.cpp : Implementierung der Klasse CSudokuMDIView
//

#include "stdafx.h"
#include "SudokuMDI.h"

#include "SudokuSquareDoc.h"
#include "SudokuDuoDoc.h"
#include "SudokuSamuraiDoc.h"
#include "SudokuMDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuMDIView

IMPLEMENT_DYNCREATE(CSudokuMDIView, CView)

BEGIN_MESSAGE_MAP(CSudokuMDIView, CView)
	// Standarddruckbefehle
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_BEARBEITEN_EIGENSCHAFTEN, &CSudokuMDIView::OnBearbeitenEigenschaften)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_ANSICHT_REFRESH, &CSudokuMDIView::OnAnsichtRefresh)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_NEW_NUMBER, OnNewNumber)
	ON_MESSAGE(WM_MOVE_CURSOR, OnMoveCursor)
    ON_MESSAGE(WM_UPDATE_SCORE, OnUpdateScore)
END_MESSAGE_MAP()

// CSudokuMDIView-Erstellung/Zerstörung

CSudokuMDIView::CSudokuMDIView()
: myMousePos(0)
, myBoardDim(9)
, m_backColor(RGB(255,220,157))
, m_fillColor(RGB(0,200,200))
{
}

CSudokuMDIView::~CSudokuMDIView()
{
}

BOOL CSudokuMDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

// CSudokuMDIView-Zeichnung

void CSudokuMDIView::OnDraw(CDC* pDC)
{
	CSudokuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rcClient;
	GetClientRect(&rcClient);
    pDC->FillSolidRect(&rcClient, m_backColor);

    myBoardDim = pDoc->GetCurBoardDim();

	pDC->SetWindowExt(rcClient.Width(), rcClient.Height());

	/*------------------------*/
	/* Draw the Sudoko grid   */
	/*------------------------*/
	pDoc->DrawSudokuBoard(pDC);
}


// CSudokuMDIView drucken

void CSudokuMDIView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CSudokuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->SetMapMode(MM_HIMETRIC);

	/*------------------------*/
	/* Print the Sudoko grid   */
	/*------------------------*/
	pDoc->PrintSudokuBoard(pDC);

	//CView::OnPrint(pDC, pInfo);
}

BOOL CSudokuMDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CSudokuMDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CSudokuMDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: Bereinigung nach dem Drucken einfügen
}


// CSudokuMDIView-Diagnose

#ifdef _DEBUG
void CSudokuMDIView::AssertValid() const
{
	CView::AssertValid();
}

void CSudokuMDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSudokuDoc* CSudokuMDIView::GetDocument() const // Nicht-Debugversion ist inline
{
	ASSERT(m_pDocument->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CSudokuDoc)));
	return (CSudokuDoc*)m_pDocument;
}
#endif //_DEBUG


// CSudokuMDIView-Meldungshandler

void CSudokuMDIView::OnBearbeitenEigenschaften()
{
    CPropertySheet  propSheet(_T("Sudoku Properties"), this);
    SudokuProps_t   BoardProperties;
    CSudokuProperties* pPropPage;
    CSudokuDoc* pDoc = GetDocument();
	CSudokuProperties* pBoard8x8 = NULL;
	CSudokuProperties* pBoard9x9 = NULL;
	CSudokuProperties* pBoard12x12 = NULL;
	CSudokuProperties* pBoard16x16 = NULL;
	CSudokuProperties* pBoardDuo = NULL;
	CSudokuProperties* pBoardSamurai = NULL;

	if (pDoc->IsKindOf(RUNTIME_CLASS(CSudokuSquareDoc)))
	{
		// preset properties from boards and add pages to sheet
		pDoc->GetBoardProperties(BoardProperties);
		switch (pDoc->GetBoardType())
		{
		case btSquare8x8:
			{
				pBoard8x8 = new CSudokuProperties(IDS_TAB_BOARD_8X8);
				pBoard8x8->SetBoardProperties(BoardProperties);
				propSheet.AddPage(pBoard8x8);
			}
			break;
		case btSquare9x9:
			{
				pBoard9x9 = new CSudokuProperties(IDS_TAB_BOARD_9X9);
				pBoard9x9->SetBoardProperties(BoardProperties);
				propSheet.AddPage(pBoard9x9);
			}
			break;
		case btSquare12x12:
			{
				pBoard12x12 = new CSudokuProperties(IDS_TAB_BOARD_12X12);
				pBoard12x12->SetBoardProperties(BoardProperties);
				propSheet.AddPage(pBoard12x12);
			}
			break;
		case btSquare16x16:
			{
				pBoard16x16 = new CSudokuProperties(IDS_TAB_BOARD_16X16);
				pBoard16x16->SetBoardProperties(BoardProperties);
				propSheet.AddPage(pBoard16x16);
			}
			break;
		}
	}
	else if (pDoc->IsKindOf(RUNTIME_CLASS(CSudokuDuoDoc)))
	{
		// create property pages
		pBoardDuo = new CSudokuProperties(IDS_TAB_BOARD_DUO);

		// preset properties from boards and add pages to sheet
		pDoc->GetBoardProperties(BoardProperties);
		pBoardDuo->SetBoardProperties(BoardProperties);
		propSheet.AddPage(pBoardDuo);
	}
	else
	{
		pBoardSamurai = new CSudokuProperties(IDS_TAB_BOARD_SAMURAI);

		pDoc->GetBoardProperties(BoardProperties);
		pBoardSamurai->SetBoardProperties(BoardProperties);
		propSheet.AddPage(pBoardSamurai);
	}

    if (IDOK == propSheet.DoModal())
    {
        pPropPage = (CSudokuProperties*)propSheet.GetActivePage();
        pPropPage->GetBoardProperties(BoardProperties);
        pDoc->SetBoardProperties(BoardProperties);
		mySudokuScore.SetProperties(BoardProperties.myDisplayAlpha ? true : false);
		mySudokuScore.SetColors(BoardProperties.myBackColor, BoardProperties.myFillColor);
		myCandDisplay.SetBackgroundColor(BoardProperties.myBackColor);
		m_backColor = BoardProperties.myBackColor;
		m_fillColor = BoardProperties.myFillColor;
		AfxGetApp()->WriteProfileInt(L"Settings", L"ViewBackgroundColor", (int)m_backColor);
		AfxGetApp()->WriteProfileInt(L"Settings", L"ScoreBackgroundColor", (int)m_fillColor);
		ResizeFrameWindow();

        Invalidate();
        UpdateWindow();
    }

	if (pBoard9x9)     delete pBoard9x9;
	if (pBoard12x12)   delete pBoard12x12;
	if (pBoard16x16)   delete pBoard16x16;
	if (pBoardDuo)     delete pBoardDuo;
	if (pBoardSamurai) delete pBoardSamurai;
}

void CSudokuMDIView::OnMouseMove(UINT nFlags, CPoint point)
{
	CSudokuDoc* pDoc = GetDocument();
	pDoc->EvalMousePosition(point);

	CView::OnMouseMove(nFlags, point);
}

int CSudokuMDIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CSudokuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	pDoc->InitSudokoField(this);
    mySudokuScore.Create(WS_CHILD|WS_VISIBLE|LVS_LIST, CRect(0, 100, 90, 630), this, IDC_SCORE);

	ResizeFrameWindow();

	myCandDisplay.Create(_T("Kandidaten"), WS_CHILD|WS_VISIBLE, CRect(50, 10, 550, 30), this, IDC_STATIC_CAND_DISPLAY);
	myCandDisplay.SetBackgroundColor(m_backColor);
	myCandDisplay.Invalidate();
	myCandDisplay.UpdateWindow();

	return 0;
}

void CSudokuMDIView::OnAnsichtRefresh()
{
	CSudokuDoc* pDoc = GetDocument();
	pDoc->RefreshCandidates();
	Invalidate();
	UpdateWindow();
}

void CSudokuMDIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSudokuDoc* pDoc = GetDocument();
	pDoc->ToggleCandidate(point);

	CView::OnLButtonDown(nFlags, point);
}

LRESULT CSudokuMDIView::OnNewNumber(WPARAM wParam, LPARAM lParam)
{
	CPoint ptId(lParam);
	CSudokuDoc* pDoc = GetDocument();
	pDoc->SetSudokoFieldNumber(ptId, (int)wParam);
	Invalidate();
	UpdateWindow();
	return TRUE;
}
LRESULT CSudokuMDIView::OnMoveCursor(WPARAM wParam, LPARAM lParam)
{
	CPoint ptId(lParam);
	CSudokuDoc* pDoc = GetDocument();
	pDoc->MoveCursor(ptId, (int)wParam);

	return TRUE;
}
LRESULT CSudokuMDIView::OnUpdateScore(WPARAM wParam, LPARAM)
{
    mySudokuScore.UpdateScore((int*)wParam);

    return TRUE;
}

void CSudokuMDIView::ResizeFrameWindow(void)
{
	CSudokuDoc* pDoc = GetDocument();
	CRect rcBoard;

	pDoc->GetBoardDimensions(rcBoard);
	CWnd* pWnd = GetParent();
	pWnd->MoveWindow(10, 10, rcBoard.Width(), rcBoard.Height());

	int nBoardDim = pDoc->GetCurBoardDim();
	switch (pDoc->GetBoardType())
	{
	case btSquare8x8:
	case btSquare9x9:
	case btSquare12x12:
	case btSquare16x16:
		mySudokuScore.ResizeScoreWindow(nBoardDim, nBoardDim); 
		break;
	case btDuo:
		mySudokuScore.ResizeScoreWindow(17, 9); 
		break;
	case btSamurai:
		mySudokuScore.ResizeScoreWindow(41, 9); 
		break;
	}
}

void CSudokuMDIView::UpdateCandDisplay(int nSingleInFields, int nSingleOnBoard)
{
	myCandDisplay.SetSingleCounters(nSingleInFields, nSingleOnBoard);
}

void CSudokuMDIView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CSudokuDoc* pDoc = (CSudokuDoc*)GetDocument();
	SudokuProps_t props;
	props.myBackColor = m_backColor;
	props.myFillColor = m_fillColor;
	pDoc->GetBoardProperties(props);
	m_backColor = props.myBackColor;
	m_fillColor = props.myFillColor;
	pDoc->SetBoardProperties(props);
	mySudokuScore.SetColors(m_backColor, m_fillColor);
	myCandDisplay.SetBackgroundColor(m_backColor);
}
