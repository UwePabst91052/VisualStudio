#include "StdAfx.h"
#include "SudokuMDI.h"
#include ".\sudokuboardsamurai.h"

// CSudokuBoardSamurai

IMPLEMENT_SERIAL(CSudokuBoardSamurai, CObject, VERSIONABLE_SCHEMA | 2)
CSudokuBoardSamurai::CSudokuBoardSamurai(void)
{
}

CSudokuBoardSamurai::CSudokuBoardSamurai(int nBoardDim)
{
    myBoardDim = nBoardDim;
	FinalConstruction();
}

CSudokuBoardSamurai::~CSudokuBoardSamurai(void)
{
    DeleteSudokuFields();
    delete [] myCandCounts;
    delete [] arrDisable;
    delete [] mySudokuRows;
    delete [] mySudokuColumns;
    delete [] mySudokuSquares;
	delete [] mySudokuUnions;
	delete myCandCounter;
	delete mySingleCounter;
	m_bConstructed = false;
}

/***********************************
*    Initialization Functions      *
***********************************/

void CSudokuBoardSamurai::CreateEmptyBoard()
{
	CPoint ptPos, ptPosDP;
	int x, y;
    CSudokuField* pField = NULL;
	int nCount = 0;

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		ptPosDP.y = -(myVertIndentDP + y * myFieldExtDP);
		for ( x=0; x<myBoardDim; x++)
		{
			if (IsValidField(CPoint(x,y)))
			{
				ptPos.x = myHoriIndent + x * myFieldExt;
				ptPosDP.x = myHoriIndentDP + x * myFieldExtDP;
				pField = new CSudokuField(ptPos, ptPosDP, 
										9, 
										myFieldExt, myFieldExtDP,
										myCandLen, myCandLenDP, myFontSizeDP);
				nCount++;
				AddSudokuField(pField, x, y);
			}
		}
	}
	TRACE1("Ich habe %d Felder erzeugt\n", nCount);
}

void CSudokuBoardSamurai::WipeBoard()
{
	CSudokuField* pField;
	int x, y;

    for (x=0; x<9; x++)
    {
        for (y=0; y<45; y++)
        {
			pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->WipeField();
        }
    }
}

void CSudokuBoardSamurai::EnableBoard(bool bEnable)
{
    int x,y;
	CSudokuField* pField = NULL;

	for (x=0; x<myBoardDim; x++)
	{
		for (y=0; y<myBoardDim; y++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->EnableField(bEnable);
			}
		}
	}
	if (bEnable)
	{
		m_iLastHoriPos = 0;
		m_iLastVertPos = 0;
	}
}

void CSudokuBoardSamurai::CleanHover()
{
	int x, y;
	CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->CleanCandHover();
			}
		}
	}
}

void CSudokuBoardSamurai::RefreshCandidates()
{
	int x, y, c;
	CPoint ptVect[4];
	CSudokuField* pField;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			VectorResult_t vr = BoardToVector(CPoint(x,y), ptVect);
			while (vr > vrInvalid)
			{
				if (vr == vrValidSingle)
				{
					pField = (CSudokuField*)mySudokuRows[ptVect[0].y].GetAt(ptVect[0].x);
				}
				else
				{
					pField = (CSudokuField*)mySudokuRows[ptVect[2].y].GetAt(ptVect[2].x);
				}
				for (c=1; c<10; c++)
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
				int iVr = (int)vr;
				iVr--;
				vr = (VectorResult_t)iVr;
			}
		}
	}
}

void CSudokuBoardSamurai::UpdateNumberCompletion()
{
    BOOL bResult;

    for (int i = 0; i < 9; i++)
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

void CSudokuBoardSamurai::ResetCandidates()
{
    int x, y;
	CSudokuField* pField = NULL;

    for (y=0; y<myBoardDim; y++)
    {
        for (x=0; x<myBoardDim; x++)
        {
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->ResetCandidates();
			}
        }
    }
}

void CSudokuBoardSamurai::InitSudokuField(CWnd* pParent)
{
    int x, y;
	CSudokuField* pField = NULL;
	CPoint ptId;

    myParent = pParent;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			ptId = CPoint(x,y);
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->InitSudokoEdit(pParent, ptId);
			}
		}
	}
}

void CSudokuBoardSamurai::UpdateBoardLayout()
{
	CPoint ptPos, ptPosDP;
	int x, y;
	CPoint ptId;
    CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		ptPosDP.y = -(myVertIndentDP + y * myFieldExtDP);
		for ( x=0; x<myBoardDim; x++)
		{
			if (IsValidField(CPoint(x,y)))
			{
				ptPos.x = myHoriIndent + x * myFieldExt;
				ptPosDP.x = myHoriIndentDP + x * myFieldExtDP;
			    ptId = CPoint(x,y);
			    pField = GetDisplayField(CPoint(x,y));
                if (pField)
                {
                    pField->UpdateLayout(ptPos, ptPosDP, 
									     myFieldExt, myFieldExtDP,
									     myCandLen, myCandLenDP, myFontSizeDP, myDisplayAlpha,
								         m_backColor);
                }
			}
		}
	}
}

/**********************************
*    Query Functions              *
**********************************/

int CSudokuBoardSamurai::GetBoardIndex()
{
	return 3;
}

void CSudokuBoardSamurai::GetMovedCursor(CPoint& ptId, int iMoveDir)
{
	int nBorder = myBoardDim-1;
	switch (iMoveDir)
	{
	case MOVE_DIR_RIGHT:
		ptId.x++;
		if (ptId.y < 6 || ptId.y > 14)
		{
			if (ptId.x > nBorder) 
			{
				ptId.x = 0;
			}
			else if (ptId.x > 8 && ptId.x < 12)
			{
				ptId.x = 12;
			}
		}
		else if (ptId.y > 8 && ptId.y < 12)
		{
			if (ptId.x > 14) ptId.x = 6;
		}
		else
		{
			if (ptId.x > nBorder) ptId.x = 0;
		}
		break;
	case MOVE_DIR_LEFT:
		ptId.x--;
		if (ptId.y < 6 || ptId.y > 14)
		{
			if (ptId.x < 0) 
			{
				ptId.x = nBorder;
			}
			else if (ptId.x > 8 && ptId.x < 12)
			{
				ptId.x = 8;
			}
		}
		else if (ptId.y > 8 && ptId.y < 12)
		{
			if (ptId.x < 6) ptId.x = 14;
		}
		else
		{
			if (ptId.x < 0) ptId.x = nBorder;
		}
		break;
	case MOVE_DIR_DOWN:
		ptId.y++;
		if (ptId.x < 6 || ptId.x > 14)
		{
			if (ptId.y > nBorder) 
			{
				ptId.y = 0;
			}
			else if (ptId.y > 8 && ptId.y < 12)
			{
				ptId.y = 12;
			}
		}
		else if (ptId.x > 8 && ptId.x < 12)
		{
			if (ptId.y > 14) ptId.y = 6;
		}
		else
		{
			if (ptId.y > nBorder) ptId.y = 0;
		}
		break;
	case MOVE_DIR_UP:
		ptId.y--;
		if (ptId.x < 6 || ptId.x > 14)
		{
			if (ptId.y < 0) 
			{
				ptId.y = nBorder;
			}
			else if (ptId.y > 8 && ptId.y < 12)
			{
				ptId.y = 8;
			}
		}
		else if (ptId.x > 8 && ptId.x < 12)
		{
			if (ptId.y < 6) ptId.y = 14;
		}
		else
		{
			if (ptId.y < 0) ptId.y = nBorder;
		}
		break;
	}
}
CSudokuField* CSudokuBoardSamurai::GetSudokuField(CPoint ptId)
{
    return GetDisplayField(ptId);
}

int CSudokuBoardSamurai::GetFieldNumber(CPoint ptId)
{
	CSudokuField* pField = NULL;

	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->m_pFieldEdit->myNumber;
	}
	return 0;
}

int CSudokuBoardSamurai::GetFieldNumberOld(CPoint ptId)
{
	CSudokuField* pField = NULL;

	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->m_pFieldEdit->myOldNumber;
	}
	return 0;
}

bool CSudokuBoardSamurai::IsNumberSelectedSingle(CPoint ptId, int nNumber)
{
	CSudokuField* pField;

    // is number a visible single candidate
	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->IsNumberSelectedSingle(nNumber);
	}
	return false;
}

bool CSudokuBoardSamurai::IsDuplicate(CPoint ptId, int nNumber)
{
	CPoint ptVect[4];
	bool bRet = false;
	bool bUnion = false;
    int  nHorMod = 3;

	VectorResult_t vr = BoardToVector(ptId, ptVect);
	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// find duplicate in row
			bRet |= CSudokuBoard::IsDuplicate(mySudokuRows[ptVect[0].y], ptVect[0].x, nNumber);

			// find duplicate in column
			bRet |= CSudokuBoard::IsDuplicate(mySudokuColumns[ptVect[1].x], ptVect[1].y, nNumber);

			// find duplicate in square
			int nIndex = (ptVect[0].y / nHorMod * nHorMod) + (ptVect[0].x / 3);
			int nOwnPos = (ptVect[0].y % nHorMod * 3) + (ptVect[0].x % 3); 
			bRet |= CSudokuBoard::IsDuplicate(mySudokuSquares[nIndex], nOwnPos, nNumber);
		}
		else
		{
			// find duplicate in row
			bRet |= CSudokuBoard::IsDuplicate(mySudokuRows[ptVect[2].y], ptVect[2].x, nNumber);

			// find duplicate in column
			bRet |= CSudokuBoard::IsDuplicate(mySudokuColumns[ptVect[3].x], ptVect[3].y, nNumber);

			// find duplicate in square
			int nIndex = (ptVect[2].y / nHorMod * nHorMod) + (ptVect[2].x / 3);
			int nOwnPos = (ptVect[2].y % nHorMod * 3) + (ptVect[2].x % 3); 
			bRet |= CSudokuBoard::IsDuplicate(mySudokuSquares[nIndex], nOwnPos, nNumber);
		}
		int iVr = (int)vr;
		iVr--;
		vr = (VectorResult_t)iVr;
	}

	return bRet;
}

bool CSudokuBoardSamurai::IsSingleInFields(int &nCount)
{
    int x, y;
	CSudokuField* pField = NULL;
	nCount = 0;
    UINT nID;

    mySingleCounter->Reset();
	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				if (pField->IsSingleInField())
				{
                    // get field position
                    nID = pField->m_pFieldEdit->m_nId;
                    // mark single in array
                    mySingleCounter->SetSingle(nID);
				}
			}
		}
	}
    nCount = mySingleCounter->GetSingleCount();
	return (nCount > 0);
}

bool CSudokuBoardSamurai::IsSingleOnBoard(int &nCount)
{
	nCount = 0;
    myCandCounter->Reset();
    // find all singles on board
	FindSingleOnBoard(mySudokuRows, 45);
	FindSingleOnBoard(mySudokuColumns, 45);
	FindSingleOnBoard(mySudokuSquares, 45);
	FindSingleOnBoard(mySudokuUnions, 4);
    // clear field singles
    myCandCounter->ClearSingleFields(mySingleCounter);
    // get remaining singles
    nCount = myCandCounter->GetSingleCount();
	return (nCount > 0);
}

void CSudokuBoardSamurai::FindSingleOnBoard(CObArray *pFieldArray, int nDim)
{
	int i, j, c;
	int iCount = 0;
	CSudokuField* pField = NULL;
	UINT nID;

	// Iterate Field arrays
	for (i=0; i<nDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<9; c++)
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

/***********************************
*    action and set functions      *
***********************************/

void CSudokuBoardSamurai::SetBoardIndex(int nIndex)
{
	myBoardIndex = nIndex;
}

void CSudokuBoardSamurai::SelectMulti(MultiSelect_t msMode)
{
    int x, y;
	CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->SelectMulti(msMode);
			}
		}
	}
}

void CSudokuBoardSamurai::SelectSingleInField(BOOL bSelect)
{
    int x, y;
	CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->SelectSingle(bSelect);
			}
		}
	}
}

void CSudokuBoardSamurai::SelectSingles(BOOL bSelect)
{
	SelectSingles(mySudokuRows, 45, bSelect);
	SelectSingles(mySudokuColumns, 45, bSelect);
	SelectSingles(mySudokuSquares, 45, bSelect);
	SelectSingles(mySudokuUnions, 4, bSelect);
}

bool CSudokuBoardSamurai::SolveLevel1()
{
    int x, y;
	CSudokuField* pField = NULL;
	bool bModified = false;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				bModified |= pField->SetSingleNumber();
			}
		}
	}
	return bModified;
}

bool CSudokuBoardSamurai::SolveLevel2()
{
	bool bModified = false;

	bModified |= SolveInArray(mySudokuRows, 45);
	bModified |= SolveInArray(mySudokuColumns, 45);
	bModified |= SolveInArray(mySudokuSquares, 45);
	bModified |= SolveInArray(mySudokuUnions, 4);

	return bModified;
}

void CSudokuBoardSamurai::SwitchEditMode(bool bEditMode)
{
    int x, y;
	CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->SetEditMode(bEditMode);
			}
		}
	}
}

void CSudokuBoardSamurai::SelectScan(int c, bool bSelect, BOOL bAllowMulti)
{
    int x, y;
	CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->SelectCandidateScan(c, bSelect, bAllowMulti);
			}
		}
	}
}

void CSudokuBoardSamurai::SetEditFocus(CPoint ptId)
{
	CSudokuField* pField = NULL;
	
	pField = GetDisplayField(ptId);
	if (pField)
	{
		pField->m_pFieldEdit->SetFocus();
	}
}

void CSudokuBoardSamurai::SetFieldNumber(CPoint ptId, int nNumber)
{
	CSudokuField* pField = NULL;
	
	pField = GetDisplayField(ptId);
	if (pField)
	{
		pField->m_pFieldEdit->myNumber = nNumber;
	}
}

void CSudokuBoardSamurai::DeleteCandidates(int xPos, int yPos, int nNumber)
{
	CPoint ptVect[4];
	bool bRet = false;
	bool bUnion = false;
    int  nHorMod = myBoardDim / 3;
	CPoint ptId(xPos, yPos);

	VectorResult_t vr = BoardToVector(ptId, ptVect);
	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// delete in row
			CSudokuBoard::DeleteCandidates(mySudokuRows[ptVect[0].y], nNumber);

			// delete in column
			CSudokuBoard::DeleteCandidates(mySudokuColumns[ptVect[0].x], nNumber);

			if (!bUnion)
			{
				// delete in square
				int nIndex = (ptVect[0].y / nHorMod * nHorMod) + (ptVect[0].x / 3);
				CSudokuBoard::DeleteCandidates(mySudokuSquares[nIndex], nNumber);
			}
		}
		else
		{
			// delete in row
			CSudokuBoard::DeleteCandidates(mySudokuRows[ptVect[2].y], nNumber);

			// delete in column
			CSudokuBoard::DeleteCandidates(mySudokuColumns[ptVect[2].x], nNumber);

			// delete in union
			int nOwnPos; 
			int nIndex = BoardToUnion(ptId, nOwnPos);
			CSudokuBoard::DeleteCandidates(mySudokuUnions[nIndex], nNumber);
		}
	}
}

// Schaltet die Kandidatenanzeige für alle Felder an oder aus
void CSudokuBoardSamurai::ShowCandidates(bool bShowCandidates)
{
    int x,y;
	CSudokuField* pField = NULL;

	for (x=0; x<myBoardDim; x++)
	{
		for (y=0; y<myBoardDim; y++)
		{
            pField = GetDisplayField(CPoint(x,y));
			if (pField)
			{
				pField->ShowCandidates(bShowCandidates);
			}
		}
	}
	myParent->Invalidate();
	myParent->UpdateWindow();
}

void CSudokuBoardSamurai::EvalMousePosition(CPoint ptPos)
{
	CSudokuField* pField = NULL;

	int iHoriPos = (ptPos.x - myHoriIndent) / myFieldExt;
	int iVertPos = (ptPos.y - myVertIndent) / myFieldExt;

	// check if mouse is over sudoku rectangle
	if ((iHoriPos > (myBoardDim-1) || iVertPos > (myBoardDim-1)) ||
		(ptPos.x < myHoriIndent || ptPos.y < myVertIndent))
	{
		pField = GetDisplayField(CPoint(m_iLastHoriPos, m_iLastVertPos));
		if (pField) 
		{
			pField->CleanCandHover();
			pField->ShowHideEditField(true);
		}
		m_iLastHoriPos = 0;
		m_iLastVertPos = 0;
		return;
	}

	if (iHoriPos != m_iLastHoriPos || iVertPos != m_iLastVertPos)
	{
		pField = GetDisplayField(CPoint(m_iLastHoriPos, m_iLastVertPos));
		if (pField) 
		{
			pField->CleanCandHover();
			pField->ShowHideEditField(true);
		}
		pField = GetDisplayField(CPoint(iHoriPos, iVertPos));
		if (pField) pField->ShowHideEditField(false);
		m_iLastHoriPos = iHoriPos;
		m_iLastVertPos = iVertPos;
	}

	pField = GetDisplayField(CPoint(iHoriPos, iVertPos));
	if (pField) pField->EvalMousePosition(ptPos);
}

bool CSudokuBoardSamurai::ToggleCandidate(CPoint ptPos, int& iHoriPos, int& iVertPos, int& nNumber)
{
	iHoriPos = (ptPos.x - myHoriIndent) / myFieldExt;
	iVertPos = (ptPos.y - myVertIndent) / myFieldExt;

	// check if mouse is over sudoku board
	if (iHoriPos > (myBoardDim-1) || iVertPos > (myBoardDim-1)) return false;

	CSudokuField* pField = GetDisplayField(CPoint(iHoriPos, iVertPos));
	if (pField) pField->ToggleCandidate(ptPos, nNumber);

	return true;
}

void CSudokuBoardSamurai::ToggleCandidate(CPoint ptId, int nNumber)
{
	CSudokuField* pField = GetSudokuField(ptId);
	if (pField) pField->ToggleCandidate(nNumber);
}

void CSudokuBoardSamurai::MoveBoard(MoveDirection_t moveDir, int nMoveValue)
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
            pField = GetDisplayField(CPoint(x,y));
			if (pField) pField->MoveSudokuField(ptPos);
		}
	}
	myParent->Invalidate();
	myParent->UpdateWindow();
}


void CSudokuBoardSamurai::Serialize(CArchive& ar)
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
		ar  << myBoardDim
			<< myHoriIndentDP
			<< myVertIndent
			<< myVertIndentDP
			<< myFieldExt
			<< myFieldExtDP
			<< myHoriIndent;
	}
	else
	{
		ar  >> myBoardDim
			>> myHoriIndentDP
			>> myVertIndent
			>> myVertIndentDP
			>> myFieldExt
			>> myFieldExtDP;
        if (nSchema > 1)
        {
            ar >> myHoriIndent;
        }
	}

    if (ar.IsLoading())
    {
        FinalConstruction();
    }

	int nCount = 0;
	bool bUnion = false;
	CPoint ptVect[4];
	for (int y=0; y<myBoardDim; y++)
	{
		for (int x=0; x<myBoardDim; x++)
		{
			VectorResult_t vr = BoardToVector(CPoint(x,y), ptVect);
			while (vr > vrInvalid)
			{
				if (vr == vrValidSingle)
				{
					if (!bUnion)
					{
						if (ar.IsStoring())
						{
							pField = (CSudokuField*)mySudokuRows[ptVect[0].y].GetAt(ptVect[0].x);
							ar << pField;
							nCount++;
						}
						else
						{
							ar >> pField;
							AddSudokuField(pField, x, y);
							nCount++;
						}
					}
					else {bUnion = false;}
				}
				else
				{
					if (ar.IsStoring())
					{
						pField = (CSudokuField*)mySudokuRows[ptVect[2].y].GetAt(ptVect[2].x);
						ar << pField;
						nCount++;
					}
					else
					{
						ar >> pField;
						AddSudokuField(pField, x, y);
						nCount++;
					}
					bUnion = true;
				}
				int iVr = (int)vr;
				iVr--;
				vr = (VectorResult_t)iVr;
			}
		}
	}
	TRACE1("Ich habe %d Felder gespeichert bzw. geladen\n", nCount);
}

void CSudokuBoardSamurai::DrawSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;
	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 2, RGB(0,0,0));
    CBrush* pOldBrush;
    CBrush aBkBrush(m_backColor);
	CPoint ptsBorder[25];
	DWORD  counts[5];
	int i;
    int nModHor =3;

	// draw rectangles
    pOldBrush = pDC->SelectObject(&aBkBrush);
	pOldPen = pDC->SelectObject(&aBoldPen);
	ptsBorder[0].x = myHoriIndent;
	ptsBorder[0].y = myVertIndent;
	ptsBorder[1].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[1].y = myVertIndent;
	ptsBorder[2].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[2].y = myFieldExt*9 + myVertIndent;
	ptsBorder[3].x = myHoriIndent;
	ptsBorder[3].y = myFieldExt*9 + myVertIndent;
	ptsBorder[4].x = myHoriIndent;
	ptsBorder[4].y = myVertIndent;
	counts[0] = 5;
	ptsBorder[5].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[5].y = myVertIndent;
	ptsBorder[6].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[6].y = myVertIndent;
	ptsBorder[7].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[7].y = myFieldExt*9 + myVertIndent;
	ptsBorder[8].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[8].y = myFieldExt*9 + myVertIndent;
	ptsBorder[9].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[9].y = myVertIndent;
	counts[1] = 5;
	ptsBorder[10].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[10].y = myVertIndent + 6*myFieldExt;
	ptsBorder[11].x = myFieldExt*15 + myHoriIndent;
	ptsBorder[11].y = myVertIndent + 6*myFieldExt;
	ptsBorder[12].x = myFieldExt*15 + myHoriIndent;
	ptsBorder[12].y = myFieldExt*15 + myVertIndent;
	ptsBorder[13].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[13].y = myFieldExt*15 + myVertIndent;
	ptsBorder[14].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[14].y = myVertIndent + 6*myFieldExt;
	counts[2] = 5;
	ptsBorder[15].x = myHoriIndent;
	ptsBorder[15].y = myVertIndent + 12*myFieldExt;
	ptsBorder[16].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[16].y = myVertIndent + 12*myFieldExt;
	ptsBorder[17].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[17].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[18].x = myHoriIndent;
	ptsBorder[18].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[19].x = myHoriIndent;
	ptsBorder[19].y = myVertIndent + 12*myFieldExt;
	counts[3] = 5;
	ptsBorder[20].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[20].y = myVertIndent + 12*myFieldExt;
	ptsBorder[21].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[21].y = myVertIndent + 12*myFieldExt;
	ptsBorder[22].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[22].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[23].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[23].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[24].x = myHoriIndent + 12*myFieldExt;
	ptsBorder[24].y = myVertIndent + 12*myFieldExt;
	counts[4] = 5;
    pDC->PolyPolyline(ptsBorder, counts, 5);

	pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
	// draw horizontal lines
	int y1 = myFieldExt + myVertIndent;
	int x1 = myFieldExt * 9 + myHoriIndent;
	int x2 = myFieldExt * myBoardDim + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndent, y1);
		pDC->LineTo(x1,y1);
		pDC->MoveTo(x1 + 3*myFieldExt, y1);
		pDC->LineTo(x2,y1);
		y1 += myFieldExt;
	}
	y1 = 7*myFieldExt + myVertIndent;
	x1 = myFieldExt * 15 + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndent + 6 * myFieldExt, y1);
		pDC->LineTo(x1,y1);
		y1 += myFieldExt;
	}
	y1 = 13*myFieldExt + myVertIndent;
	x1 = myFieldExt * 9 + myHoriIndent;
	x2 = myFieldExt * myBoardDim + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndent, y1);
		pDC->LineTo(x1,y1);
		pDC->MoveTo(myHoriIndent + 12 * myFieldExt, y1);
		pDC->LineTo(x2,y1);
		y1 += myFieldExt;
	}
	// draw vertical lines
	y1 = myFieldExt * 9 + myVertIndent;
	int y2 = myFieldExt * myBoardDim + myVertIndent;
	x1 = myFieldExt + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, myVertIndent);
		pDC->LineTo(x1, y1);
		pDC->MoveTo(x1, y1 + 3*myFieldExt);
		pDC->LineTo(x1, y2);
		x1 += myFieldExt;
	}
	y1 = myFieldExt * 15 + myVertIndent;
	x1 = 7*myFieldExt + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, myVertIndent + 6 * myFieldExt);
		pDC->LineTo(x1,y1);
		x1 += myFieldExt;
	}
	y1 = myFieldExt * 9 + myVertIndent;
	y2 = myFieldExt * myBoardDim + myVertIndent;
	x1 = 13 * myFieldExt + myHoriIndent;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, myVertIndent);
		pDC->LineTo(x1, y1);
		pDC->MoveTo(x1, y1 + 3*myFieldExt);
		pDC->LineTo(x1, y2);
		x1 += myFieldExt;
	}
	for (int i=0; i<myBoardDim; i++)
	{
		for (int j=0; j<myBoardDim; j++)
		{
			pField = GetDisplayField(CPoint(j, i));
			if (pField) pField->DrawCandidates(pDC);
		}
	}
}

void CSudokuBoardSamurai::PrintSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;

	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 60, RGB(0,0,0));
	CPen   aThinPen(PS_SOLID, 30, RGB(0,0,0));
	CPoint ptsBorder[25];
	DWORD  counts[5];
	int i;
    int nModHor = 3;

	// draw rectangles
	pOldPen = pDC->SelectObject(&aBoldPen);
	ptsBorder[0].x = myHoriIndentDP;
	ptsBorder[0].y = -myVertIndentDP;
	ptsBorder[1].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[1].y = -myVertIndentDP;
	ptsBorder[2].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[2].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[3].x = myHoriIndentDP;
	ptsBorder[3].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[4].x = myHoriIndentDP;
	ptsBorder[4].y = -myVertIndentDP;
	counts[0] = 5;
	ptsBorder[5].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[5].y = -myVertIndentDP;
	ptsBorder[6].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[6].y = -myVertIndentDP;
	ptsBorder[7].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[7].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[8].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[8].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[9].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[9].y = -myVertIndentDP;
	counts[1] = 5;
	ptsBorder[10].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[10].y = -myVertIndentDP - 6*myFieldExtDP;
	ptsBorder[11].x = myFieldExtDP*15 + myHoriIndentDP;
	ptsBorder[11].y = -myVertIndentDP - 6*myFieldExtDP;
	ptsBorder[12].x = myFieldExtDP*15 + myHoriIndentDP;
	ptsBorder[12].y = -myFieldExtDP*15 - myVertIndentDP;
	ptsBorder[13].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[13].y = -myFieldExtDP*15 - myVertIndentDP;
	ptsBorder[14].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[14].y = -myVertIndentDP - 6*myFieldExtDP;
	counts[2] = 5;
	ptsBorder[15].x = myHoriIndentDP;
	ptsBorder[15].y = -myVertIndentDP - 12*myFieldExtDP;
	ptsBorder[16].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[16].y = -myVertIndentDP - 12*myFieldExtDP;
	ptsBorder[17].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[17].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[18].x = myHoriIndentDP;
	ptsBorder[18].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[19].x = myHoriIndentDP;
	ptsBorder[19].y = -myVertIndentDP - 12*myFieldExtDP;
	counts[3] = 5;
	ptsBorder[20].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[20].y = -myVertIndentDP - 12*myFieldExtDP;
	ptsBorder[21].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[21].y = -myVertIndentDP - 12*myFieldExtDP;
	ptsBorder[22].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[22].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[23].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[23].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[24].x = myHoriIndentDP + 12*myFieldExtDP;
	ptsBorder[24].y = -myVertIndentDP - 12*myFieldExtDP;
	counts[4] = 5;
    pDC->PolyPolyline(ptsBorder, counts, 5);

	pDC->SelectObject(pOldPen);
	// draw horizontal lines
	int y1 = -myFieldExtDP - myVertIndentDP;
	int x1 = myFieldExtDP * 9 + myHoriIndentDP;
	int x2 = myFieldExtDP * myBoardDim + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndentDP, y1);
		pDC->LineTo(x1,y1);
		pDC->MoveTo(x1 + 3*myFieldExtDP, y1);
		pDC->LineTo(x2,y1);
		y1 -= myFieldExtDP;
	}
	y1 = -7*myFieldExtDP - myVertIndentDP;
	x1 = myFieldExtDP * 15 + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndentDP + 6 * myFieldExtDP, y1);
		pDC->LineTo(x1,y1);
		y1 -= myFieldExtDP;
	}
	y1 = -13*myFieldExtDP - myVertIndentDP;
	x1 = myFieldExtDP * 9 + myHoriIndentDP;
	x2 = myFieldExtDP * myBoardDim + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(myHoriIndentDP, y1);
		pDC->LineTo(x1,y1);
		pDC->MoveTo(myHoriIndentDP + 12 * myFieldExtDP, y1);
		pDC->LineTo(x2,y1);
		y1 -= myFieldExtDP;
	}
	// draw vertical lines
	y1 = -myFieldExtDP * 9 - myVertIndentDP;
	int y2 = -myFieldExtDP * myBoardDim - myVertIndentDP;
	x1 = myFieldExtDP + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, -myVertIndentDP);
		pDC->LineTo(x1, y1);
		pDC->MoveTo(x1, y1 - 3*myFieldExtDP);
		pDC->LineTo(x1, y2);
		x1 += myFieldExtDP;
	}
	y1 = -myFieldExtDP * 15 - myVertIndentDP;
	x1 = 7*myFieldExtDP + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, -myVertIndentDP - 6 * myFieldExtDP);
		pDC->LineTo(x1,y1);
		x1 += myFieldExtDP;
	}
	y1 = -myFieldExtDP * 9 - myVertIndentDP;
	y2 = -myFieldExtDP * myBoardDim - myVertIndentDP;
	x1 = 13 * myFieldExtDP + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(pOldPen);
		}
		pDC->MoveTo(x1, -myVertIndentDP);
		pDC->LineTo(x1, y1);
		pDC->MoveTo(x1, y1 - 3*myFieldExtDP);
		pDC->LineTo(x1, y2);
		x1 += myFieldExtDP;
	}

	// draw canditates in all fields
	for (int i=0; i<myBoardDim; i++)
	{
		for (int j=0; j<myBoardDim; j++)
		{
			pField = GetDisplayField(CPoint(j, i));
			if (pField) pField->PrintSudokuField(pDC);
		}
	}
}

/***********************************
*   internal helper functions      *
***********************************/

void CSudokuBoardSamurai::FinalConstruction()
{
    myCandCounts    = new int[9];
    arrDisable      = new BOOL[9];
    mySudokuSquares = new CObArray[45];
    mySudokuColumns = new CObArray[45];
    mySudokuRows    = new CObArray[45];
	mySudokuUnions  = new CObArray[4];
    myCandCounter   = new CCandCounter(myBoardDim);
    mySingleCounter = new CCandCounter(myBoardDim);
	m_iLastVertPos	= 0;
	m_iLastHoriPos	= 0;
	myFieldExt = 40;
	myFieldExtDP = 800;
	myCandLen  = 6;
	myCandLenDP = 100;
    myFontSizeDP = 500;
    for (int i=0; i<9; i++)
    {
        arrDisable[i] = TRUE;
        myCandCounts[i] = 0;
    }
    m_bConstructed = true;
}

bool CSudokuBoardSamurai::AddSudokuField(CSudokuField* pField, int iHoriPos, int iVertPos)
{
	bool bRet = false;
	CPoint ptVect[4];
	VectorResult_t vr = BoardToVector(CPoint(iHoriPos, iVertPos), ptVect);

	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// fill surrounding 9 x 9 field
			// add field to row
			mySudokuRows[ptVect[0].y].Add(pField);
			// add field to column
			mySudokuColumns[ptVect[1].x].Add(pField);
			// add field to square
			int iIndex = (ptVect[0].x/3) + ((ptVect[0].y/3)*3);
			mySudokuSquares[iIndex].Add(pField);
			bRet = true;
		}
		else
		{
			// fill middle 9 x 9 field
			// add field to row
			mySudokuRows[ptVect[2].y].Add(pField);
			// add field to column
			mySudokuColumns[ptVect[3].x].Add(pField);
			// add field to square
			int iIndex = (ptVect[2].x/3) + (((ptVect[2].y-18)/3)*3) + 18;
			mySudokuSquares[iIndex].Add(pField);
			// fill the common field in the edge of the middle
			// add field to union
			int nOwnPos; 
			int nIndex = BoardToUnion(CPoint(iHoriPos, iVertPos), nOwnPos);
			mySudokuUnions[nIndex].Add(pField);
			ASSERT(nOwnPos == mySudokuUnions[nIndex].GetUpperBound());
		}
		int iVr = (int)vr;
		iVr--;
		vr = (VectorResult_t)iVr;
	}
	return bRet;
}

void CSudokuBoardSamurai::DeleteSudokuFields()
{
	CSudokuField* pField;
    int y, x;
	int nCount = 0;
	int nSize = 0;

    if (!m_bConstructed) return;

    for (y=0; y<45; y++)
    {
        for (x=0; x<9; x++)
        {
			nSize = mySudokuRows[y].GetSize();
            if (nSize)
            {
				if ((y<18 || y>26) ||
					(y>17 && y<21 && x>2 && x<6) ||
					(y>20 && y<24) ||
					(y>23 && y<27 && x>2 && x<6))
				{
					pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
					delete pField;
					nCount++;
				}
            }
			else
			{
				TRACE1("Diese Zeile ist leer: %d\n", y);
			}
        }
        mySudokuRows[y].RemoveAll();
        mySudokuColumns[y].RemoveAll();
        mySudokuSquares[y].RemoveAll();
    }
	for (x=0; x<4; x++)
		mySudokuUnions[x].RemoveAll();
	TRACE1("Ich habe %d Felder gelöscht\n", nCount);
}

BOOL CSudokuBoardSamurai::CheckNumberCompletion(int nNumber)
{
	CSudokuField* pField;
    int i, j;
    int nCount = 0;

    for (i=0; i<21; i++)
    {
        for (j=0; j<21; j++)
        {
            pField = GetDisplayField(CPoint(j,i));
            if (pField && pField->m_pFieldEdit->myNumber == nNumber) nCount++;
        }
    }

    myCandCounts[nNumber-1] = nCount;

    return (nCount == 41 ? FALSE : TRUE);
}

/***********************************
*    Helper Functions              *
***********************************/

VectorResult_t CSudokuBoardSamurai::BoardToVector(CPoint ptBoard , CPoint* ptVect)
{
	VectorResult_t vectorResult  = vrValidSingle;
	int x = ptBoard.x,
		y = ptBoard.y;

	ptVect[0] = CPoint(-1,-1);
	ptVect[1] = CPoint(-1,-1);
	ptVect[2] = CPoint(-1,-1);
	ptVect[3] = CPoint(-1,-1);

	if ((x<9 && y<6) || (x<6 && y>5 && y<9))
	{
		// upper left single
		ptVect[0] = CPoint(x,y);
		ptVect[1] = CPoint(x,y);
	}
	else if ((x>11 && y<6) || (x>14 && y>5 && y<9))
	{
		// upper right single
		ptVect[0] = CPoint(x-12,y+9);
		ptVect[1] = CPoint(x-3,y);
	}
	else if ((x>8 && x<12 && y>5 && y<9) || 
		     (x>5 && x<15 && y>8 && y<12) ||
			 (x>8 && x<12 && y>11 && y<15))
	{
		// middle single
		ptVect[0] = CPoint(x-6,y+12);
		ptVect[1] = CPoint(x+12,y-6);
	}
	else if ((x<9 && y>14) || (x<6 && y>11 && y<15))
	{
		// lower left single
		ptVect[0] = CPoint(x,y+15);
		ptVect[1] = CPoint(x+27,y-12);
	}
	else if ((x>11 && y>14) || (x>14 && y>11 && y<15))
	{
		// lower right single
		ptVect[0] = CPoint(x-12,y+24);
		ptVect[1] = CPoint(x+24,y-12);
	}
	else if ((x>5) && (x<9) && (y>5) && (y<9))
	{
		// upper left union
		ptVect[0] = CPoint(x,y);
		ptVect[1] = CPoint(x,y);
		ptVect[2] = CPoint(x-6,y+12);
		ptVect[3] = CPoint(x+12,y-6);
		vectorResult = vrValidDouble;
	}
	else if ((x>11) && (x<15) && (y>5) && (y<9))
	{
		// upper right union
		ptVect[0] = CPoint(x-12,y+9);
		ptVect[1] = CPoint(x-3,y);
		ptVect[2] = CPoint(x-6,y+12);
		ptVect[3] = CPoint(x+12,y-6);
		vectorResult = vrValidDouble;
	}
	else if ((x>5) && (x<9) && (y>11) && (y<15))
	{
		// lower left union
		ptVect[0] = CPoint(x,y+15);
		ptVect[1] = CPoint(x+27,y-12);
		ptVect[2] = CPoint(x-6,y+12);
		ptVect[3] = CPoint(x+12,y-6);
		vectorResult = vrValidDouble;
	}
	else if ((x>11) && (x<15) && (y>11) && (y<15))
	{
		// lower right union
		ptVect[0] = CPoint(x-12,y+24);
		ptVect[1] = CPoint(x+24,y-12);
		ptVect[2] = CPoint(x-6,y+12);
		ptVect[3] = CPoint(x+12,y-6);
		vectorResult = vrValidDouble;
	}
	else
	{
		vectorResult = vrInvalid;
	}
	return vectorResult;
}

int CSudokuBoardSamurai::BoardToUnion(CPoint ptBoard, int& nOwnPos)
{
	int nIndex = -1;

	nOwnPos = ptBoard.y%3 * 3 + ptBoard.x%3;

	if (ptBoard.y<9)
	{
		nIndex = (ptBoard.x<9 ? 0 : 1);
	}
	else
	{
		nIndex = (ptBoard.x<9 ? 2 : 3);
	}
	return nIndex;
}

bool CSudokuBoardSamurai::IsValidField(CPoint ptBoard)
{
	int x = ptBoard.x,
		y = ptBoard.y;

	if ((x>8  && x<12 && y<6 ) ||
		(x<6  && y>8  && y<12) ||
		(x>14 && y>8  && y<12) ||
		(x>8  && x<12 && y>14)) return false;

	return true;
}

CSudokuField* CSudokuBoardSamurai::GetDisplayField(CPoint ptBoard)
{
	CPoint ptVect[4];
	CSudokuField* pField = NULL;
	
	VectorResult_t vr = BoardToVector(ptBoard, ptVect);
	if (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// get a single field from upper or lower 9 x 9
			pField = (CSudokuField*)mySudokuRows[ptVect[0].y].GetAt(ptVect[0].x);
		}
		else
		{
			// return a field from union square
			int nOwnPos;
			int nIndex = BoardToUnion(ptBoard, nOwnPos);
			pField = (CSudokuField*)mySudokuUnions[nIndex].GetAt(nOwnPos);
		}
	}
	return pField;
}

void CSudokuBoardSamurai::SelectSingles(CObArray* pFieldArray, int nDim, BOOL bSelect)
{
	int i, j, c;
	int iCount = 0;
	int iSingleIndex;
	CSudokuField* pField = NULL;

	// Iterate Field arrays
	for (i=0; i<nDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<9; c++)
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

bool CSudokuBoardSamurai::SolveInArray(CObArray* pFieldArray, int nDim)
{
	int i, j, c;
	int iCount = 0;
	int iSingleIndex;
	CSudokuField* pField = NULL;
	bool bModified = false;


	// Iterate Field arrays
	for (i=0; i<nDim; i++)
	{
		// Iterate Fields
        int nSize = (int)pFieldArray[i].GetSize();
		for (c=0; c<9; c++)
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
				pField->SetNumber(c+1);
				bModified |= true;
			}
			iCount = 0;
		}
	}
	return bModified;
}

