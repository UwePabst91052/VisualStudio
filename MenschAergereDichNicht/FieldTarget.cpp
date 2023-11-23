#include "StdAfx.h"
#include "MenschAergereDichNicht.h"
#include "FieldTarget.h"

TCHAR* CFieldTarget::myCharacters = _T("abcd");

CFieldTarget::CFieldTarget()
{
}

CFieldTarget::CFieldTarget(Color_t color, CPoint ptNext, int pos)
:
CField(color, ptNext, CPoint(0,0), weiss)
,myPosition(pos)
{
}

CFieldTarget::~CFieldTarget(void)
{
}

IMPLEMENT_SERIAL(CFieldTarget, CField, 1)

void CFieldTarget::DrawField(CDC *pDC, CRect rcField)
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
	CFont ftNumbers, *pFont;

	oldPen = pDC->SelectObject(&borderPen);
	oldBrush = pDC->SelectObject(&innerBrush);

	// draw circle
	pDC->Ellipse(myInnerRect);

	// draw character
	pFont = pDC->GetCurrentFont();
	LOGFONT lfNumbers;
	if (pFont) pFont->GetLogFont(&lfNumbers);
	lfNumbers.lfHeight = myInnerRect.Width() - 10;
	lfNumbers.lfWidth  = lfNumbers.lfHeight / 3;
	lfNumbers.lfWeight = 400;
	_tcscpy_s((TCHAR*)lfNumbers.lfFaceName, 5, _T("Nina"));
	ftNumbers.CreateFontIndirect(&lfNumbers);
	pFont = pDC->SelectObject(&ftNumbers);
	CRect rcChar(myInnerRect);
	rcChar.DeflateRect(10,10);
	if (CLR_SCHWARZ == myInnerColor)
	{
		pDC->SetTextColor(CLR_WEISS);
	}
	pDC->SetBkColor(myInnerColor);
	pDC->DrawText((TCHAR*)&myCharacters[myPosition], 1, &rcChar, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

	if (CLR_SCHWARZ == myInnerColor)
	{
		pDC->SetTextColor(CLR_SCHWARZ);
	}
	pDC->SelectObject(pFont);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CFieldTarget::Serialize(CArchive& ar)
{
	CField::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar << myPosition;
	}
	else
	{	// loading code
		ar >> myPosition;
	}
}
