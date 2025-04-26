// SudokuBoard.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuBoard.h"
#include ".\sudokuboard.h"

// CSudokuBoard

IMPLEMENT_SERIAL(CSudokuBoard, CObject, VERSIONABLE_SCHEMA | 5)
CSudokuBoard::CSudokuBoard()
:
 myParent(NULL)
,myBoardDim(9)
,myCandCounts(NULL)
,myBoardIndex(-1)
,arrDisable(NULL)
,mySudokuSquares(NULL)
,mySudokuColumns(NULL)
,mySudokuRows(NULL)
,m_bConstructed(false)
,myHoriIndent(SUDOKU_HINDENT)
,myHoriIndentDP(SUDOKU_INDENT_DP)
,myVertIndent(SUDOKU_VINDENT)
,myVertIndentDP(SUDOKU_INDENT_DP)
,myFieldExt(FIELD_EXT)
,myFieldExtDP(FIELD_EXT_DP)
,myCandLen(CAND_LEN)
,myCandLenDP(CAND_LEN_DP)
,myFontSizeDP(1000)
,myBoardAlignment(1)
,myDisplayAlpha(TRUE)
,myCandCounter(NULL)
,mySingleCounter(NULL)
,m_backColor(RGB(255,220,157))
{
}

CSudokuBoard::CSudokuBoard(int nBoardDim)
:
 myParent(NULL)
,myBoardDim(nBoardDim)
,myBoardIndex(-1)
,m_bConstructed(false)
,myHoriIndent(SUDOKU_HINDENT)
,myHoriIndentDP(SUDOKU_INDENT_DP)
,myVertIndent(SUDOKU_VINDENT)
,myVertIndentDP(SUDOKU_INDENT_DP)
,myFieldExt(FIELD_EXT)
,myFieldExtDP(FIELD_EXT_DP)
,myCandLen(CAND_LEN)
,myCandLenDP(CAND_LEN_DP)
,myFontSizeDP(1000)
,myBoardAlignment(1)
,myDisplayAlpha(TRUE)
,myCandCounter(NULL)
,mySingleCounter(NULL)
,m_backColor(RGB(255,220,157))
{
	if (myBoardDim > 9)
	{
		myFieldExt = 40;
		myCandLen  = 6;
	}
	else
	{
		myDisplayAlpha = FALSE;
	}
    if (myBoardDim > 12)
    {
        myFieldExtDP = 1200;
        myCandLenDP  = 180;
    }

	FinalConstruction();
}

CSudokuBoard::~CSudokuBoard()
{
	if (!m_bConstructed) return;

    DeleteSudokuFields();
    delete [] myCandCounts;
    delete [] arrDisable;
    delete [] mySudokuRows;
    delete [] mySudokuColumns;
    delete [] mySudokuSquares;
    delete myCandCounter;
    delete mySingleCounter;
}

void CSudokuBoard::FinalConstruction()
{
    myCandCounts    = new int[myBoardDim];
    arrDisable      = new BOOL[myBoardDim];
    mySudokuSquares = new CObArray[myBoardDim];
    mySudokuColumns = new CObArray[myBoardDim];
    mySudokuRows    = new CObArray[myBoardDim];
    myCandCounter   = new CCandCounter(myBoardDim);
    mySingleCounter = new CCandCounter(myBoardDim);
	m_iLastVertPos	= 0;
	m_iLastHoriPos	= 0;
    for (int i=0; i<myBoardDim; i++)
    {
        arrDisable[i] = TRUE;
        myCandCounts[i] = 0;
    }
    m_bConstructed = true;
}

// CSudokuBoard-Memberfunktionen

void CSudokuBoard::CreateEmptyBoard()
{
	CPoint ptPos, ptPosDP;
	int x, y;
    CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		ptPosDP.y = -(myVertIndentDP + y * myFieldExtDP);
		for ( x=0; x<myBoardDim; x++)
		{
			ptPos.x = myHoriIndent + x * myFieldExt;
			ptPosDP.x = myHoriIndentDP + x * myFieldExtDP;
			pField = new CSudokuField(ptPos, ptPosDP, 
								      myBoardDim, 
									  myFieldExt, myFieldExtDP,
									  myCandLen, myCandLenDP, myFontSizeDP);
			AddSudokuField(pField, x, y);
		}
	}
}

void CSudokuBoard::WipeBoard()
{
	CSudokuField* pField;
    int i, j;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
            pField->WipeField();
        }
    }
}

void CSudokuBoard::AddSudokuField(CSudokuField* pField, int iHoriPos, int iVertPos)
{
    int nVertMod;
    int nHoriMod;

	CALC_MODULO

	// add field to row
	mySudokuRows[iVertPos].Add(pField);
	// add field to column
	mySudokuColumns[iHoriPos].Add(pField);
	// add field to square
	int iIndex = (iHoriPos/nHoriMod) + ((iVertPos/nVertMod)*nVertMod);
	mySudokuSquares[iIndex].Add(pField);
}

void CSudokuBoard::UpdateAlignment(int nNewAlign)
{
	int x, y;
    CSudokuField* pField = NULL;
    int nVertMod;
    int nHoriMod;

	if (nNewAlign != myBoardAlignment)
	{
		for (x=0; x<myBoardDim; x++)
		{
			// remove all square fields
			mySudokuSquares[x].RemoveAll();
		}
		// reorganize square fields
		myBoardAlignment = nNewAlign;
		CALC_MODULO;
		for (y=0; y<myBoardDim; y++)
		{
			for ( x=0; x<myBoardDim; x++)
			{
				// first get a field
				pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
				// then put it into its new square
				int iIndex = (x/nHoriMod) + ((y/nVertMod)*nVertMod);
				mySudokuSquares[iIndex].Add(pField);
			}
		}
	}
}

void CSudokuBoard::DeleteSudokuFields()
{
	CSudokuField* pField;
    int i, j;

    if (!m_bConstructed) return;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            if (mySudokuRows[i].GetSize())
            {
                pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
                delete pField;
            }
        }
        mySudokuRows[i].RemoveAll();
        mySudokuColumns[i].RemoveAll();
        mySudokuSquares[i].RemoveAll();
    }
}

void CSudokuBoard::SelectMulti(MultiSelect_t msMode)
{
	CSudokuField* pField;
    int i, j;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
            pField->SelectMulti(msMode);
        }
    }
}

void CSudokuBoard::SelectSingleInField(BOOL bSelect)
{
	CSudokuField* pField;
    int i, j;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
            pField->SelectSingle(bSelect);
        }
    }
}

void CSudokuBoard::SelectSingles(BOOL bSelect)
{
	SelectSingles(mySudokuRows, bSelect);
	SelectSingles(mySudokuColumns, bSelect);
	SelectSingles(mySudokuSquares, bSelect);
}

void CSudokuBoard::SelectSingles(CObArray* pFieldArray, BOOL bSelect)
{
	int i, j, c;
	int iCount = 0;
	int iSingleIndex;
	CSudokuField* pField = NULL;

	// Iterate Field arrays
	for (i=0; i<myBoardDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<myBoardDim; c++)
		{
			for (j=0; j<nSize; j++)
			{
				pField = (CSudokuField*)pFieldArray[i].GetAt(j);
				if (pField->IsCandidateOn(c))
				{
					// count visible candidates
					iCount++;
					iSingleIndex = j;
				}
			}
			if (iCount == 1)
			{
				// Select single candidate in this field
				pField = (CSudokuField*)pFieldArray[i].GetAt(iSingleIndex);
				pField->SelectCandidate(c, bSelect);
			}
			iCount = 0;
		}
	}
}

bool CSudokuBoard::SolveLevel1(void)
{
	CSudokuField* pField;
    int i, j;
	bool bModified = false;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
			bModified |= pField->SetSingleNumber();
        }
    }
	return bModified;
}

bool CSudokuBoard::SolveLevel2(void)
{
	bool bModified = false;

	bModified |= SolveInArray(mySudokuRows);
	bModified |= SolveInArray(mySudokuColumns);
	bModified |= SolveInArray(mySudokuSquares);

	return bModified;
}

bool CSudokuBoard::SolveInArray(CObArray* pFieldArray)
{
	int i, j, c;
	int iCount = 0;
	int iSingleIndex;
	CSudokuField* pField = NULL;
	bool bModified = false;

	// Iterate Field arrays
	for (i=0; i<myBoardDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<myBoardDim; c++)
		{
			for (j=0; j<nSize; j++)
			{
				pField = (CSudokuField*)pFieldArray[i].GetAt(j);
				if (pField->IsCandidateOn(c))
				{
					// count visible candidates
					iCount++;
					iSingleIndex = j;
				}
			}
			if (iCount == 1)
			{
				// Set single number
				pField = (CSudokuField*)pFieldArray[i].GetAt(iSingleIndex);
				pField->SetNumber(c+1);
				bModified |= true;
			}
			iCount = 0;
		}
	}
	return bModified;
}

void CSudokuBoard::Serialize(CArchive& ar)
{
	UINT nSchema; 
    CObject::Serialize(ar);

	CSudokuField* pField;

    if (ar.IsLoading())
    {
        DeleteSudokuFields();
    }

	nSchema = ar.GetObjectSchema();
	if (ar.IsStoring())
	{
        ar << myBoardDim
           << myHoriIndentDP
           << myVertIndent
           << myVertIndentDP
           << myFieldExt
           << myFieldExtDP
           << myFontSizeDP
           << myHoriIndent
		   << myBoardAlignment;
	}
	else
	{
		if (nSchema > 1)
		{
			ar >> myBoardDim;
			if (nSchema > 2)
			{
				ar >> myHoriIndentDP
				   >> myVertIndent
				   >> myVertIndentDP
				   >> myFieldExt
				   >> myFieldExtDP;
                if (nSchema > 3)
                {
                    ar >> myFontSizeDP
                       >> myHoriIndent;
					if (nSchema > 4)
					{
						ar >> myBoardAlignment;
					}
                }
			}
			else if (myBoardDim > 9)
			{
				myVertIndent = 100;
				myFieldExt = 40;
				myCandLen  = 6;
			}
		}
		else
		{
			myBoardDim = 9;
			myVertIndent = 100;
		}
	}

    if (ar.IsLoading())
    {
        FinalConstruction();
    }

	for (int y=0; y<myBoardDim; y++)
	{
		for (int x=0; x<myBoardDim; x++)
		{
            if (ar.IsStoring())
            {
                pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
                ar << pField;
            }
            else
            {
                ar >> pField;
                AddSudokuField(pField, x, y);
            }
		}
	}
}

void CSudokuBoard::DrawSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;
	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 2, RGB(0,0,0));
    CBrush* pOldBrush;
    CBrush aBkBrush(m_backColor);
	int i;
    int nVertMod;
    int nHoriMod;

	CALC_MODULO

	// draw rectangle
    pOldBrush = pDC->SelectObject(&aBkBrush);
	pOldPen = pDC->SelectObject(&aBoldPen);
	pDC->Rectangle(myHoriIndent,myVertIndent,myFieldExt*myBoardDim + myHoriIndent+1, myFieldExt*myBoardDim + myVertIndent+1);
	pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
	// draw horizontal lines
	int y = myFieldExt + myVertIndent;
	int x = myFieldExt * myBoardDim + myHoriIndent;
	for (i=1; i<myBoardDim; i++)
	{
		if (!(i%nVertMod))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndent, y);
		pDC->LineTo(x,y);
		y += myFieldExt;
	}
	// draw vertical lines
	y = myFieldExt * myBoardDim + myVertIndent;
	x = myFieldExt + myHoriIndent;
	for (i=1; i<myBoardDim; i++)
	{
		if (!(i%nHoriMod))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x, myVertIndent);
		pDC->LineTo(x,y);
		x += myFieldExt;
	}
	for (int i=0; i<myBoardDim; i++)
	{
		for (int j=0; j<myBoardDim; j++)
		{
			pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
			pField->DrawCandidates(pDC);
		}
	}
}

void CSudokuBoard::PrintSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;

	int iSquareDim;
	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 60, RGB(0,0,0));
	CPen   aThinPen(PS_SOLID, 30, RGB(0,0,0));
	int i;
    int nVertMod;
    int nHoriMod;

	CALC_MODULO

	iSquareDim = myFieldExtDP;
	// draw rectangle
	pOldPen = pDC->SelectObject(&aBoldPen);
	pDC->Rectangle(myHoriIndentDP,-myVertIndentDP,iSquareDim*myBoardDim + myHoriIndentDP+30, -iSquareDim*myBoardDim - myVertIndentDP-30);
	pDC->SelectObject(pOldPen);
	// draw horizontal lines
	int y = -iSquareDim - myVertIndentDP;
	int x = iSquareDim * myBoardDim + myHoriIndentDP;
	for (i=1; i<myBoardDim; i++)
	{
		if (!(i%nVertMod))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(myHoriIndentDP, y);
		pDC->LineTo(x,y);
		y -= iSquareDim;
	}
	// draw vertical lines
	y = -iSquareDim * myBoardDim - myVertIndentDP;
	x = iSquareDim + myHoriIndentDP;
	for (i=1; i<myBoardDim; i++)
	{
		if (!(i%nHoriMod))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(x, -myVertIndentDP);
		pDC->LineTo(x,y);
		x += iSquareDim;
	}
	pDC->SelectObject(pOldPen);

	// draw canditates in all fields
	for (int i=0; i<myBoardDim; i++)
	{
		for (int j=0; j<myBoardDim; j++)
		{
			pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
			pField->PrintSudokuField(pDC);
		}
	}
}

void CSudokuBoard::InitSudokuField(CWnd* pParent)
{
	CSudokuField* pField;
	CPoint ptId;

    myParent = pParent;

	for (int i=0; i<myBoardDim; i++)
	{
		for (int j=0; j<myBoardDim; j++)
		{
			ptId = CPoint(j,i);
			pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
			pField->InitSudokoEdit(pParent, ptId);
		}
	}
}

void CSudokuBoard::UpdateBoardLayout()
{
	CPoint ptPos, ptPosDP;
	int x, y;
    CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		ptPosDP.y = -(myVertIndentDP + y * myFieldExtDP);
		for ( x=0; x<myBoardDim; x++)
		{
			ptPos.x = myHoriIndent + x * myFieldExt;
			ptPosDP.x = myHoriIndentDP + x * myFieldExtDP;
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
            pField->UpdateLayout(ptPos, ptPosDP, 
								 myFieldExt, myFieldExtDP,
								 myCandLen, myCandLenDP, myFontSizeDP, myDisplayAlpha,
								 m_backColor);
		}
	}
}

bool CSudokuBoard::IsNumberSelectedSingle(CPoint ptId, int nNumber)
{
    // is number a visible single candidate
    CSudokuField* pField = (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
    return pField->IsNumberSelectedSingle(nNumber);
}

bool CSudokuBoard::IsDuplicate(CPoint ptId, int nNumber)
{
	bool bRet = false;
    int nVertMod;
    int nHoriMod;

	CALC_MODULO

	// find duplicate in row
	bRet |= IsDuplicate(mySudokuRows[ptId.y], ptId.x, nNumber);

	// find duplicate in column
	bRet |= IsDuplicate(mySudokuColumns[ptId.x], ptId.y, nNumber);

	// find duplicate in square
	int nIndex = (ptId.y / nVertMod * nVertMod) + (ptId.x / nHoriMod);
	int nOwnPos = (ptId.y % nVertMod * nHoriMod) + (ptId.x % nHoriMod); 
	bRet |= IsDuplicate(mySudokuSquares[nIndex], nOwnPos, nNumber);

	return bRet;
}

bool CSudokuBoard::IsDuplicate(CObArray& pFieldArray , int nOwnPos , int nNumber)
{
	bool bRet = false;
	CSudokuField* pField;

    int nSize = (int)pFieldArray.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pField = (CSudokuField*)pFieldArray.GetAt(i);
		if (i != nOwnPos)
		{
			bRet |= (pField->m_pFieldEdit->myNumber == nNumber);
		}
	}
	return bRet;
}

bool CSudokuBoard::IsSingleInFields(int& nCount)
{
	nCount = 0;
	CSudokuField* pField;
    int i, j;
    UINT nID;

    mySingleCounter->Reset();
    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
			if (pField->IsSingleInField())
			{
                // get field position
                nID = pField->m_pFieldEdit->m_nId;
                // mark single in array
                mySingleCounter->SetSingle(nID);
			}
        }
    }
    nCount = mySingleCounter->GetSingleCount();
	return (nCount > 0);
}

bool CSudokuBoard::IsSingleOnBoard(int& nCount)
{
	nCount = 0;
    myCandCounter->Reset();
    // find all singles on board
	FindSingleOnBoard(mySudokuRows);
	FindSingleOnBoard(mySudokuColumns);
	FindSingleOnBoard(mySudokuSquares);
    // clear field singles
    myCandCounter->ClearSingleFields(mySingleCounter);
    // get remaining singles
    nCount = myCandCounter->GetSingleCount();
	return (nCount > 0);
}

void CSudokuBoard::FindSingleOnBoard(CObArray *pFieldArray)
{
	int i, j, c;
	int iCount = 0;
	CSudokuField* pField = NULL;
    UINT nID;

	// Iterate Field arrays
	for (i=0; i<myBoardDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<myBoardDim; c++)
		{
			for (j=0; j<nSize; j++)
			{
				pField = (CSudokuField*)pFieldArray[i].GetAt(j);
				if (pField->IsCandidateOn(c))
				{
                    // memory last field position
                    nID = pField->m_pFieldEdit->m_nId;
					// count visible candidates
					iCount++;
				}
			}
			if (iCount == 1)
			{
				// Single was found
                myCandCounter->SetSingle(nID);
			}
			iCount = 0;
		}
	}
}

void CSudokuBoard::DeleteCandidates(int xPos, int yPos, int nNumber)
{
	bool bRet = false;
    int nVertMod;
    int nHoriMod;

	CALC_MODULO

	// delete in row
	DeleteCandidates(mySudokuRows[yPos], nNumber);

	// delete in column
	DeleteCandidates(mySudokuColumns[xPos], nNumber);

	// delete in square
	int nIndex = (yPos / nVertMod * nVertMod) + (xPos / nHoriMod);
	DeleteCandidates(mySudokuSquares[nIndex], nNumber);
}

void CSudokuBoard::DeleteCandidates(CObArray& pFieldArray, int nNumber)
{
	CSudokuField* pField;

    int nSize = (int)pFieldArray.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pField = (CSudokuField*)pFieldArray.GetAt(i);
		pField->DeleteCandidate(nNumber);
	}
}

void CSudokuBoard::RefreshCandidates()
{
	int x, y, c;
	CSudokuField* pField;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			for (c=1; c<myBoardDim+1; c++)
			{
				if (!IsDuplicate(CPoint(x,y), c) &&
					!pField->m_pFieldEdit->myNumber)
				{
					pField->SetCandidate(c);
				}
				else
				{
					pField->DeleteCandidate(c);
				}
			}
		}
	}
}

void CSudokuBoard::ResetCandidates()
{
	CSudokuField* pField;
    int i, j;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
            pField->ResetCandidates();
        }
    }
}

void CSudokuBoard::UpdateNumberCompletion()
{
    BOOL bResult;

    for (int i = 0; i < myBoardDim; i++)
    {
        bResult = CheckNumberCompletion(i+1);
        if (bResult != arrDisable[i])
        {
            //AfxGetMainWnd()->SendMessage(WM_ENABLE_SCAN_BUTTON, (WPARAM) i+1, bResult);
            arrDisable[i] = bResult;
        }
    }

    if (myParent) myParent->SendMessage(WM_UPDATE_SCORE, (WPARAM)myCandCounts);
}

BOOL CSudokuBoard::CheckNumberCompletion(int nNumber)
{
	CSudokuField* pField;
    int i, j;
    int nCount = 0;

    for (i=0; i<myBoardDim; i++)
    {
        for (j=0; j<myBoardDim; j++)
        {
            pField = (CSudokuField*)mySudokuRows[i].GetAt(j);
            if (pField->m_pFieldEdit->myNumber == nNumber) nCount++;
        }
    }

    myCandCounts[nNumber-1] = nCount;

    return (nCount == myBoardDim ? FALSE : TRUE);
}

void CSudokuBoard::SetFieldNumber(CPoint ptId, int nNumber)
{
	CSudokuField* pField;
    pField = (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
    pField->m_pFieldEdit->myNumber = nNumber;
}

int CSudokuBoard::GetFieldNumber(CPoint ptId)
{
	CSudokuField* pField;
    pField = (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
    return pField->m_pFieldEdit->myNumber;
}

int CSudokuBoard::GetFieldNumberOld(CPoint ptId)
{
	CSudokuField* pField;
    pField = (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
    return pField->m_pFieldEdit->myOldNumber;
}

void CSudokuBoard::SetEditFocus(CPoint ptId)
{
	CSudokuField* pField;
    pField = (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
    pField->m_pFieldEdit->SetFocus();
}

CSudokuField* CSudokuBoard::GetSudokuField(CPoint ptId)
{
    return (CSudokuField*)mySudokuRows[ptId.y].GetAt(ptId.x);
}

void CSudokuBoard::SelectScan(int c, bool bSelect, BOOL bAllowMulti)
{
	int x, y;
	CSudokuField* pField;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->SelectCandidateScan(c, bSelect, bAllowMulti);
		}
	}
}

void CSudokuBoard::CleanHover()
{
	int x, y;
	CSudokuField* pField;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->CleanCandHover();
		}
	}
}

void CSudokuBoard::SwitchEditMode(bool bEditMode)
{
    int x,y;
	CSudokuField* pField;

	for (x=0; x<myBoardDim; x++)
	{
		for (y=0; y<myBoardDim; y++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->SetEditMode(bEditMode);
		}
	}
}

int CSudokuBoard::GetBoardIndex()
{
    return (myBoardDim == 9 ? 0 : 1);
}

void CSudokuBoard::GetMovedCursor(CPoint& ptId, int iMoveDir)
{
	int nBorder = myBoardDim-1;
	switch (iMoveDir)
	{
	case MOVE_DIR_RIGHT:
		ptId.x++;
		if (ptId.x > nBorder) ptId.x = 0;
		break;
	case MOVE_DIR_DOWN:
		ptId.y++;
		if (ptId.y > nBorder) ptId.y = 0;
		break;
	case MOVE_DIR_LEFT:
		ptId.x--;
		if (ptId.x < 0) ptId.x = nBorder;
		break;
	case MOVE_DIR_UP:
		ptId.y--;
		if (ptId.y < 0) ptId.y = nBorder;
		break;
	}
}
void CSudokuBoard::SetBoardIndex(int nIndex)
{
	myBoardIndex = nIndex;
}

void CSudokuBoard::EnableBoard(bool bEnable)
{
    int x,y;
	CSudokuField* pField;

	for (x=0; x<myBoardDim; x++)
	{
		for (y=0; y<myBoardDim; y++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->EnableField(bEnable);
		}
	}
	if (bEnable)
	{
		m_iLastHoriPos = 0;
		m_iLastVertPos = 0;
	}
}

// Schaltet die Kandidatenanzeige für alle Felder an oder aus
void CSudokuBoard::ShowCandidates(bool bShowCandidates)
{
    int x,y;
	CSudokuField* pField;

	for (x=0; x<myBoardDim; x++)
	{
		for (y=0; y<myBoardDim; y++)
		{
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->ShowCandidates(bShowCandidates);
		}
	}
	myParent->Invalidate();
	myParent->UpdateWindow();
}

void CSudokuBoard::EvalMousePosition(CPoint ptPos)
{
	int iHoriPos = (ptPos.x - myHoriIndent) / myFieldExt;
	int iVertPos = (ptPos.y - myVertIndent) / myFieldExt;

	// check if mouse is over sudoku rectangle
	if ((iHoriPos > (myBoardDim-1) || iVertPos > (myBoardDim-1)) ||
		(ptPos.x < myHoriIndent || ptPos.y < myVertIndent))
	{
		CSudokuField* pField = GetSudokuField(CPoint(m_iLastHoriPos, m_iLastVertPos));
		pField->CleanCandHover();
		pField->ShowHideEditField(true);
		m_iLastHoriPos = 0;
		m_iLastVertPos = 0;
		return;
	}

	if (iHoriPos != m_iLastHoriPos || iVertPos != m_iLastVertPos)
	{
		CSudokuField* pField = GetSudokuField(CPoint(m_iLastHoriPos, m_iLastVertPos));
		pField->CleanCandHover();
		pField->ShowHideEditField(true);
		GetSudokuField(CPoint(iHoriPos, iVertPos))->ShowHideEditField(false);
		m_iLastHoriPos = iHoriPos;
		m_iLastVertPos = iVertPos;
	}

	GetSudokuField(CPoint(iHoriPos, iVertPos))->EvalMousePosition(ptPos);
}

bool CSudokuBoard::ToggleCandidate(CPoint ptPos, int& iHoriPos, int& iVertPos, int& nNumber)
{
	iHoriPos = (ptPos.x - myHoriIndent) / myFieldExt;
	iVertPos = (ptPos.y - myVertIndent) / myFieldExt;

	// check if mouse is over sudoku rectangle
	if (iHoriPos > (myBoardDim-1) || iVertPos > (myBoardDim-1)) return false;

	GetSudokuField(CPoint(iHoriPos, iVertPos))->ToggleCandidate(ptPos, nNumber);
	return true;
}

void CSudokuBoard::ToggleCandidate(CPoint ptId, int nNumber)
{
	GetSudokuField(ptId)->ToggleCandidate(nNumber);
}

void CSudokuBoard::MoveBoard(MoveDirection_t moveDir, int nMoveValue)
{
	CPoint ptPos;
    int x,y;
	CSudokuField* pField = NULL;

	switch (moveDir)
	{
	case moveUp:
		myVertIndent -= nMoveValue;
		break;
	case moveDown:
		myVertIndent += nMoveValue;
		break;
	case moveLeft:
		myHoriIndent -= nMoveValue;
		break;
	case moveRight:
		myHoriIndent += nMoveValue;
		break;
	}

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		for (x=0; x<myBoardDim; x++)
		{
			ptPos.x = myHoriIndent + x * myFieldExt;
            pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->MoveSudokuField(ptPos);
		}
	}
	myParent->Invalidate();
	myParent->UpdateWindow();
}

void CSudokuBoard::SetBoardProperties(SudokuProps_t props)
{
    myHoriIndent		= props.myHoriIndent;
    myVertIndent		= props.myVertIndent;
    myFieldExt			= props.myFieldSize;
    myCandLen			= props.myCandLength;
    myHoriIndentDP		= props.myHoriIndentDP;
    myVertIndentDP		= props.myVertIndentDP;
    myFieldExtDP		= props.myFieldSizeDP;
    myCandLenDP			= props.myCandLengthDP;
    myFontSizeDP		= props.myFontSizeDP;
	myDisplayAlpha      = props.myDisplayAlpha;
	m_backColor			= props.myBackColor;
	UpdateAlignment(props.myBoardAlignment);
    UpdateBoardLayout();
	RefreshCandidates();
}

void CSudokuBoard::GetBoardProperties(SudokuProps_t& props)
{
    props.myHoriIndent		= myHoriIndent;    
    props.myVertIndent		= myVertIndent;    
    props.myFieldSize		= myFieldExt;       
    props.myCandLength		= myCandLen;       
    props.myHoriIndentDP	= myHoriIndentDP;  
    props.myVertIndentDP	= myVertIndentDP;
    props.myFieldSizeDP		= myFieldExtDP;
    props.myCandLengthDP	= myCandLenDP;
    props.myFontSizeDP		= myFontSizeDP;  
	props.myBoardAlignment	= myBoardAlignment;
	props.myDisplayAlpha    = myDisplayAlpha;
	props.myBackColor		= m_backColor;
}
