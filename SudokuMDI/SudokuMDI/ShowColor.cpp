// ShowColor.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "ShowColor.h"


// CShowColor

IMPLEMENT_DYNAMIC(CShowColor, CStatic)

CShowColor::CShowColor()
{
	myColor = RGB(255,255,255);
}

CShowColor::~CShowColor()
{
}


BEGIN_MESSAGE_MAP(CShowColor, CStatic)
END_MESSAGE_MAP()



// CShowColor message handlers



void CShowColor::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	CRect rcClient;
	GetClientRect(rcClient);
	CDC* pDC = GetDC();
	pDC->FillSolidRect(rcClient, myColor);

}

void CShowColor::SetColor(COLORREF color)
{
	myColor = color;

	Invalidate();
	UpdateWindow();
}
