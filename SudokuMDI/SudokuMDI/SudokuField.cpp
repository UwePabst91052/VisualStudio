// SudokuField2.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuField.h"
#include ".\sudokufield.h"

#define _USE_MATH_DEFINES 
#include <math.h>
#include <cmath> 

const double pi      = M_PI;
const double arc45   = M_PI_4;
const double arc135  = 3 * M_PI_4;
const double arc225  = 5 * M_PI_4;
const double arc315  = 7 * M_PI_4;

// CSudokuField

IMPLEMENT_SERIAL(CSudokuField, CObject, VERSIONABLE_SCHEMA | 4)

CSudokuField::CSudokuField()
: 
 m_pParent(NULL)
,m_pFieldEdit(NULL)
,m_bEditMode(true)
,m_bShowCandidates(false)
,myDim(0)
,myDimDP(0)
,myCandLen(8)
,myCandLenDP(240)
,myFontSizeDP(500)
,myDisplayAlpha(TRUE)
,ptFieldPos(0,0)
,ptFieldPosDP(0,0)
,pCandidates(NULL)
{
}

CSudokuField::CSudokuField(CPoint ptPos, CPoint ptPosDP, 
						   int nBoardDim, 
						   int iDim, int iDimDP,
						   int iCandLen, int iCandLenDP, int iFontSizeDP)
: m_pParent(NULL)
, m_bEditMode(true)
, myBoardDim(nBoardDim)
, m_bShowCandidates(false)
, myDisplayAlpha(TRUE)
{
    FinalConstruction();
	myDim = iDim;
	myDimDP = iDimDP;
	myCandLen = iCandLen;
	myCandLenDP = iCandLenDP;
    myFontSizeDP = iFontSizeDP;
	ptFieldPos = ptPos;
	ptFieldPosDP = ptPosDP;
    m_pFieldEdit = new CSudokoEdit;
	InitCandidates();
}

void CSudokuField::UpdateLayout(CPoint ptPos, CPoint ptPosDP,
                                int iDim, int iDimDP, 
				                int iCandLen, int iCandLenDP, 
                                int iFontSizeDP, BOOL bDisplayAlpha,
								COLORREF backColor)
{
	myDim = iDim;
	myDimDP = iDimDP;
	myCandLen = iCandLen;
	myCandLenDP = iCandLenDP;
    myFontSizeDP = iFontSizeDP;
	myDisplayAlpha = bDisplayAlpha;
	ptFieldPos = ptPos;
	ptFieldPosDP = ptPosDP;
	InitCandidates();

	CRect rcEdit;
	// update and move field for number input
	rcEdit.left = ptFieldPos.x + myCandLen;
	rcEdit.right = ptFieldPos.x + myDim - myCandLen;
	rcEdit.top = ptFieldPos.y + myCandLen;
	rcEdit.bottom = ptFieldPos.y + myDim - myCandLen;
    m_pFieldEdit->MoveWindow(rcEdit);
	m_pFieldEdit->SetProperties(bDisplayAlpha, rcEdit, backColor);

	// update Print rectangle for number
	m_rcNumber.left = ptFieldPosDP.x + myCandLenDP;
	m_rcNumber.right = ptFieldPosDP.x + myDimDP - myCandLenDP;
	m_rcNumber.top = ptFieldPosDP.y - myCandLenDP;
	m_rcNumber.bottom = ptFieldPosDP.y - myDimDP + myCandLenDP;
}

CSudokuField::CSudokuField(CSudokuField& fromField)
: m_pParent(NULL)
, m_bEditMode(true)
, m_bShowCandidates(false)
{
	myBoardDim = fromField.myBoardDim;
    FinalConstruction();
	myDim = fromField.myDim;
	myDimDP = fromField.myDimDP;
	myCandLen = fromField.myCandLen;
	myCandLenDP = fromField.myCandLenDP;
    myFontSizeDP = fromField.myFontSizeDP;
	myDisplayAlpha = fromField.myDisplayAlpha;
	ptFieldPos = fromField.ptFieldPos;
	ptFieldPosDP = fromField.ptFieldPosDP;
    m_pFieldEdit = new CSudokoEdit;
	InitCandidates();
}

void CSudokuField::FinalConstruction()
{
    pCandidates = new Candidate_t[myBoardDim];
}

CSudokuField::~CSudokuField()
{
    m_pFieldEdit->DestroyWindow();
    delete m_pFieldEdit;
    delete [] pCandidates;
}


// CSudokuField-Memberfunktionen

void CSudokuField::InitSudokoEdit(CWnd* pParent, CPoint ptId)
{
	UINT nId = MAKELONG(ptId.x, ptId.y);

	m_pParent = pParent;

	CRect rcEdit;
	// init and create edit field for number input
	rcEdit.left = ptFieldPos.x + myCandLen;
	rcEdit.right = ptFieldPos.x + myDim - myCandLen;
	rcEdit.top = ptFieldPos.y + myCandLen;
	rcEdit.bottom = ptFieldPos.y + myDim - myCandLen;
	m_pFieldEdit->Create(rcEdit, pParent, myBoardDim, nId);
	// init Print rectangle for number
	m_rcNumber.left = ptFieldPosDP.x + myCandLenDP;
	m_rcNumber.right = ptFieldPosDP.x + myDimDP - myCandLenDP;
	m_rcNumber.top = ptFieldPosDP.y - myCandLenDP;
	m_rcNumber.bottom = ptFieldPosDP.y - myDimDP + myCandLenDP;
}

void CSudokuField::MoveSudokuField(CPoint ptId)
{
	CRect rcEdit;

	// store new field position
	ptFieldPos = ptId;
	// move edit field to new position
	rcEdit.left = ptId.x + myCandLen;
	rcEdit.right = ptId.x + myDim - myCandLen;
	rcEdit.top = ptId.y + myCandLen;
	rcEdit.bottom = ptId.y + myDim - myCandLen;
	m_pFieldEdit->MoveWindow(rcEdit);
	InitCandidates();
}

void CSudokuField::SetCandidate(int iCand)
{
	if (!pCandidates[iCand-1].bForced)
	{
		pCandidates[iCand-1].bVisible = true;
	}
}

void CSudokuField::DeleteCandidate(int iCand)
{
	pCandidates[iCand-1].bVisible = false;
}

void CSudokuField::DrawCandidates(CDC* pDC)
{
	if (m_pFieldEdit->myNumber) return;
	if (!m_bShowCandidates) return;

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	COLORREF clrSingle = RGB(255,0,0);
	COLORREF clrPair = RGB(0,0,255);
    COLORREF clrTriple = RGB(0,255,0);
	COLORREF clrScan = RGB(255,64,0);
	CPen  aSinglePen(PS_SOLID, 2, clrSingle);
	CPen  aPairPen(PS_SOLID, 2, clrPair);
	CPen  aTriplePen(PS_SOLID, 2, clrTriple);
	CPen  aScanPen(PS_SOLID, 2, clrScan);
	CBrush aSingleBrush(clrSingle);
	CBrush aPairBrush(clrPair);
	CBrush aTripleBrush(clrTriple);
	CBrush aScanBrush(clrScan);
	CBrush aNoneBrush(RGB(0,0,0));

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].bHover)
		{
			pDC->Rectangle(pCandidates[i].rcFrame);
		}
		if (pCandidates[i].dispMode == Single ||
            pCandidates[i].dispMode == SingleIF)
		{
			pOldPen = pDC->SelectObject(&aSinglePen);
			pOldBrush = pDC->SelectObject(&aSingleBrush);
		}
		else if (pCandidates[i].dispMode == Pair)
		{
			pOldPen = pDC->SelectObject(&aPairPen);
			pOldBrush = pDC->SelectObject(&aPairBrush);
		}
		else if (pCandidates[i].dispMode == Triple)
		{
			pOldPen = pDC->SelectObject(&aTriplePen);
			pOldBrush = pDC->SelectObject(&aTripleBrush);
		}
		else if (pCandidates[i].dispMode == Scan)
		{
			pOldPen = pDC->SelectObject(&aScanPen);
			pOldBrush = pDC->SelectObject(&aScanBrush);
		}
		else
		{
			pOldBrush = pDC->SelectObject(&aNoneBrush);
		}
		if (pCandidates[i].bVisible)
		{
			if (i != 4 || myBoardDim != 9)
			{
				pDC->MoveTo(pCandidates[i].ptFrom);
				pDC->LineTo(pCandidates[i].ptTo);
			}
			else
			{
				pDC->Ellipse(CRect(pCandidates[i].ptFrom, pCandidates[i].ptTo));
			}
		}
		if (pOldPen) pDC->SelectObject(pOldPen);
		if (pOldBrush) pDC->SelectObject(pOldBrush);
	}
}

void CSudokuField::PrintSudokuField(CDC* pDC)
{
	if (m_pFieldEdit->myNumber)
	{
		// print number
		CFont* pFont = pDC->GetCurrentFont();
		CFont ftNumbers;
		LOGFONT lfNumbers;
		if (pFont) pFont->GetLogFont(&lfNumbers);
		lfNumbers.lfHeight = myFontSizeDP;
		lfNumbers.lfWidth = 0;
		wcscpy_s((wchar_t*)&lfNumbers.lfFaceName,6, _T("Arial"));
		ftNumbers.CreateFontIndirect(&lfNumbers);
		pFont = pDC->SelectObject(&ftNumbers);

		CString szNumber;

        if (myBoardDim > 8 && myDisplayAlpha)
        {
		    szNumber.Format(_T("%c"), m_pFieldEdit->myNumber + 'A' - 1);
        }
        else
        {
		    szNumber.Format(_T("%.1d"), m_pFieldEdit->myNumber);
        }
		pDC->DrawText(szNumber, -1, m_rcNumber, DT_SINGLELINE | DT_CENTER |DT_VCENTER);

		pDC->SelectObject(pFont);
	}
	else
	{
		CPen  penCand(PS_SOLID, 30, RGB(0,0,0));
		CPen* pOldPen = pDC->SelectObject(&penCand);
		CBrush bshCand(RGB(0,0,0));
		CBrush* pOldBrush = pDC->SelectObject(&bshCand);
		for (int i=0; i<myBoardDim; i++)
		{
			if (pCandidates[i].bVisible)
			{
				if (i != 4 || myBoardDim != 9)
				{
					pDC->MoveTo(pCandidates[i].ptFromDP);
					pDC->LineTo(pCandidates[i].ptToDP);
				}
				else
				{
					CRect rcCand(pCandidates[i].ptFromDP, pCandidates[i].ptToDP);
					pDC->Ellipse(&rcCand);
				}
			}
		}
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

void CSudokuField::InitCandidates(void)
{
    int xPos;
    int yPos;
	const double pi = 3.1415926535897932;
	const double arc45  = pi / 4;
	const double arc90  = pi / 2;
	const double arc135 = pi * 3 / 4;
	const double arc225 = pi * 5 / 4;
	const double arc270 = pi * 3 / 2;
	const double arc315 = pi * 7 / 4;
	const double arcDiff = 2 * pi / myBoardDim;

	for (int i=0; i<myBoardDim; i++)
	{
		pCandidates[i].bVisible = true;
		pCandidates[i].bForced = false;
		pCandidates[i].bHover = false;
		pCandidates[i].dispMode = None;
	}

    if (myBoardDim == 9)
    {
	    xPos = ptFieldPos.x;
	    yPos = ptFieldPos.y;
	    // Candidates for view
	    // candidate 9
	    pCandidates[8].ptFrom.x = xPos;
	    pCandidates[8].ptFrom.y = yPos;
	    pCandidates[8].ptTo.x = xPos + myCandLen;
	    pCandidates[8].ptTo.y = yPos + myCandLen;
	    pCandidates[8].rcFrame = CRect(pCandidates[8].ptFrom, pCandidates[8].ptTo);
	    // candidate 1
	    xPos = ptFieldPos.x + myDim / 2;
	    pCandidates[0].ptFrom.x = xPos;
	    pCandidates[0].ptFrom.y = yPos;
	    pCandidates[0].ptTo.x   = xPos;
	    pCandidates[0].ptTo.y   = yPos + myCandLen;
	    pCandidates[0].rcFrame = CRect(xPos-4, yPos, xPos+4, yPos+myCandLen);
	    // candidate 2
	    xPos = ptFieldPos.x + myDim;
	    pCandidates[1].ptFrom.x = xPos;
	    pCandidates[1].ptFrom.y = yPos;
	    pCandidates[1].ptTo.x = xPos - myCandLen;
	    pCandidates[1].ptTo.y = yPos + myCandLen;
	    pCandidates[1].rcFrame = CRect(xPos-myCandLen, yPos, xPos, yPos+myCandLen);
	    // candidate 8
	    xPos = ptFieldPos.x;
	    yPos = ptFieldPos.y + myDim / 2;
	    pCandidates[7].ptFrom.x = xPos;
	    pCandidates[7].ptFrom.y = yPos;
	    pCandidates[7].ptTo.x = xPos + myCandLen;
	    pCandidates[7].ptTo.y = yPos;
	    pCandidates[7].rcFrame = CRect(xPos, yPos-4, xPos+myCandLen, yPos+4);
	    // candidate 5
	    xPos = ptFieldPos.x + myDim / 2;
	    pCandidates[4].ptFrom.x = xPos-2;
	    pCandidates[4].ptFrom.y = yPos-2;
	    pCandidates[4].ptTo.x = xPos+2;
	    pCandidates[4].ptTo.y = yPos+2;
	    pCandidates[4].rcFrame = CRect(xPos-4, yPos-4, xPos+4, yPos+4);
	    // candidate 3
	    xPos = ptFieldPos.x + myDim;
	    pCandidates[2].ptFrom.x = xPos;
	    pCandidates[2].ptFrom.y = yPos;
	    pCandidates[2].ptTo.x = xPos - myCandLen;
	    pCandidates[2].ptTo.y = yPos;
	    pCandidates[2].rcFrame = CRect(xPos-myCandLen, yPos-4, xPos, yPos+4);
	    // candidate 7
	    xPos = ptFieldPos.x;
	    yPos = ptFieldPos.y + myDim;
	    pCandidates[6].ptFrom.x = xPos;
	    pCandidates[6].ptFrom.y = yPos;
	    pCandidates[6].ptTo.x = xPos + myCandLen;
	    pCandidates[6].ptTo.y = yPos - myCandLen;
	    pCandidates[6].rcFrame = CRect(xPos, yPos-myCandLen, xPos+myCandLen, yPos);
	    // candidate 6
	    xPos = ptFieldPos.x + myDim / 2;
	    pCandidates[5].ptFrom.x = xPos;
	    pCandidates[5].ptFrom.y = yPos;
	    pCandidates[5].ptTo.x = xPos;
	    pCandidates[5].ptTo.y = yPos - myCandLen;
	    pCandidates[5].rcFrame = CRect(xPos-4, yPos-myCandLen, xPos+4, yPos);
	    // candidate 4
	    xPos = ptFieldPos.x + myDim;
	    pCandidates[3].ptFrom.x = xPos;
	    pCandidates[3].ptFrom.y = yPos;
	    pCandidates[3].ptTo.x = xPos - myCandLen;
	    pCandidates[3].ptTo.y = yPos - myCandLen;
	    pCandidates[3].rcFrame = CRect(pCandidates[3].ptTo, pCandidates[3].ptFrom);
    }
    else
    {
        double arcInc  = pi * 2 / myBoardDim;
        double arcIter = arcInc;
        int nDiff1, nDiff2, i;
        xPos = ptFieldPos.x + myDim / 2;
        yPos = ptFieldPos.y + myDim / 2;
        CRect rcOuter(ptFieldPos, CSize(myDim, myDim));
        CRect rcInner(rcOuter);
        rcInner.DeflateRect(myCandLen, myCandLen);

        for (i=0; i<myBoardDim; i++)
        {
            if ((arcIter < arc45)||(arcIter > arc315))
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * tan(arcIter));
                nDiff2 = (LONG)(rcInner.Width()/2 * tan(arcIter));
	            pCandidates[i].ptFrom  = CPoint(xPos + nDiff2, rcInner.top);
	            pCandidates[i].ptTo    = CPoint(xPos + nDiff1, rcOuter.top);
                if (nDiff1 == 0)
                {
	                pCandidates[i].rcFrame = CRect(xPos-myCandLen/2, rcOuter.top, xPos+myCandLen/2, rcInner.top);
                }
                else if (nDiff1 > 0)
                {
	                pCandidates[i].rcFrame = CRect(xPos + nDiff2-2, rcOuter.top, xPos + nDiff1+2, rcInner.top);
                }
                else
                {
	                pCandidates[i].rcFrame = CRect(xPos + nDiff1-2, rcOuter.top, xPos + nDiff2+2, rcInner.top);
                }
            }
            else if (arcIter == arc45)
            {
                pCandidates[i].ptFrom  = CPoint(rcInner.right,rcInner.top);
                pCandidates[i].ptTo    = CPoint(rcOuter.right, rcOuter.top);
                pCandidates[i].rcFrame = CRect(rcInner.right, rcOuter.top, rcOuter.right, rcInner.top);
            }
            else if (arcIter < arc135)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * (1/tan(arcIter)));
                nDiff2 = (LONG)(rcInner.Width()/2 * (1/tan(arcIter)));
                pCandidates[i].ptFrom  = CPoint(rcInner.right, yPos-nDiff2);
                pCandidates[i].ptTo    = CPoint(rcOuter.right, yPos-nDiff1);
                if (nDiff1 == 0)
                {
                    pCandidates[i].rcFrame = CRect(rcInner.right, yPos-myCandLen/2, rcOuter.right, yPos+myCandLen/2);
                }
                else if (nDiff1 > 0)
                {
                    pCandidates[i].rcFrame = CRect(rcInner.right, yPos-nDiff1-2, rcOuter.right, yPos-nDiff2+2);
                }
                else
                {
                    pCandidates[i].rcFrame = CRect(rcInner.right, yPos-nDiff2-2, rcOuter.right, yPos-nDiff1+2);
                }
            }
            else if (arcIter == arc135)
            {
                pCandidates[i].ptFrom  = rcInner.BottomRight();
                pCandidates[i].ptTo    = rcOuter.BottomRight();
                pCandidates[i].rcFrame = CRect(rcInner.right, rcInner.bottom, rcOuter.right, rcOuter.bottom);
            }
            else if (arcIter < arc225)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * tan(arcIter));
                nDiff2 = (LONG)(rcInner.Width()/2 * tan(arcIter));
                pCandidates[i].ptFrom  = CPoint(xPos-nDiff2, rcInner.bottom);
                pCandidates[i].ptTo    = CPoint(xPos-nDiff1, rcOuter.bottom);
                if (nDiff1 == 0)
                {
                    pCandidates[i].rcFrame = CRect(xPos-myCandLen/2, rcInner.bottom, xPos+myCandLen/2, rcOuter.bottom);
                }
                else if (nDiff1 > 0)
                {
                    pCandidates[i].rcFrame = CRect(xPos-nDiff1-2, rcInner.bottom, xPos-nDiff2+2, rcOuter.bottom);
                }
                else
                {
                    pCandidates[i].rcFrame = CRect(xPos-nDiff2-2, rcInner.bottom, xPos-nDiff1+2, rcOuter.bottom);
                }
            }
            else if (arcIter == arc225)
            {
                pCandidates[i].ptFrom  = CPoint(rcInner.left, rcInner.bottom);
                pCandidates[i].ptTo    = CPoint(rcOuter.left, rcOuter.bottom);
                pCandidates[i].rcFrame = CRect(rcOuter.left, rcInner.bottom, rcInner.left, rcOuter.bottom);
            }
            else if (arcIter < arc315)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * (1/tan(arcIter)));
                nDiff2 = (LONG)(rcInner.Width()/2 * (1/tan(arcIter)));
                pCandidates[i].ptFrom  = CPoint(rcInner.left, yPos+nDiff2);
                pCandidates[i].ptTo    = CPoint(rcOuter.left, yPos+nDiff1);
                if (nDiff1 == 0)
                {
                    pCandidates[i].rcFrame = CRect(rcOuter.left, yPos-myCandLen/2, rcInner.left, yPos+myCandLen/2);
                }
                else if (nDiff1 > 0)
                {
                    pCandidates[i].rcFrame = CRect(rcOuter.left, yPos+nDiff2-2, rcInner.left, yPos+nDiff1+2);
                }
                else
                {
                    pCandidates[i].rcFrame = CRect(rcOuter.left, yPos+nDiff1-2, rcInner.left, yPos+nDiff2+2);
                }
            }
            else if (arcIter == arc315)
            {
                pCandidates[i].ptFrom  = rcInner.TopLeft();
                pCandidates[i].ptTo    = rcOuter.TopLeft();
                pCandidates[i].rcFrame = CRect(rcInner.TopLeft(), rcOuter.TopLeft());
            }
            arcIter += arcInc;
        }

     }

    if (myBoardDim == 9)
    {
	    // candidates for printing 
	    xPos = ptFieldPosDP.x;
	    yPos = ptFieldPosDP.y;

	    // candidate 9
	    pCandidates[8].ptFromDP.x = xPos;
	    pCandidates[8].ptFromDP.y = yPos;
	    pCandidates[8].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[8].ptToDP.y = yPos - myCandLenDP;

	    // candidate 1
	    xPos = ptFieldPosDP.x + myDimDP / 2;
	    pCandidates[0].ptFromDP.x = xPos;
	    pCandidates[0].ptFromDP.y = yPos;
	    pCandidates[0].ptToDP.x = xPos;
	    pCandidates[0].ptToDP.y = yPos - myCandLenDP;
	    // candidate 2
	    xPos = ptFieldPosDP.x + myDimDP;
	    pCandidates[1].ptFromDP.x = xPos;
	    pCandidates[1].ptFromDP.y = yPos;
	    pCandidates[1].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[1].ptToDP.y = yPos - myCandLenDP;
	    // candidate 8
	    xPos = ptFieldPosDP.x;
	    yPos = ptFieldPosDP.y - myDimDP / 2;
	    pCandidates[7].ptFromDP.x = xPos;
	    pCandidates[7].ptFromDP.y = yPos;
	    pCandidates[7].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[7].ptToDP.y = yPos;
	    // candidate 5
	    xPos = ptFieldPosDP.x + myDimDP / 2;
	    pCandidates[4].ptFromDP.x = xPos - 60;
	    pCandidates[4].ptFromDP.y = yPos + 60;
	    pCandidates[4].ptToDP.x = xPos + 60;
	    pCandidates[4].ptToDP.y = yPos - 60;
	    // candidate 3
	    xPos = ptFieldPosDP.x + myDimDP;
	    pCandidates[2].ptFromDP.x = xPos;
	    pCandidates[2].ptFromDP.y = yPos;
	    pCandidates[2].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[2].ptToDP.y = yPos;
	    // candidate 7
	    xPos = ptFieldPosDP.x;
	    yPos = ptFieldPosDP.y - myDimDP;
	    pCandidates[6].ptFromDP.x = xPos;
	    pCandidates[6].ptFromDP.y = yPos;
	    pCandidates[6].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[6].ptToDP.y = yPos + myCandLenDP;
	    // candidate 6
	    xPos = ptFieldPosDP.x + myDimDP / 2;
	    pCandidates[5].ptFromDP.x = xPos;
	    pCandidates[5].ptFromDP.y = yPos;
	    pCandidates[5].ptToDP.x = xPos;
	    pCandidates[5].ptToDP.y = yPos + myCandLenDP;
	    // candidate 4
	    xPos = ptFieldPosDP.x + myDimDP;
	    pCandidates[3].ptFromDP.x = xPos;
	    pCandidates[3].ptFromDP.y = yPos;
	    pCandidates[3].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[3].ptToDP.y = yPos + myCandLenDP;
    }
    else
    {
        double arcInc  = pi * 2 / myBoardDim;
        double arcIter = arcInc;
        int nDiff1, nDiff2, i;
        xPos = ptFieldPosDP.x + myDimDP / 2;
        yPos = ptFieldPosDP.y - myDimDP / 2;
        CRect rcOuter(ptFieldPosDP, CSize(myDimDP, -myDimDP));
        CRect rcInner(rcOuter);
        rcInner.DeflateRect(myCandLenDP, -myCandLenDP);

        for (i=0; i<myBoardDim; i++)
        {
            if ((arcIter <= arc45)||(arcIter > arc315))
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * tan(arcIter));
                nDiff2 = (LONG)(rcInner.Width()/2 * tan(arcIter));
	            pCandidates[i].ptFromDP  = CPoint(xPos + nDiff2, rcInner.top);
	            pCandidates[i].ptToDP    = CPoint(xPos + nDiff1, rcOuter.top);
            }
            //else if (arcIter == arc45)
            //{
            //    pCandidates[i].ptFromDP  = CPoint(rcInner.right,rcInner.top);
            //    pCandidates[i].ptToDP    = CPoint(rcOuter.right, rcOuter.top);
            //}
            else if (arcIter <= arc135)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * (1/tan(arcIter)));
                nDiff2 = (LONG)(rcInner.Width()/2 * (1/tan(arcIter)));
                pCandidates[i].ptFromDP  = CPoint(rcInner.right, yPos+nDiff2);
                pCandidates[i].ptToDP    = CPoint(rcOuter.right, yPos+nDiff1);
            }
            //else if (arcIter == arc135)
            //{
            //    pCandidates[i].ptFromDP  = rcInner.BottomRight();
            //    pCandidates[i].ptToDP    = rcOuter.BottomRight();
            //}
            else if (arcIter <= arc225)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * tan(arcIter));
                nDiff2 = (LONG)(rcInner.Width()/2 * tan(arcIter));
                pCandidates[i].ptFromDP  = CPoint(xPos-nDiff2, rcInner.bottom);
                pCandidates[i].ptToDP    = CPoint(xPos-nDiff1, rcOuter.bottom);
            }
            //else if (arcIter == arc225)
            //{
            //    pCandidates[i].ptFromDP  = CPoint(rcInner.left, rcInner.bottom);
            //    pCandidates[i].ptToDP    = CPoint(rcOuter.left, rcOuter.bottom);
            //}
            else if (arcIter <= arc315)
            {
                nDiff1 = (LONG)(rcOuter.Width()/2 * (1/tan(arcIter)));
                nDiff2 = (LONG)(rcInner.Width()/2 * (1/tan(arcIter)));
                pCandidates[i].ptFromDP  = CPoint(rcInner.left, yPos-nDiff2);
                pCandidates[i].ptToDP    = CPoint(rcOuter.left, yPos-nDiff1);
            }
            //else if (arcIter == arc315)
            //{
            //    pCandidates[i].ptFromDP  = rcInner.TopLeft();
            //    pCandidates[i].ptToDP    = rcOuter.TopLeft();
            //}
            arcIter += arcInc;
        }

        /*
        int diff1 = (int)((double)myDimDP / 2 * tan(3.141 / 6));
        int diff2 = (int)((double)(myDimDP/2-myCandLenDP) * tan(3.141 / 6));
	    xPos = ptFieldPosDP.x + myDimDP / 2;
	    yPos = ptFieldPosDP.y;
	    // Candidates for view
	    // candidate 12
	    pCandidates[11].ptFromDP.x = xPos;
	    pCandidates[11].ptFromDP.y = yPos;
	    pCandidates[11].ptToDP.x = xPos;
	    pCandidates[11].ptToDP.y = yPos - myCandLenDP;
	    // candidate 1
	    pCandidates[0].ptFromDP.x = xPos + diff1;
	    pCandidates[0].ptFromDP.y = yPos;
	    pCandidates[0].ptToDP.x = xPos + diff2;
	    pCandidates[0].ptToDP.y = yPos - myCandLenDP;
	    // candidate 11
	    pCandidates[10].ptFromDP.x = xPos - diff1;
	    pCandidates[10].ptFromDP.y = yPos;
	    pCandidates[10].ptToDP.x = xPos - diff2;
	    pCandidates[10].ptToDP.y = yPos - myCandLenDP;

        xPos = ptFieldPosDP.x + myDimDP;
        yPos = ptFieldPosDP.y - myDimDP / 2;
	    // candidate 3
	    pCandidates[2].ptFromDP.x = xPos;
	    pCandidates[2].ptFromDP.y = yPos;
	    pCandidates[2].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[2].ptToDP.y = yPos;
	    // candidate 4
	    pCandidates[3].ptFromDP.x = xPos;
	    pCandidates[3].ptFromDP.y = yPos - diff1;
	    pCandidates[3].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[3].ptToDP.y = yPos - diff2;
	    // candidate 2
	    pCandidates[1].ptFromDP.x = xPos;
	    pCandidates[1].ptFromDP.y = yPos + diff1;
	    pCandidates[1].ptToDP.x = xPos - myCandLenDP;
	    pCandidates[1].ptToDP.y = yPos + diff2;

        xPos = ptFieldPosDP.x + myDimDP / 2;
        yPos = ptFieldPosDP.y - myDimDP;
	    // candidate 6
	    pCandidates[5].ptFromDP.x = xPos;
	    pCandidates[5].ptFromDP.y = yPos;
	    pCandidates[5].ptToDP.x = xPos;
	    pCandidates[5].ptToDP.y = yPos + myCandLenDP;
	    // candidate 7
	    pCandidates[6].ptFromDP.x = xPos - diff1;
	    pCandidates[6].ptFromDP.y = yPos;
	    pCandidates[6].ptToDP.x = xPos - diff2;
	    pCandidates[6].ptToDP.y = yPos + myCandLenDP;
	    // candidate 5
	    pCandidates[4].ptFromDP.x = xPos + diff1;
	    pCandidates[4].ptFromDP.y = yPos;
	    pCandidates[4].ptToDP.x = xPos + diff2;
	    pCandidates[4].ptToDP.y = yPos + myCandLenDP;

        xPos = ptFieldPosDP.x;
        yPos = ptFieldPosDP.y - myDimDP / 2;
	    // candidate 9
	    pCandidates[8].ptFromDP.x = xPos;
	    pCandidates[8].ptFromDP.y = yPos;
	    pCandidates[8].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[8].ptToDP.y = yPos;
	    // candidate 10
	    pCandidates[9].ptFromDP.x = xPos;
	    pCandidates[9].ptFromDP.y = yPos + diff1;
	    pCandidates[9].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[9].ptToDP.y = yPos + diff2;
	    // candidate 8
	    pCandidates[7].ptFromDP.x = xPos;
	    pCandidates[7].ptFromDP.y = yPos - diff1;
	    pCandidates[7].ptToDP.x = xPos + myCandLenDP;
	    pCandidates[7].ptToDP.y = yPos - diff2;
		*/
    }
}

void CSudokuField::Serialize(CArchive& ar)
{
    BYTE bTemp;
	UINT nObjSchem;

    CObject::Serialize(ar);

	if (ar.IsStoring())
	{	
        // store members
        ar << myBoardDim
           << myCandLen
           << myCandLenDP 
           << myFontSizeDP
           << myDim << myDimDP
           << ptFieldPos << ptFieldPosDP;
        // store candidates
		for (int i=0; i<myBoardDim; i++)
		{
            bTemp = pCandidates[i].bVisible ? TRUE : FALSE;
			ar << bTemp;
            bTemp = pCandidates[i].bForced ? TRUE : FALSE;
            ar << bTemp;
		}
        // store edit field
		ar << m_pFieldEdit;
	}
	else
	{	// loading code
        // delete existing edit field
        delete m_pFieldEdit;

        // load members
		nObjSchem = ar.GetObjectSchema();
        if (nObjSchem > 1)
        {
            ar >> myBoardDim;
 			if (nObjSchem > 2)
			{
				ar >> myCandLen
				   >> myCandLenDP;
                if (nObjSchem > 3)
                {
                    ar >> myFontSizeDP;
                }
			}
			else
			{
				if (myBoardDim > 9)
				{
					myCandLen = 6;
				}
			}
       }
        else
        {
            myBoardDim = 9;
        }
        FinalConstruction();
        ar >> myDim >> myDimDP
           >> ptFieldPos >> ptFieldPosDP;
        // load candidates
        InitCandidates();
		for (int i=0; i<myBoardDim; i++)
		{
			ar >> bTemp;
            pCandidates[i].bVisible = bTemp ? true : false;
			ar >> bTemp;
            pCandidates[i].bForced = bTemp ? true : false;
		}
        // dynamic creation of edit field during serialization
		ar >> m_pFieldEdit;
	}
}

void CSudokuField::EvalMousePosition(CPoint ptPos)
{
	if (m_bEditMode) return;

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].rcFrame.PtInRect(ptPos))
		{
			pCandidates[i].bHover = true;
			m_pParent->InvalidateRect(pCandidates[i].rcFrame);
		}
		else
		{
			pCandidates[i].bHover = false;
			m_pParent->InvalidateRect(pCandidates[i].rcFrame);
		}
	}
	m_pParent->UpdateWindow();
}

void CSudokuField::ToggleCandidate(CPoint ptPos, int& number)
{
	number = 0;
	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].rcFrame.PtInRect(ptPos))
		{
			if (pCandidates[i].bForced)
			{
				pCandidates[i].bVisible = false;
				pCandidates[i].bForced = false;
			}
			else
			{
				pCandidates[i].bVisible = false;
				pCandidates[i].bForced = true;
			}
			number = i + 1;
			m_pParent->InvalidateRect(pCandidates[i].rcFrame);
		}
	}
}

void CSudokuField::ToggleCandidate(int number)
{
	int i = number - 1;
	if (pCandidates[i].bForced)
	{
		pCandidates[i].bVisible = false;
		pCandidates[i].bForced = false;
	}
	else
	{
		pCandidates[i].bVisible = false;
		pCandidates[i].bForced = true;
	}
	m_pParent->InvalidateRect(pCandidates[i].rcFrame);
}

bool CSudokuField::SelectSingle(BOOL bSelect)
{
	int iCount = 0;
	bool bFound = false;

	for (int i=0; i<myBoardDim; i++)
	{
		if(pCandidates[i].dispMode == SingleIF)
			pCandidates[i].dispMode = None;
	}
	if (bSelect)
	{
		for (int i=0; i<myBoardDim; i++)
		{
			if (pCandidates[i].bVisible)
				iCount++;
		}
		if (iCount == 1)
		{
			for (int i=0; i<myBoardDim; i++)
			{
				if (pCandidates[i].bVisible)
					pCandidates[i].dispMode = SingleIF;
			}
			bFound = true;
		}
	}

	return bFound;
}

bool CSudokuField::IsSingleInField(void)
{
	int iCount = 0;

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].bVisible)
			iCount++;
	}

	return (iCount == 1);
}

bool CSudokuField::SetSingleNumber(void)
{
	int iCount = 0;
	int nNumber = 0;
	bool bModified = false;

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].bVisible)
		{
			iCount++;
			nNumber = i+1;
		}
	}
	if (iCount == 1)
	{
		m_pFieldEdit->SignalNewNumber(nNumber, true);
		bModified = true;
	}
	return bModified;
}

bool CSudokuField::SelectMulti(MultiSelect_t msMode)
{
	int iCount = 0;
	bool bFound = false;

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].dispMode == Pair || pCandidates[i].dispMode == Triple)
			pCandidates[i].dispMode = None;
	}
	
	if (msMode != msNone)
	{
		for (int i=0; i<myBoardDim; i++)
		{
			if (pCandidates[i].bVisible)
			{
				iCount++;
			}
		}
		if (iCount > 1 && iCount <= 3)
		{
			for (int i=0; i<myBoardDim; i++)
			{
				if (pCandidates[i].bVisible)
                {
                    if (iCount == 2 && msMode & msPair)
                    {
					    pCandidates[i].dispMode = Pair;
                    }
                    else if(iCount == 3 && msMode & msTriple)
                    {
					    pCandidates[i].dispMode = Triple;
                    }
                }
			}
			bFound = true;
		}
	}
	return bFound;
}

bool CSudokuField::IsCandidateOn(int iCand)
{
	return pCandidates[iCand].bVisible;
}

void CSudokuField::SelectCandidate(int iCand, BOOL bSelect)
{
	if (bSelect)
	{
		pCandidates[iCand].dispMode = Single;
	}
	else
	{
        if (pCandidates[iCand].dispMode != Scan &&
            pCandidates[iCand].dispMode != SingleIF)
        {
		    pCandidates[iCand].dispMode = None;
        }
	}
}

void CSudokuField::SelectCandidateScan(int c, bool bSelect, BOOL bAllowMulti)
{
    if (!bAllowMulti)
    {
	    for (int i=0; i<myBoardDim; i++)
	    {
		    pCandidates[i].dispMode = None;
	    }
    }

	if (bSelect) 
    {
        pCandidates[c-1].dispMode = Scan;
        if (!bAllowMulti) m_pFieldEdit->myScanNumber = c;
    }
    else
    {
        if (bAllowMulti)pCandidates[c-1].dispMode = None;
        m_pFieldEdit->myScanNumber = 0;
    }
}

void CSudokuField::ShowHideEditField(bool bShow)
{
	int nCmdShow = (bShow || m_pFieldEdit->myNumber || m_bEditMode) ? SW_SHOW : SW_HIDE;
	m_pFieldEdit->ShowWindow(nCmdShow);
}

void CSudokuField::SetEditMode(bool bEditMode)
{
	m_bEditMode = bEditMode;
}

bool CSudokuField::IsNumberSelectedSingle(int nNumber)
{
	bool bRetVal = true;

	for (int i=0; i<myBoardDim; i++)
	{
		if (pCandidates[i].dispMode == Single)
		{
			bRetVal = (nNumber == (i+1));
		}
	}
	return bRetVal;
}

void CSudokuField::ResetCandidates()
{
	for (int i=0; i<myBoardDim; i++)
	{
        if (pCandidates[i].dispMode != Scan)
        {
            pCandidates[i].dispMode = None;
        }
    }    
}

void CSudokuField::WipeField()
{
    m_pFieldEdit->myNumber = 0;

    for (int i=0; i<myBoardDim; i++)
	{
        pCandidates[i].bVisible = true;
    }

    CleanCandHover();
}

void CSudokuField::CleanCandHover()
{
	for (int i=0; i<myBoardDim; i++)
	{
        pCandidates[i].bHover = false;
		m_pParent->InvalidateRect(pCandidates[i].rcFrame);
    }
	m_pParent->UpdateWindow();
}

void CSudokuField::EnableField(bool bEnable)
{
	int nCmdShow = bEnable ? SW_SHOW : SW_HIDE;
	m_pFieldEdit->ShowWindow(nCmdShow);
}

// Schaltet die Kandidatenanzeige an oder aus
void CSudokuField::ShowCandidates(bool bShowCandidates)
{
	m_bShowCandidates = bShowCandidates;
}

CSudokuField& CSudokuField::operator&(CSudokuField& Right)
{
	for (int i=0; i<myBoardDim; i++)
	{
		pCandidates[i].bVisible &= Right.pCandidates[i].bVisible;
	}
	return *this;
}

CSudokuField& CSudokuField::operator=(CSudokuField& Right)
{
	for (int i=0; i<myBoardDim; i++)
	{
		pCandidates[i].bVisible = Right.pCandidates[i].bVisible;
	}
	return *this;
}
void CSudokuField::SetNumber(int nNumber)
{
	m_pFieldEdit->SignalNewNumber(nNumber, true);
}
