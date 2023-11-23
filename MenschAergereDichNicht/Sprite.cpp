// Sprite.cpp : implementation file
//

#include "stdafx.h"
#include "MenschAergereDichNicht.h"
#include "Sprite.h"

// CSprite

CSprite::CSprite()
: m_hTimer(NULL)
, m_pView()
, m_hThread(NULL)
, m_bExitThread(FALSE)
, m_bmpBackground(NULL)
{
	DWORD dwThreadId;
	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	m_hThread = CreateThread(NULL, 0, SpriteThread, this, 0, &dwThreadId);

	m_bmpBackground = new CBitmap();
}

CSprite::~CSprite()
{
	if (m_hTimer) 
	{
		CloseHandle(m_hTimer);
		m_hTimer = NULL;
	}
	if (m_hThread) 
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if (m_bmpBackground)
	{
		delete m_bmpBackground;
	}
}


// CSprite member functions

DWORD CSprite::SpriteThread(LPVOID context)
{
	CSprite* pSprite = (CSprite*) context;
	DWORD dwRetVal;

	while(!pSprite->m_bExitThread)
	{
		dwRetVal = ::WaitForSingleObject(pSprite->m_hTimer, INFINITE);
		if (WAIT_OBJECT_0 == dwRetVal)
		{
			if (!pSprite->MoveSprite())
			{
				pSprite->StopMove();
			}
		}
	}

	return 0;
}

bool CSprite::CreateSprite(CView* pView)
{
    TRACE_FUNCTION;
	m_pView = pView;

	return false;
}

void CSprite::DestroySprite(void)
{
    TRACE_FUNCTION;
	m_bExitThread = TRUE;
	WaitForSingleObject(m_hThread, 3000);
}

void CSprite::SetStartPosition(CRect rcFrom, CBitmap* bmpSprite)
{
    TRACE_FUNCTION;
	m_bmpSprite = bmpSprite;
	m_bmpSprite->GetBitmap(&m_bmpInfo);
	m_SpritePosX = rcFrom.left + ((rcFrom.Width() / 2) - (m_bmpInfo.bmWidth / 2));
	m_SpritePosY = rcFrom.top + ((rcFrom.Height() / 2) - (m_bmpInfo.bmHeight / 2));
	TRACE2("SetPosition: xPosTo = %d,  yPosTo = %d\n", m_SpritePosX, m_SpritePosY);
	m_SpriteWidth = m_bmpInfo.bmWidth/*rcFrom.Width()*/;
	m_SpriteHeight = m_bmpInfo.bmHeight/*rcFrom.Height()*/;
	m_rcSprite = CRect(CPoint(m_SpritePosX, m_SpritePosY), CSize(m_SpriteWidth, m_SpriteHeight));
	DrawSprite();
}

void CSprite::StartMove(CDC* pDC, CRect rcTo)
{
    TRACE_FUNCTION;
	m_SpritePosToX = rcTo.left  + ((rcTo.Width() / 2) - (m_bmpInfo.bmWidth / 2));
	m_SpritePosToY = rcTo.top + ((rcTo.Height() / 2) - (m_bmpInfo.bmHeight / 2));
	m_rcSprite = CRect(CPoint(m_SpritePosToX, m_SpritePosToY), CSize(m_SpriteWidth, m_SpriteHeight));
	TRACE2("StartMove: xPosTo = %d,  yPosTo = %d\n", m_SpritePosToX, m_SpritePosToY);
	// calculate move steps
	int deltaX = m_SpritePosToX - m_SpritePosX;
	int signX  = sign(deltaX);
	int deltaY = m_SpritePosToY - m_SpritePosY;
	int signY  = sign(deltaY);
	m_realPosX = m_SpritePosX;
	m_realPosY = m_SpritePosY;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);
	if (0 == deltaX)
	{
		m_deltaX = 0;
		m_deltaY = 2.0 * signY;
	}
	else if (0 == deltaY)
	{
		m_deltaY = 0;
		m_deltaX = 2.0 * signX;
	}
	else
	{
		if (deltaX > deltaY)
		{
			m_deltaX = 2.0 * signX;
			m_deltaY = (double)deltaY / ((double)deltaX / 2.0) * signY;
		}
		else if (deltaX < deltaY)
		{
			m_deltaX = (double)deltaX / ((double)deltaY / 2.0) * signX;
			m_deltaY = 2.0 * signY;
		}
		else
		{
			m_deltaX = 2.0 * signX;
			m_deltaY = 2.0 * signY;
		}
	}
	TRACE2("StartMove: deltaX = %.3f,  deltaY = %.3f\n", m_deltaX, m_deltaY);

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -100000;
	::SetWaitableTimer(m_hTimer, &liDueTime, 10, NULL, NULL, FALSE);
}

bool CSprite::MoveSprite(void)
{
	bool bMove = false;
	if ((m_SpritePosX > m_SpritePosToX+abs((int)m_deltaX)) ||
		(m_SpritePosX < m_SpritePosToX-abs((int)m_deltaX)))
	{
		m_realPosX += m_deltaX;
		m_SpritePosX = (int)m_realPosX;
		bMove = true;
	}
	if ((m_SpritePosY > m_SpritePosToY+abs((int)m_deltaY)) ||
		(m_SpritePosY < m_SpritePosToY-abs((int)m_deltaY)))
	{
		m_realPosY += m_deltaY;
		m_SpritePosY = (int)m_realPosY ;
		bMove = true;
	}
	if (bMove)
	{
		m_rcSprite.MoveToXY(m_SpritePosX, m_SpritePosY);
		DrawSprite();
	}
	return bMove;
}

void CSprite::StopMove(void)
{
    TRACE_FUNCTION;
	//DeleteSprite();

	CancelWaitableTimer(m_hTimer);
	m_pView->SendMessage(WM_SPRITE_STOPPED);
}

void CSprite::DeleteSprite(void)
{
    TRACE_FUNCTION;
	CDC* pDC = m_pView->GetDC();
	CDC dcBackground;
	dcBackground.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dcBackground.SelectObject(m_bmpBackground);

	CBitmap bmpCurrent, *pOldCurrent;
	bmpCurrent.CreateCompatibleBitmap(pDC, m_SpriteWidth, m_SpriteHeight);
	CDC dcCurrent;
	dcCurrent.CreateCompatibleDC(pDC);
	pOldCurrent = dcCurrent.SelectObject(&bmpCurrent);

	dcCurrent.BitBlt(0, 0, m_SpriteWidth, m_SpriteHeight, &dcBackground, m_SpritePosX, m_SpritePosY, SRCCOPY);
	pDC->BitBlt(m_SpritePosX, m_SpritePosY, m_SpriteWidth, m_SpriteHeight, &dcCurrent, 0, 0, SRCCOPY);

	dcBackground.SelectObject(pOldBitmap);
	dcCurrent.SelectObject(pOldCurrent);

    m_pView->ReleaseDC(pDC);
}

void CSprite::DrawSprite(void)
{
	CDC* pDC = m_pView->GetDC();

	// create dc with background bitmap as source
	CDC dcBackground;
	dcBackground.CreateCompatibleDC(pDC);
	CBitmap* pOldBackground = dcBackground.SelectObject(m_bmpBackground);

	// create dc for current view background bitmap as target
	BITMAP bmpInfo;
	m_bmpBackground->GetBitmap(&bmpInfo);
	CDC dcCurrent;
	dcCurrent.CreateCompatibleDC(pDC);
	CBitmap bmpCurrent, *pOldCurrent;
	bmpCurrent.CreateCompatibleBitmap(pDC, bmpInfo.bmWidth, bmpInfo.bmHeight);
	pOldCurrent = dcCurrent.SelectObject(&bmpCurrent);

	// copy current bitmap as background for sprite
	dcCurrent.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcBackground, 0, 0, SRCCOPY);

	// Create an in-memory DC compatible with the
	// display DC we're using to paint
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	// Select the bitmap into the in-memory DC
	CBitmap* pOldBitmap = dcMemory.SelectObject(m_bmpSprite);
	COLORREF clrTransparent = dcMemory.GetPixel(0,0);

	// Copy the bits from the in-memory DC into the on-
	// screen DC to actually do the painting. Use the centerpoint
	// we computed for the target offset.
	dcCurrent.TransparentBlt(m_SpritePosX, m_SpritePosY, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMemory,
	  0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, clrTransparent);

	pDC->BitBlt(m_SpritePosX, m_SpritePosY, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcCurrent, m_SpritePosX, m_SpritePosY, SRCCOPY);

	dcMemory.SelectObject(pOldBitmap);
	dcBackground.SelectObject(pOldBackground);
	dcCurrent.SelectObject(pOldCurrent);

    m_pView->ReleaseDC(pDC);
}
