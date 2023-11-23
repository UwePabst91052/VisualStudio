// Field.cpp : implementation file
//

#include "stdafx.h"
#include "MenschAergereDichNicht.h"
#include "Field.h"


// CField

const UINT CField::bitmapIDs[]  = { IDB_PLAYER_YELLOW, IDB_PLAYER_GREEN, IDB_PLAYER_RED, IDB_PLAYER_BLACK};

IMPLEMENT_DYNAMIC(CField, CObject)

CField::CField()
: myBmpStone(NULL)
{
}

CField::CField(Color_t color, CPoint ptNext, CPoint ptAlt, Color_t clrAlt)
:
 myFieldRect(0,0,0,0)
,myInnerRect(0,0,0,0)
,myBorderWidth(0)
,myBorderColor(CLR_SCHWARZ)
,myBmpStone(NULL)
,myStoneColor(undef)
,m_ptNextMain(ptNext)
,m_ptNextAlt(ptAlt)
,m_clrAlt(clrAlt)
{
	// choose color
	switch (color)
	{
	case rot:
		myInnerColor = CLR_ROT;
		break;
	case gelb:
		myInnerColor = CLR_GELB;
		break;
	case gruen:
		myInnerColor = CLR_GRUEN;
		break;
	case schwarz:
		myInnerColor = CLR_SCHWARZ;
		break;
	default:
		myInnerColor = CLR_WEISS;
	}
}

CField::~CField()
{
}

// CField member functions

void CField::SetStone(CBitmap* pBmpStone, Color_t clrStone)
{
    TRACE_FUNCTION; 
	myBmpStone = pBmpStone;
	myStoneColor = clrStone;
}

CBitmap* CField::RemoveStone(CDC* pDC, CBitmap* pBmpBackground)
{
	CBitmap* bmpRet = myBmpStone;
	Color_t  color  = myStoneColor;
    TRACE_FUNCTION; 

	if (pDC && pBmpBackground)
	{
		CDC dcBackground;
		dcBackground.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap = dcBackground.SelectObject(pBmpBackground);

		myBmpStone = NULL;
		myStoneColor = undef;

		DrawField(&dcBackground, myFieldRect);

		myBmpStone = bmpRet;
		myStoneColor = color;

		dcBackground.SelectObject(pOldBitmap);
	}
	else
	{
		myBmpStone = NULL;
		myStoneColor = undef;
	}

	return bmpRet;
}

void CField::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar  << (int)m_clrAlt
			<< m_ptNextAlt.x
			<< m_ptNextAlt.y
			<< m_ptNextMain.x
			<< m_ptNextMain.y
			<< myBorderColor
			<< myBorderWidth
			<< myConnectors.connDown
			<< myConnectors.connLeft
			<< myConnectors.connRight
			<< myConnectors.connUp
			<< myFieldRect.top
			<< myFieldRect.left
			<< myFieldRect.bottom
			<< myFieldRect.right
			<< myInnerColor
			<< myInnerRect.top
			<< myInnerRect.left
			<< myInnerRect.bottom
			<< myInnerRect.right
			<< (int)myStoneColor;
	}
	else
	{	// loading code
		ar  >> (int&)m_clrAlt
			>> m_ptNextAlt.x
			>> m_ptNextAlt.y
			>> m_ptNextMain.x
			>> m_ptNextMain.y
			>> myBorderColor
			>> myBorderWidth
			>> myConnectors.connDown
			>> myConnectors.connLeft
			>> myConnectors.connRight
			>> myConnectors.connUp
			>> myFieldRect.top
			>> myFieldRect.left
			>> myFieldRect.bottom
			>> myFieldRect.right
			>> myInnerColor
			>> myInnerRect.top
			>> myInnerRect.left
			>> myInnerRect.bottom
			>> myInnerRect.right
			>> (int&)myStoneColor;
	}
}
