#include "StdAfx.h"
#include "MenschAergereDichNicht.h"
#include "FieldBase.h"

CFieldBase::CFieldBase()
{}

CFieldBase::CFieldBase(Color_t color, CPoint ptNext)
:
CField(color, ptNext, CPoint(0,0), weiss)
{

}

CFieldBase::~CFieldBase(void)
{
}

IMPLEMENT_SERIAL(CFieldBase, CField, 1)

void CFieldBase::DrawField(CDC *pDC, CRect rcField)
{
	myFieldRect = rcField;
	myInnerRect = rcField;
	int deflateLength = myFieldRect.Width() / 5;
	myInnerRect.DeflateRect(deflateLength, deflateLength);
	myBorderWidth = rcField.Width() / 20;

	CPen borderPen(PS_SOLID, myBorderWidth, CLR_SCHWARZ),
		 *oldPen;
	CBrush innerBrush(myInnerColor), 
		   *oldBrush;

	oldPen = pDC->SelectObject(&borderPen);
	oldBrush = pDC->SelectObject(&innerBrush);

	pDC->Ellipse(myInnerRect);

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

void CFieldBase::Serialize(CArchive& ar)
{
	CField::Serialize(ar);
}
