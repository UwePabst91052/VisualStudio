#include "StdAfx.h"
#include "MenschAergereDichNicht.h"
#include "FieldMove.h"

CFieldMove::CFieldMove()
{
}

CFieldMove::CFieldMove(CPoint ptNext, CPoint ptAlt, Color_t clrAlt, bool up, bool left, bool down, bool right)
:
CField(weiss, ptNext, ptAlt, clrAlt)
{
	myConnectors.connUp = up;
	myConnectors.connLeft = left;
	myConnectors.connDown = down;
	myConnectors.connRight = right;
}

CFieldMove::~CFieldMove(void)
{
}

IMPLEMENT_SERIAL(CFieldMove, CField, 1)

void CFieldMove::DrawField(CDC *pDC, CRect rcField)
{
	myFieldRect = rcField;
	myInnerRect = rcField;
	myConnectorLength = myFieldRect.Width() / 10;
	myInnerRect.DeflateRect(myConnectorLength, myConnectorLength);
	myBorderWidth = rcField.Width() / 20;

	CPen borderPen(PS_SOLID, myBorderWidth, CLR_SCHWARZ),
		 *oldPen;
	CBrush innerBrush(myInnerColor), 
		   *oldBrush;
	oldPen = pDC->SelectObject(&borderPen);
	oldBrush = pDC->SelectObject(&innerBrush);

	pDC->Ellipse(myInnerRect);

	// draw connectors
	CPoint ptFrom, ptTo;
	if (myConnectors.connUp)
	{
		ptFrom.x = myInnerRect.left + myInnerRect.Width() / 2;
		ptFrom.y = myInnerRect.top;
		ptTo.x = ptFrom.x;
		ptTo.y = ptFrom.y - myConnectorLength;
		pDC->MoveTo(ptFrom);
		pDC->LineTo(ptTo);
	}
	if (myConnectors.connRight)
	{
		ptFrom.x = myInnerRect.right;
		ptFrom.y = myInnerRect.top + myInnerRect.Width() / 2;
		ptTo.x = ptFrom.x + myConnectorLength;
		ptTo.y = ptFrom.y;
		pDC->MoveTo(ptFrom);
		pDC->LineTo(ptTo);
	}
	if (myConnectors.connDown)
	{
		ptFrom.x = myInnerRect.left + myInnerRect.Width() / 2;
		ptFrom.y = myInnerRect.bottom;
		ptTo.x = ptFrom.x;
		ptTo.y = ptFrom.y + myConnectorLength;
		pDC->MoveTo(ptFrom);
		pDC->LineTo(ptTo);
	}
	if (myConnectors.connLeft)
	{
		ptFrom.x = myInnerRect.left;
		ptFrom.y = myInnerRect.top + myInnerRect.Width() / 2;
		ptTo.x = ptFrom.x - myConnectorLength;
		ptTo.y = ptFrom.y;
		pDC->MoveTo(ptFrom);
		pDC->LineTo(ptTo);
	}

	// draw stone
	int xposStone, yposStone;
	CBitmap *pOldBitmap = NULL;
	if (NULL != myBmpStone)
	{
		CDC dcStone;
		dcStone.CreateCompatibleDC(pDC);
		BITMAP bmpInfo;
		myBmpStone->GetBitmap(&bmpInfo);
		pOldBitmap = dcStone.SelectObject(myBmpStone);
		COLORREF clrTransparent = dcStone.GetPixel(0, 0);
		xposStone = myInnerRect.left + ((myInnerRect.Width() / 2) - (bmpInfo.bmWidth / 2));
		yposStone = myInnerRect.top  + ((myInnerRect.Height() / 2) - (bmpInfo.bmHeight / 2));
		pDC->TransparentBlt(xposStone, yposStone, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcStone,
						    0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, clrTransparent);
		dcStone.SelectObject(pOldBitmap);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CFieldMove::Serialize(CArchive& ar)
{
	CField::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar << myConnectorLength;
	}
	else
	{	// loading code
		ar >> myConnectorLength;
	}
}
