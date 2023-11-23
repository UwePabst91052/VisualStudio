// MenschAergereDichNichtView.cpp : Implementierung der Klasse CMADNView
//

#include "stdafx.h"
#include "MenschAergereDichNicht.h"

#include "MenschAergereDichNichtDoc.h"
#include "MenschAergereDichNichtView.h"
#include "Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CMADNView::clientDimension = 630;

// CMADNView

IMPLEMENT_DYNCREATE(CMADNView, CView)

BEGIN_MESSAGE_MAP(CMADNView, CView)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SPRITE_STOPPED, OnSpriteStopped)
	ON_MESSAGE(WM_NEXT_PLAYER, OnNextPlayer)
    ON_MESSAGE(WM_FINISH_GAME, OnFinishGame)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MODE_AUTO, &CMADNView::OnModeAutomatic)
	ON_UPDATE_COMMAND_UI(ID_MODE_AUTO, OnUpdateModeAuto)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMADNView, CView)
	ON_EVENT(CMADNView, 102, 1, OnHabeGewuerfelt, VTS_I4)
END_EVENTSINK_MAP()

// CMADNView-Erstellung/Zerstörung

CMADNView::CMADNView()
: m_SpritePosX(10)
, m_SpritePosY(10)
, m_SpritePosToX(10)
, m_SpritePosToY(10)
, m_bMoveStarted(false)
, m_bAutoPlay(FALSE)
, m_bInitialized(false)
{	
}

CMADNView::~CMADNView()
{
}

BOOL CMADNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

// CMADNView-Zeichnung

void CMADNView::OnDraw(CDC* pDC)
{
    TRACE_FUNCTION;
	CMADNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// fill background color
	CRect rcClient;
	this->GetClientRect(&rcClient);

	CDC dcBackground;
	dcBackground.CreateCompatibleDC(pDC);

	// paint colored rectangles
	CBitmap *pOldBitmap, *pBmpBackground;
	pBmpBackground = m_Sprite.GetBackgroundBitmap();
    if (!m_bInitialized)
    {
	    BOOL ret = pBmpBackground->CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
        if (!ret)
        {
            DWORD error = GetLastError();
            TRACE1("CreateCompatibleBitmap failed with 0x%x\n", error);
            return;
        }
        m_bInitialized = true;
    }
	pOldBitmap = dcBackground.SelectObject(pBmpBackground);


	pDC->FillSolidRect(rcClient, CLR_VIEW_BACKGROUND);
	// calculate field dimension
	int fieldWidth = clientDimension / NO_OF_FIELD_DIM;

	// draw title parts
	CPoint ptTitle;
	CSize  szTitle;
	CRect  rcTitle;
	CFont  ftTitle, *pFont; 
	LOGFONT lfTitle;
	ptTitle.x = clientDimension / 16;
	ptTitle.y = (clientDimension * 7) / 30;
	szTitle.cx = 400;
	szTitle.cy = clientDimension / 18;
	rcTitle = CRect(ptTitle, szTitle);
	pFont = pDC->GetCurrentFont();
	pFont->GetLogFont(&lfTitle);
	lfTitle.lfHeight = szTitle.cy;
	lfTitle.lfWidth = szTitle.cy / 2;
	lfTitle.lfWeight = 600;
	_tcscpy_s(lfTitle.lfFaceName, 6, _T("Arial"));
	ftTitle.CreateFontIndirect(&lfTitle);
	pFont = pDC->SelectObject(&ftTitle);
	pDC->DrawText(_T("Mensch"), -1, rcTitle, DT_SINGLELINE);
	ptTitle.x = (clientDimension * 14) / 20;
	rcTitle = CRect(ptTitle, szTitle);
	pDC->DrawText(_T("ärgere"), -1, rcTitle, DT_SINGLELINE);
	ptTitle.x = (clientDimension * 2) / 20;
	ptTitle.y = (clientDimension * 21) / 30;
	rcTitle = CRect(ptTitle, szTitle);
	pDC->DrawText(_T("Dich"), -1, rcTitle, DT_SINGLELINE);
	ptTitle.x = (clientDimension * 22) / 30;
	rcTitle = CRect(ptTitle, szTitle);
	pDC->DrawText(_T("nicht"), -1, rcTitle, DT_SINGLELINE);

	// draw arrows
	CPoint ptArrow;
	CSize  szArrow;
	CRect  rcArrow;
	ptArrow.x = fieldWidth / 3;
	ptArrow.y = (4 * fieldWidth) - ((fieldWidth * 13) / 60);
	szArrow.cx = fieldWidth;
	szArrow.cy = (fieldWidth * 13) / 60;
	rcArrow = CRect(ptArrow, szArrow);
	DrawArrow(pDC, rcArrow, right, IDB_BITMAP1);

	ptArrow.x = (7 * fieldWidth);
	ptArrow.y = fieldWidth / 3;
	szArrow.cx = (fieldWidth * 13) / 60;
	szArrow.cy = fieldWidth;
	rcArrow = CRect(ptArrow, szArrow);
	DrawArrow(pDC, rcArrow, down, IDB_BITMAP1);

	ptArrow.x = clientDimension - fieldWidth - (fieldWidth / 3);
	ptArrow.y = 7 * fieldWidth;
	szArrow.cx = fieldWidth;
	szArrow.cy = (fieldWidth * 13) / 60;
	rcArrow = CRect(ptArrow, szArrow);
	DrawArrow(pDC, rcArrow, left, IDB_BITMAP1);

	ptArrow.x = (4 * fieldWidth) - ((fieldWidth * 13) / 60);
	ptArrow.y = clientDimension - fieldWidth - (fieldWidth / 3);
	szArrow.cx = (fieldWidth * 13) / 60;
	szArrow.cy = fieldWidth;
	rcArrow = CRect(ptArrow, szArrow);
	DrawArrow(pDC, rcArrow, up, IDB_BITMAP1);

	// draw fields
	int xPos, yPos;
	CRect rcField;
	CSize szDim(fieldWidth, fieldWidth);
	CFieldMap* pFieldMap = GetDocument()->GetFieldMap();
	CFieldMap::CPair* curVal = pFieldMap->PGetFirstAssoc();
	while (curVal != NULL)
	{
		CField* pField = curVal->value;
		CPoint  ptPos;
		WORD_TO_POINT(curVal->key, ptPos);
		xPos = ptPos.x * fieldWidth;
		yPos = ptPos.y * fieldWidth;
		rcField = CRect(CPoint(xPos, yPos), szDim);
		pField->DrawField(pDC, rcField);
		curVal = pFieldMap->PGetNextAssoc(curVal);
	}

	dcBackground.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDC, 0, 0, SRCCOPY);

	// update player color
	m_Würfel.SetProperty(DISPID_FORECOLOR, VT_I4, GetDocument()->GetPlayerColor());

	dcBackground.SelectObject(pOldBitmap);
}


// CMADNView-Diagnose

#ifdef _DEBUG
void CMADNView::AssertValid() const
{
	CView::AssertValid();
}

void CMADNView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMADNDoc* CMADNView::GetDocument() const // Nicht-Debugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMADNDoc)));
	return (CMADNDoc*)m_pDocument;
}
#endif //_DEBUG


// CMADNView-Meldungshandler


int CMADNView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Sprite.CreateSprite((CView*)this);

	int fieldWidth = clientDimension / NO_OF_FIELD_DIM;
	m_Würfel.Create(_T("WUERFEL_CLASS"), _T("Würfel"), WS_VISIBLE | WS_CHILD, 
		            CRect(CPoint(5 * fieldWidth, 5 * fieldWidth), CSize(fieldWidth, fieldWidth)), this, 102);
	m_Würfel.SetProperty(DISPID_FORECOLOR, VT_I4, GetDocument()->GetPlayerColor());
	m_Würfel.SetProperty(DISPID_BACKCOLOR, VT_I4, CLR_VIEW_BACKGROUND);

	return 0;
}

LRESULT CMADNView::OnSpriteStopped(WPARAM, LPARAM)
{
    TRACE_FUNCTION; 
	if (GetDocument()->MoveControler())
	{
		Invalidate();
		UpdateWindow();
	}

	return TRUE;
}

void CMADNView::OnHabeGewuerfelt(int number)
{
    TRACE_FUNCTION;
    DBG_TRACE1("CChildView::OnHabeGewuerfelt: Gewuerfelte Zahl = %d\n", number);
	if (GetDocument()->InvestigateMove(number))
	{
		SetPlayerColor(GetDocument()->GetPlayerColor());
	}
	else
	{
		PostMessage(WM_NEXT_PLAYER);
	}
}

void CMADNView::DrawArrow(CDC* pDC, CRect rcArrow, ArrowDir_t arrowDir, UINT nID)
{
	CBitmap bmp;

	if (bmp.LoadBitmap(nID))
	{
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

		// fill background
		CBrush brush(CLR_VIEW_BACKGROUND), *pOldBrush;
		pOldBrush = dcMemory.SelectObject(&brush);
		dcMemory.FloodFill(5, 5, CLR_SCHWARZ);

		// create rotate bitmap in DC
		int copyWidth, copyHeight;
		int hori, vert;
		CBitmap *pOldRotate, bmpRotate;
		CDC dcRotate;
		dcRotate.CreateCompatibleDC(pDC);
		switch (arrowDir)
		{
		case up:
		case down:
			copyWidth  = bmpInfo.bmHeight;
			copyHeight = bmpInfo.bmWidth;
			break;
		case right:
		case left:
		default:
			copyWidth  = bmpInfo.bmWidth;
			copyHeight = bmpInfo.bmHeight;
			break;
		}
		bmpRotate.CreateCompatibleBitmap(pDC, copyWidth, copyHeight);
		pOldRotate = dcRotate.SelectObject(&bmpRotate);

		// rotate bitmap
		COLORREF clrPixel;
		for (vert = 0; vert < bmpInfo.bmHeight; vert++)
		{
			for (hori = 0; hori < bmpInfo.bmWidth; hori++)
			{
				clrPixel = dcMemory.GetPixel(hori, vert);
				switch (arrowDir)
				{
				case up:
					dcRotate.SetPixel(vert, copyHeight-1-hori, clrPixel);
					break;
				case down:
					dcRotate.SetPixel(copyWidth-1-vert, hori, clrPixel);
					break;
				case left:
					dcRotate.SetPixel(copyWidth-1-hori, vert, clrPixel); 
					break;
				case right:
					dcRotate.SetPixel(hori, vert, clrPixel);
					break;
				}
			}
		}
		// draw arrow
		pDC->StretchBlt(rcArrow.left, rcArrow.top, rcArrow.Width(), rcArrow.Height(), &dcRotate,
			0, 0, copyWidth, copyHeight, SRCCOPY);

		dcMemory.SelectObject(pOldBitmap);
		dcRotate.SelectObject(pOldRotate);
		pDC->SelectObject(pOldBrush);
	}
}


void CMADNView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int fieldWidth = clientDimension / NO_OF_FIELD_DIM;
	CRect rcWuerfel(CPoint(5 * fieldWidth, 5 * fieldWidth), CSize(fieldWidth, fieldWidth));
	if (rcWuerfel.PtInRect(point))
	{
		m_Würfel.WuerfleZahl();
		return;
	}

	if (m_bMoveStarted) return;

    TRACE_FUNCTION;
	CFieldMap* pFieldMap = GetDocument()->GetFieldMap();
	POSITION pos = pFieldMap->GetStartPosition();
	WORD     key;
	while (pos != NULL)
	{
		CField* pField = NULL;
		pFieldMap->GetNextAssoc(pos, key, pField);
		if (pField)
		{
			if (pField->isClickedIn(point))
			{
				CPoint ptPos;
				WORD_TO_POINT(key, ptPos);
				DBG_TRACE2("Start move at %d, %d\n", HIBYTE(key), LOBYTE(key)/*ptPos.x, ptPos.y*/);
				m_moveSimu.fieldFrom = pField;
				m_moveSimu.ptFieldFrom = ptPos;
				m_bMoveStarted = true;
			}
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CMADNView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bMoveStarted) return;

    TRACE_FUNCTION;
	CMADNDoc* pDoc = GetDocument();
	CFieldMap* pFieldMap = GetDocument()->GetFieldMap();
	POSITION pos = pFieldMap->GetStartPosition();
	WORD     key;
	while (pos != NULL)
	{
		CField* pField = NULL;
		pFieldMap->GetNextAssoc(pos, key, pField);
		if (pField)
		{
			if (pField->isClickedIn(point))
			{
				CPoint ptPos;
				WORD_TO_POINT(key, ptPos);
				DBG_TRACE2("Move to %d, %d\n", HIBYTE(key), LOBYTE(key)/*ptPos.x, ptPos.y*/);
				m_moveSimu.fieldTo = pField;
				m_moveSimu.ptFieldTo = ptPos;
				m_moveSimu.moveState = active;
				pDoc->SetPlayerMove(m_moveSimu);
				pDoc->MoveControler();
				m_bMoveStarted = false;
			}
		}
	}

	CView::OnRButtonDown(nFlags, point);
}

void CMADNView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    CDC* pDC = GetDC();
	Move_t* pMove = (Move_t*)lHint;
	CRect rcSprite;
    TRACE_FUNCTION;

    DBG_TRACE1("    pMove = %d", (int)pMove);

	if (NULL == pMove) return;

	if (pMove->fieldFrom->GetSpriteRect(rcSprite))
	{
        DBG_TRACE0("Move Sprite");
		m_pBmpSpriteMove = pMove->fieldFrom->RemoveStone(pDC, m_Sprite.GetBackgroundBitmap());
		m_Sprite.SetStartPosition(rcSprite, m_pBmpSpriteMove);
		pMove->fieldTo->GetSpriteRect(rcSprite);
		m_Sprite.StartMove(pDC, rcSprite);
		pMove->fieldTo->SetStone(m_pBmpSpriteMove, pMove->fieldTo->GetStoneColor());
	}
    ReleaseDC(pDC);
}

void CMADNView::SetPlayerColor(COLORREF clrPlayer)
{
	TRACE_FUNCTION;
	m_Würfel.SetProperty(DISPID_FORECOLOR, VT_I4, clrPlayer);
}

void CMADNView::OnModeAutomatic()
{
	m_bAutoPlay = m_bAutoPlay ? FALSE : TRUE;
}

void CMADNView::OnUpdateModeAuto(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAutoPlay);
}

LRESULT CMADNView::OnNextPlayer(WPARAM, LPARAM)
{
	TRACE_FUNCTION;
	if (m_bAutoPlay) m_Würfel.WuerfleZahl();
	return TRUE;
}

LRESULT CMADNView::OnFinishGame(WPARAM, LPARAM)
{
	TRACE_FUNCTION;
    m_bAutoPlay = false;
    return TRUE;
}