#include "StdAfx.h"
#include "SudokuMDI.h"
#include ".\sudokuboardduo.h"

// CSudokuBoardDuo

IMPLEMENT_SERIAL(CSudokuBoardDuo, CObject, VERSIONABLE_SCHEMA | 2)
CSudokuBoardDuo::CSudokuBoardDuo(void)
{
}

CSudokuBoardDuo::CSudokuBoardDuo(int nBoardDim)
{
    myBoardDim = nBoardDim;
	myFieldExt = 40;
	myCandLen  = 6;
	myFieldExtDP = 1200;
	myCandLenDP = 180;
    myFontSizeDP = 700;
	FinalConstruction();
}

CSudokuBoardDuo::~CSudokuBoardDuo(void)
{
    DeleteSudokuFields();
    delete [] myCandCounts;
    delete [] arrDisable;
    delete [] mySudokuRows;
    delete [] mySudokuColumns;
    delete [] mySudokuSquares;
	delete myCandCounter;
	delete mySingleCounter;
	m_bConstructed = false;
}

/***********************************
*    Initialization Functions      *
***********************************/

void CSudokuBoardDuo::CreateEmptyBoard()
{
	bool bValidField = false;
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
			if ((x > 5 && y < 9) || (x < 9 && y > 5))
			{
				ptPos.x = myHoriIndent + x * myFieldExt;
				ptPosDP.x = myHoriIndentDP + x * myFieldExtDP;
				pField = new CSudokuField(ptPos, ptPosDP, 
										9, 
										myFieldExt, myFieldExtDP,
										myCandLen, myCandLenDP, myFontSizeDP);
				nCount++;
			}
			bValidField = AddSudokuField(pField, x, y);
		}
	}
	TRACE1("Ich habe %d Felder erzeugt\n", nCount);
}

void CSudokuBoardDuo::WipeBoard()
{
	CSudokuField* pField;
	int x, y;

    for (x=0; x<9; x++)
    {
        for (y=0; y<18; y++)
        {
			pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
			pField->WipeField();
        }
    }
}

void CSudokuBoardDuo::EnableBoard(bool bEnable)
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

void CSudokuBoardDuo::CleanHover()
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

void CSudokuBoardDuo::RefreshCandidates()
{
	int x, y, c;
	CPoint ptVect1, ptVect2, ptVect3;
	CSudokuField* pField;

	for (y=0; y<myBoardDim; y++)
	{
		for (x=0; x<myBoardDim; x++)
		{
			VectorResult_t vr = BoardToVector(CPoint(x,y), ptVect1, ptVect2, ptVect3);
			while (vr > vrInvalid)
			{
				if (vr == vrValidSingle)
				{
					pField = (CSudokuField*)mySudokuRows[ptVect1.y].GetAt(ptVect1.x);
				}
				else
				{
					pField = (CSudokuField*)mySudokuRows[ptVect2.y].GetAt(ptVect2.x);
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

void CSudokuBoardDuo::UpdateNumberCompletion()
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

void CSudokuBoardDuo::ResetCandidates()
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

void CSudokuBoardDuo::InitSudokuField(CWnd* pParent)
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

void CSudokuBoardDuo::UpdateBoardLayout()
{
	bool bValidField = false;
	CPoint ptPos, ptPosDP;
	CPoint ptId;
	int x, y;
    CSudokuField* pField = NULL;

	for (y=0; y<myBoardDim; y++)
	{
		ptPos.y = myVertIndent + y * myFieldExt;
		ptPosDP.y = -(myVertIndentDP + y * myFieldExtDP);
		for ( x=0; x<myBoardDim; x++)
		{
			if ((x > 5 && y < 9) || (x < 9 && y > 5))
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

int CSudokuBoardDuo::GetBoardIndex()
{
	return 2;
}

void CSudokuBoardDuo::GetMovedCursor(CPoint& ptId, int iMoveDir)
{
	int nBorder = myBoardDim-1;
	switch (iMoveDir)
	{
	case MOVE_DIR_RIGHT:
	case MOVE_DIR_LEFT:
		switch (iMoveDir)
		{
		case MOVE_DIR_RIGHT:
			ptId.x++;
			if (ptId.y < 6)
			{
				if (ptId.x > nBorder) ptId.x = 6;
			}
			else if (ptId.y > 8)
			{
				if (ptId.x > 8) ptId.x = 0;
			}
			else
			{
				if (ptId.x > nBorder) ptId.x = 0;
			}
			break;
		case MOVE_DIR_LEFT:
			ptId.x--;
			if (ptId.y < 6)
			{
				if (ptId.x < 0) ptId.x = nBorder;
			}
			else if (ptId.y > 8)
			{
				if (ptId.x < 0) ptId.x = 8;
			}
			else
			{
				if (ptId.x < 0) ptId.x = nBorder;
			}
			break;
		}
		break;
	case MOVE_DIR_DOWN:
	case MOVE_DIR_UP:
		switch (iMoveDir)
		{
		case MOVE_DIR_DOWN:
			ptId.y++;
			if (ptId.x < 6)
			{
				if (ptId.y > nBorder) ptId.y = 6;
			}
			else if (ptId.x > 8)
			{
				if (ptId.y > 8) ptId.y = 0;
			}
			else
			{
				if (ptId.y > nBorder) ptId.y = 0;
			}
			break;
		case MOVE_DIR_UP:
			ptId.y--;
			if (ptId.x < 6)
			{
				if (ptId.y < 6) ptId.y = nBorder;
			}
			else if (ptId.x > 8)
			{
				if (ptId.y < 0) ptId.y = 8;
			}
			else
			{
				if (ptId.y < 0) ptId.y = nBorder;
			}
			break;
		}
		break;
	}
}
CSudokuField* CSudokuBoardDuo::GetSudokuField(CPoint ptId)
{
    return GetDisplayField(ptId);
}

int CSudokuBoardDuo::GetFieldNumber(CPoint ptId)
{
	CSudokuField* pField = NULL;

	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->m_pFieldEdit->myNumber;
	}
	return 0;
}

int CSudokuBoardDuo::GetFieldNumberOld(CPoint ptId)
{
	CSudokuField* pField = NULL;

	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->m_pFieldEdit->myOldNumber;
	}
	return 0;
}

bool CSudokuBoardDuo::IsNumberSelectedSingle(CPoint ptId, int nNumber)
{
	CPoint ptVect1, ptVect2;
	CSudokuField* pField;

    // is number a visible single candidate
	pField = GetDisplayField(ptId);
	if (pField)
	{
		return pField->IsNumberSelectedSingle(nNumber);
	}
	return false;
}

bool CSudokuBoardDuo::IsDuplicate(CPoint ptId, int nNumber)
{
	CPoint ptVect1, ptVect2, ptVect3;
	bool bRet = false;
	bool bUnion = false;
    int  nHorMod = 3;

	VectorResult_t vr = BoardToVector(ptId, ptVect1, ptVect2, ptVect3);
	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// find duplicate in row
			bRet |= CSudokuBoard::IsDuplicate(mySudokuRows[ptVect1.y], ptVect1.x, nNumber);

			// find duplicate in column
			if (ptVect3.x < 0)
			{
				bRet |= CSudokuBoard::IsDuplicate(mySudokuColumns[ptVect1.x], ptVect1.y, nNumber);
			}
			else
			{
				bRet |= CSudokuBoard::IsDuplicate(mySudokuColumns[ptVect3.x], ptVect3.y, nNumber);
			}

			if (!bUnion)
			{
				// find duplicate in square
				int nIndex = (ptVect1.y / nHorMod * nHorMod) + (ptVect1.x / 3);
				int nOwnPos = (ptVect1.y % nHorMod * 3) + (ptVect1.x % 3); 
				bRet |= CSudokuBoard::IsDuplicate(mySudokuSquares[nIndex], nOwnPos, nNumber);
			}
		}
		else
		{
			bUnion = true;
			// find duplicate in row
			bRet |= CSudokuBoard::IsDuplicate(mySudokuRows[ptVect2.y], ptVect2.x, nNumber);

			// find duplicate in column
			bRet |= CSudokuBoard::IsDuplicate(mySudokuColumns[ptVect3.x], ptVect3.y, nNumber);
			ptVect3 = CPoint(-1,-1);

			// find duplicate in union
			int nIndex = (ptVect1.y-6) * 3 + ptVect1.x;
			bRet |= CSudokuBoard::IsDuplicate(mySudokuUnion, nIndex, nNumber);
		}
		int iVr = (int)vr;
		iVr--;
		vr = (VectorResult_t)iVr;
	}

	return bRet;
}

bool CSudokuBoardDuo::IsSingleInFields(int &nCount)
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

bool CSudokuBoardDuo::IsSingleOnBoard(int &nCount)
{
	nCount = 0;
    myCandCounter->Reset();
    // find all singles on board
	FindSingleOnBoard(mySudokuRows, 18);
	FindSingleOnBoard(mySudokuColumns, 18);
	FindSingleOnBoard(mySudokuSquares, 18);
	FindSingleOnBoard(&mySudokuUnion, 1);
    // clear field singles
    myCandCounter->ClearSingleFields(mySingleCounter);
    // get remaining singles
    nCount = myCandCounter->GetSingleCount();
	return (nCount > 0);
}

void CSudokuBoardDuo::FindSingleOnBoard(CObArray *pFieldArray, int nDim)
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

void CSudokuBoardDuo::SetBoardIndex(int nIndex)
{
	myBoardIndex = nIndex;
}

void CSudokuBoardDuo::SelectMulti(MultiSelect_t msMode)
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

void CSudokuBoardDuo::SelectSingleInField(BOOL bSelect)
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

void CSudokuBoardDuo::SelectSingles(BOOL bSelect)
{
	SelectSingles(mySudokuRows, 18, bSelect);
	SelectSingles(mySudokuColumns, 18, bSelect);
	SelectSingles(mySudokuSquares, 18, bSelect);
	SelectSingles(&mySudokuUnion, 1, bSelect);
}

bool CSudokuBoardDuo::SolveLevel1()
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

bool CSudokuBoardDuo::SolveLevel2()
{
	bool bModified = false;

	bModified |= SolveInArray(mySudokuRows, 18);
	bModified |= SolveInArray(mySudokuColumns, 18);
	bModified |= SolveInArray(mySudokuSquares, 18);
	bModified |= SolveInArray(&mySudokuUnion, 1);

	return bModified;
}

void CSudokuBoardDuo::SwitchEditMode(bool bEditMode)
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

void CSudokuBoardDuo::SelectScan(int c, bool bSelect, BOOL bAllowMulti)
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

void CSudokuBoardDuo::SetEditFocus(CPoint ptId)
{
	CSudokuField* pField = NULL;
	
	pField = GetDisplayField(ptId);
	if (pField)
	{
		pField->m_pFieldEdit->SetFocus();
	}
}

void CSudokuBoardDuo::SetFieldNumber(CPoint ptId, int nNumber)
{
	CSudokuField* pField = NULL;
	
	pField = GetDisplayField(ptId);
	if (pField)
	{
		pField->m_pFieldEdit->myNumber = nNumber;
	}
}

void CSudokuBoardDuo::DeleteCandidates(int xPos, int yPos, int nNumber)
{
	CPoint ptVect1, ptVect2, ptVect3;
	bool bRet = false;
	bool bUnion = false;
    int  nHorMod = myBoardDim / 3;
	CPoint ptId(xPos, yPos);

	VectorResult_t vr = BoardToVector(ptId, ptVect1, ptVect2, ptVect3);
	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// delete in row
			CSudokuBoard::DeleteCandidates(mySudokuRows[ptVect1.y], nNumber);

			// delete in column
			CSudokuBoard::DeleteCandidates(mySudokuColumns[ptVect1.x], nNumber);

			if (!bUnion)
			{
				// delete in square
				int nIndex = (ptVect1.y / nHorMod * nHorMod) + (ptVect1.x / 3);
				CSudokuBoard::DeleteCandidates(mySudokuSquares[nIndex], nNumber);
			}
		}
		else
		{
			// delete in row
			CSudokuBoard::DeleteCandidates(mySudokuRows[ptVect2.y], nNumber);

			// delete in column
			CSudokuBoard::DeleteCandidates(mySudokuColumns[ptVect2.x], nNumber);

			// delete in union
			int nIndex = ptVect2.y * 3 + ptVect2.x;
			CSudokuBoard::DeleteCandidates(mySudokuUnion, nNumber);
		}
	}
}

// Schaltet die Kandidatenanzeige für alle Felder an oder aus
void CSudokuBoardDuo::ShowCandidates(bool bShowCandidates)
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

void CSudokuBoardDuo::EvalMousePosition(CPoint ptPos)
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

bool CSudokuBoardDuo::ToggleCandidate(CPoint ptPos, int& iHoriPos, int& iVertPos, int& nNumber)
{
	iHoriPos = (ptPos.x - myHoriIndent) / myFieldExt;
	iVertPos = (ptPos.y - myVertIndent) / myFieldExt;

	// check if mouse is over sudoku rectangle
	if (iHoriPos > (myBoardDim-1) || iVertPos > (myBoardDim-1)) return false;

	CSudokuField* pField = GetDisplayField(CPoint(iHoriPos, iVertPos));
	if (pField) pField->ToggleCandidate(ptPos, nNumber);

	return true;
}

void CSudokuBoardDuo::ToggleCandidate(CPoint ptId, int nNumber)
{
	CSudokuField* pField = GetSudokuField(ptId);
	if (pField) pField->ToggleCandidate(nNumber);
}

void CSudokuBoardDuo::MoveBoard(MoveDirection_t moveDir, int nMoveValue)
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

void CSudokuBoardDuo::Serialize(CArchive& ar)
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
	CPoint ptVect1, ptVect2, ptVect3;
	for (int y=0; y<myBoardDim; y++)
	{
		for (int x=0; x<myBoardDim; x++)
		{
			VectorResult_t vr = BoardToVector(CPoint(x,y), ptVect1, ptVect2, ptVect3);
			while (vr > vrInvalid)
			{
				if (vr == vrValidSingle)
				{
					if (!bUnion)
					{
						if (ar.IsStoring())
						{
							pField = (CSudokuField*)mySudokuRows[ptVect1.y].GetAt(ptVect1.x);
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
						pField = (CSudokuField*)mySudokuRows[ptVect2.y].GetAt(ptVect2.x);
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

void CSudokuBoardDuo::DrawSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;
	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 2, RGB(0,0,0));
    CBrush* pOldBrush;
    CBrush aBkBrush(m_backColor);
	CPoint ptsBorder[10];
	DWORD  counts[2];
	int i;
    int nModHor =3;

	// draw rectangles
    pOldBrush = pDC->SelectObject(&aBkBrush);
	pOldPen = pDC->SelectObject(&aBoldPen);
	ptsBorder[0].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[0].y = myVertIndent;
	ptsBorder[1].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[1].y = myVertIndent;
	ptsBorder[2].x = myFieldExt*myBoardDim + myHoriIndent;
	ptsBorder[2].y = myFieldExt*9 + myVertIndent;
	ptsBorder[3].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[3].y = myFieldExt*9 + myVertIndent;
	ptsBorder[4].x = myHoriIndent + 6*myFieldExt;
	ptsBorder[4].y = myVertIndent;
	counts[0] = 5;
	ptsBorder[5].x = myHoriIndent;
	ptsBorder[5].y = myVertIndent + 6*myFieldExt;
	ptsBorder[6].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[6].y = myVertIndent + 6*myFieldExt;
	ptsBorder[7].x = myFieldExt*9 + myHoriIndent;
	ptsBorder[7].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[8].x = myHoriIndent;
	ptsBorder[8].y = myFieldExt*myBoardDim + myVertIndent;
	ptsBorder[9].x = myHoriIndent;
	ptsBorder[9].y = myVertIndent + 6*myFieldExt;
	counts[1] = 5;
	pDC->PolyPolyline(ptsBorder, counts, 2);

	pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
	// draw horizontal lines
	int y = myFieldExt + myVertIndent;
	int x = myFieldExt * myBoardDim + myHoriIndent;
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
		pDC->MoveTo(myHoriIndent + 6*myFieldExt, y);
		pDC->LineTo(x,y);
		y += myFieldExt;
	}
	y = 7*myFieldExt + myVertIndent;
	x = myFieldExt * 9 + myHoriIndent;
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
		pDC->MoveTo(myHoriIndent, y);
		pDC->LineTo(x,y);
		y += myFieldExt;
	}
	// draw vertical lines
	y = myFieldExt * myBoardDim + myVertIndent;
	x = myFieldExt + myHoriIndent;
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
		pDC->MoveTo(x, myVertIndent + 6*myFieldExt);
		pDC->LineTo(x,y);
		x += myFieldExt;
	}
	y = myFieldExt * 9 + myVertIndent;
	x = 7*myFieldExt + myHoriIndent;
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
		pDC->MoveTo(x, myVertIndent);
		pDC->LineTo(x,y);
		x += myFieldExt;
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

void CSudokuBoardDuo::PrintSudokuBoard(CDC* pDC)
{
	CSudokuField* pField;

	CPen*  pOldPen;
	CPen   aBoldPen(PS_SOLID, 60, RGB(0,0,0));
	CPen   aThinPen(PS_SOLID, 30, RGB(0,0,0));
	CPoint ptsBorder[10];
	DWORD  counts[2];
	int i;
    int nModHor = 3;

	// print rectangles
	pOldPen = pDC->SelectObject(&aBoldPen);
	ptsBorder[0].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[0].y = -myVertIndentDP;
	ptsBorder[1].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[1].y = -myVertIndentDP;
	ptsBorder[2].x = myFieldExtDP*myBoardDim + myHoriIndentDP;
	ptsBorder[2].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[3].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[3].y = -myFieldExtDP*9 - myVertIndentDP;
	ptsBorder[4].x = myHoriIndentDP + 6*myFieldExtDP;
	ptsBorder[4].y = -myVertIndentDP;
	counts[0] = 5;
	ptsBorder[5].x = myHoriIndentDP;
	ptsBorder[5].y = -myVertIndentDP - 6*myFieldExtDP;
	ptsBorder[6].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[6].y = -myVertIndentDP - 6*myFieldExtDP;
	ptsBorder[7].x = myFieldExtDP*9 + myHoriIndentDP;
	ptsBorder[7].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[8].x = myHoriIndentDP;
	ptsBorder[8].y = -myFieldExtDP*myBoardDim - myVertIndentDP;
	ptsBorder[9].x = myHoriIndentDP;
	ptsBorder[9].y = -myVertIndentDP - 6*myFieldExtDP;
	counts[1] = 5;
	pDC->PolyPolyline(ptsBorder, counts, 2);
	pDC->SelectObject(pOldPen);
	// draw horizontal lines
	int y = -myFieldExtDP - myVertIndentDP;
	int x = myFieldExtDP * myBoardDim + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(6*myFieldExtDP + myHoriIndentDP, y);
		pDC->LineTo(x,y);
		y -= myFieldExtDP;
	}
	y = -7*myFieldExtDP - myVertIndentDP;
	x = myFieldExtDP * 9 + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%nModHor))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(myHoriIndentDP, y);
		pDC->LineTo(x,y);
		y -= myFieldExtDP;
	}
	// draw vertical lines
	y = -myFieldExtDP * myBoardDim - myVertIndentDP;
	x = myFieldExtDP + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(x, -myFieldExtDP*6 - myVertIndentDP);
		pDC->LineTo(x,y);
		x += myFieldExtDP;
	}
	y = -myFieldExtDP * 9 - myVertIndentDP;
	x = 7*myFieldExtDP + myHoriIndentDP;
	for (i=1; i<9; i++)
	{
		if (!(i%3))
		{
			pDC->SelectObject(&aBoldPen);
		}
		else
		{
			pDC->SelectObject(&aThinPen);
		}
		pDC->MoveTo(x,  -myVertIndentDP);
		pDC->LineTo(x,y);
		x += myFieldExtDP;
	}
	pDC->SelectObject(pOldPen);

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

void CSudokuBoardDuo::FinalConstruction()
{
    myCandCounts    = new int[9];
    arrDisable      = new BOOL[9];
    mySudokuSquares = new CObArray[18];
    mySudokuColumns = new CObArray[18];
    mySudokuRows    = new CObArray[18];
    myCandCounter   = new CCandCounter(myBoardDim);
    mySingleCounter = new CCandCounter(myBoardDim);
	m_iLastVertPos	= 0;
	m_iLastHoriPos	= 0;
	myFieldExt = 40;
	myFieldExtDP = 1200;
	myCandLen  = 6;
	myCandLenDP = 180;
    for (int i=0; i<9; i++)
    {
        arrDisable[i] = TRUE;
        myCandCounts[i] = 0;
    }
    m_bConstructed = true;
}

void CSudokuBoardDuo::SelectSingles(CObArray* pFieldArray, int nDim, BOOL bSelect)
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

bool CSudokuBoardDuo::SolveInArray(CObArray* pFieldArray, int nDim)
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

bool CSudokuBoardDuo::AddSudokuField(CSudokuField* pField, int iHoriPos, int iVertPos)
{
	bool bRet = false;
	CPoint ptVect1, ptVect2, ptVect3;
	VectorResult_t vr = BoardToVector(CPoint(iHoriPos, iVertPos), ptVect1, ptVect2, ptVect3);

	while (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// fill upper 9 x 9 field
			// add field to row
			mySudokuRows[ptVect1.y].Add(pField);
			// add field to column
			if (ptVect3.x < 0)
			{
				mySudokuColumns[ptVect1.x].Add(pField);
			}
			else
			{
				mySudokuColumns[ptVect3.x].Add(pField);
			}
			// add field to square
			int iIndex = (ptVect1.x/3) + ((ptVect1.y/3)*3);
			mySudokuSquares[iIndex].Add(pField);
			bRet = true;
		}
		else
		{
			// fill lower 9 x 9 field
			// add field to row
			mySudokuRows[ptVect2.y].Add(pField);
			// add field to column
			mySudokuColumns[ptVect3.x].Add(pField);
			ptVect3 = CPoint(-1,-1);
			// add field to square
			int iIndex = (ptVect2.x/3) + ((ptVect2.y/3)*3);
			mySudokuSquares[iIndex].Add(pField);
			// fill the common field in the middle
			// add field to union
			mySudokuUnion.Add(pField);
		}
		int iVr = (int)vr;
		iVr--;
		vr = (VectorResult_t)iVr;
	}
	return bRet;
}

void CSudokuBoardDuo::DeleteSudokuFields()
{
	CSudokuField* pField;
    int y, x;
	int nCount = 0;

    if (!m_bConstructed) return;

    for (y=0; y<18; y++)
    {
        for (x=0; x<9; x++)
        {
            if (mySudokuRows[y].GetSize())
            {
				if (x<6 || (x>5 && y<9) || (x>5 && y>11))
				{
					pField = (CSudokuField*)mySudokuRows[y].GetAt(x);
					delete pField;
					nCount++;
				}
            }
        }
        mySudokuRows[y].RemoveAll();
        mySudokuColumns[y].RemoveAll();
        mySudokuSquares[y].RemoveAll();
    }
	mySudokuUnion.RemoveAll();
	TRACE1("Ich habe %d Felder gelöscht\n", nCount);
}

BOOL CSudokuBoardDuo::CheckNumberCompletion(int nNumber)
{
	CSudokuField* pField;
    int i, j;
    int nCount = 0;

    for (i=0; i<15; i++)
    {
        for (j=0; j<15; j++)
        {
            pField = GetDisplayField(CPoint(j,i));
            if (pField && pField->m_pFieldEdit->myNumber == nNumber) nCount++;
        }
    }

    myCandCounts[nNumber-1] = nCount;

    return (nCount == 17 ? FALSE : TRUE);
}

/***********************************
*    Helper Functions              *
***********************************/

VectorResult_t CSudokuBoardDuo::BoardToVector(CPoint ptBoard , CPoint& ptVect1, CPoint& ptVect2, CPoint& ptVect3)
{
	VectorResult_t vectorResult  = vrValidSingle;
	int x = ptBoard.x,
		y = ptBoard.y;

	ptVect2 = CPoint(-1,-1);
	ptVect3 = CPoint(-1,-1);

	if (x > 5 && y < 6)
	{
		// upper single
		ptVect1 = CPoint(x-6,y);
	}
	else if (x < 6 && y > 5 && y < 9)
	{
		// left single
		ptVect1 = CPoint(x,y+3);
		ptVect3 = CPoint(x+9,y-6);
	}
	else if (x > 5 && x < 9 && y > 5 && y < 9)
	{
		// union square
		ptVect1 = CPoint(x-6,y);
		ptVect2 = CPoint(x, y+3);
		ptVect3 = CPoint(x+9, y-6);
		vectorResult = vrValidDouble;
	}
	else if (x > 8 && y > 5 && y < 9)
	{
		// right single
		ptVect1 = CPoint(x-6,y);
	}
	else if (x < 9 && y > 8)
	{
		// lower single
		ptVect1 = CPoint(x, y+3);
		ptVect3 = CPoint(x+9, y-6);
	}
	else
	{
		vectorResult = vrInvalid;
	}
	return vectorResult;
}

CSudokuField* CSudokuBoardDuo::GetDisplayField(CPoint ptBoard)
{
	CPoint ptVect1, ptVect2, ptVect3;
	CSudokuField* pField = NULL;
	
	VectorResult_t vr = BoardToVector(ptBoard, ptVect1, ptVect2, ptVect3);
	if (vr > vrInvalid)
	{
		if (vr == vrValidSingle)
		{
			// get a single field from upper or lower 9 x 9
			pField = (CSudokuField*)mySudokuRows[ptVect1.y].GetAt(ptVect1.x);
		}
		else
		{
			// return a filed from union square
			pField = (CSudokuField*)mySudokuUnion.GetAt(ptVect2.y%3*3 + ptVect2.x-6);
		}
	}
	return pField;
}

void CSudokuBoardDuo::AddSudokuField(CSudokuField* pField)
{
	mySudokuUnion.Add(pField);
}

