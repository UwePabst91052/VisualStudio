// SudokuDoc.cpp : Implementierung der Klasse CSudokuDoc
//

#include "stdafx.h"
#include "SudokuMDI.h"

#include "SudokuMDIView.h"
#include "SudokuDoc.h"
#include "SudokuBoardDuo.h"
#include "SudokuBoardSamurai.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuDoc

IMPLEMENT_DYNCREATE(CSudokuDoc, CDocument)

BEGIN_MESSAGE_MAP(CSudokuDoc, CDocument)
	ON_COMMAND(ID_AKTIONEN_SELECTSINGLE, OnAktionenSelectsingle)
	ON_COMMAND(ID_AKTIONEN_SINGLEINFIELD, OnAktionenSingleInField)
	ON_COMMAND(ID_AKTIONEN_SELECTPAIR, OnAktionenSelectpair)
	ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SELECTPAIR, OnUpdateAktionenSelectpair)
	ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SELECTSINGLE, OnUpdateAktionenSelectsingle)
	ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SINGLEINFIELD, OnUpdateAktionenSingleInField)
	ON_COMMAND(ID_AKTIONEN_SELECTPAIR, OnAktionenSelectpair)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO_ALL, OnEditRedoAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO_ALL, OnUpdateEditRedoAll)
	ON_COMMAND(ID_UNDO_TRIAL, OnUndoTrial)
	ON_UPDATE_COMMAND_UI(ID_UNDO_TRIAL, OnUpdateUndoTrial)
	ON_COMMAND(ID_BEGIN_TRIAL, OnBeginTrial)
	ON_UPDATE_COMMAND_UI(ID_BEGIN_TRIAL, OnUpdateBeginTrial)
	ON_COMMAND(ID_EDIT_MODE, OnEditMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODE, OnUpdateEditMode)
	ON_COMMAND(ID_ANSICHT_MIT_KAND, OnAnsichtMitkandidaten)
	ON_UPDATE_COMMAND_UI(ID_ANSICHT_MIT_KAND, OnUpdateAnsichtMitkandidaten)
	ON_COMMAND(ID_MOVE_DOWN, &CSudokuDoc::OnMoveDown)
	ON_COMMAND(ID_MOVE_LEFT, &CSudokuDoc::OnMoveLeft)
	ON_COMMAND(ID_MOVE_RIGHT, &CSudokuDoc::OnMoveRight)
	ON_COMMAND(ID_MOVE_UP, &CSudokuDoc::OnMoveUp)
	ON_COMMAND(ID_AKTIONEN_SOLVE_LEVEL1, &CSudokuDoc::OnAktionenSolveLevel1)
	ON_COMMAND(ID_AKTIONEN_SOLVE_LEVEL2, &CSudokuDoc::OnAktionenSolveLevel2)
	ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SOLVE_LEVEL1, &CSudokuDoc::OnUpdateAktionenSolveLevel1)
	ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SOLVE_LEVEL2, &CSudokuDoc::OnUpdateAktionenSolveLevel2)
    ON_COMMAND(ID_AKTIONEN_SELECTTRIPLE, &CSudokuDoc::OnAktionenSelecttriple)
    ON_UPDATE_COMMAND_UI(ID_AKTIONEN_SELECTTRIPLE, &CSudokuDoc::OnUpdateAktionenSelecttriple)
END_MESSAGE_MAP()


// CSudokuDoc Erstellung/Zerstörung

CSudokuDoc::CSudokuDoc()
: m_pParent(NULL)
, m_bEditMode(TRUE)
, m_bAktionSingle(FALSE)
, m_bAktionSingleInField(FALSE)
, m_bAktionPair(FALSE)
, m_bAktionTriple(FALSE)
, m_bBeginTrial(FALSE)
, m_bEditUndo(FALSE)
, m_bEditRedo(FALSE)
, m_bUndoTrial(FALSE)
, m_bEditRedoAll(FALSE)
, m_BoardType(btSquare9x9)
, m_pSudokuBoard(NULL)
, m_nSingleInFields(0)
, m_nSingleOnBoard(0)
, m_msMode(msNone)
, m_backColor(RGB(255,255,255))
, m_scoreColor(RGB(255,255,255))
{
	m_backColor = AfxGetApp()->GetProfileInt(L"Settings", L"ViewBackgroundColor", (int)m_backColor);
	m_scoreColor = AfxGetApp()->GetProfileInt(L"Settings", L"ScoreBackgroundColor", (int)m_scoreColor);
}

CSudokuDoc::~CSudokuDoc()
{
    delete m_pUndoList;
}

BOOL CSudokuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    m_pSudokuBoard->WipeBoard();

	RefreshCandidates();

	m_pUndoList->SetNewList();
    m_szPathName = GetPathName();
	m_bEditUndo = FALSE;
	m_bEditRedo = FALSE;
	m_bUndoTrial = FALSE;
    m_bEditRedoAll = FALSE;

	return TRUE;
}

// CSudokuDoc Serialisierung

void CSudokuDoc::Serialize(CArchive& ar)
{
    CUndoList* pListLoad = NULL;
	int nCurBoardIndex;
	int nSchema = 3;

	if (ar.IsStoring())
    {
        ReDoAllActions();
    }
    if (ar.IsStoring())
    {
		ar << nSchema;
		ar << m_BoardType;
        ar << m_pSudokuBoard;
        ar << m_pUndoList;
		ar << m_backColor;
		ar << m_scoreColor;
        m_bEditRedo = FALSE;
        m_bEditRedoAll = FALSE;
        m_szPathName = ar.GetFile()->GetFilePath();
    }
    else
    {
		ar >> nSchema;

		if (nSchema == 1)
		{
			ar >> nCurBoardIndex;
			switch (nCurBoardIndex)
			{
			case 0:
				m_BoardType = btSquare9x9;
				break;
			case 1:
				m_BoardType = btSquare12x12;
				break;
			case 2:
				m_BoardType = btDuo;
				break;
			case 3:
				m_BoardType = btSamurai;
				break;
			case 4:
				m_BoardType = btSquare16x16;
				break;
			}
		}
		else
		{
			int nBoardType;
			ar >> nBoardType;
			m_BoardType = (BoardType_t)nBoardType;
		}
		
        m_pSudokuBoard = NULL;
		CSudokuBoardDuo* m_pDuoBoard = NULL;
		CSudokuBoardSamurai* m_pSamuraiBoard = NULL;
        TRY
        {
			if (m_BoardType == btDuo)
			{
				ar >> m_pDuoBoard;
				m_pSudokuBoard = m_pDuoBoard;
			}
			else if (m_BoardType == btSamurai)
			{
				ar >> m_pSamuraiBoard;
				m_pSudokuBoard = m_pSamuraiBoard;
			}
			else
			{
				ar >> m_pSudokuBoard;
			}
            ar >> pListLoad;
            if (pListLoad) 
            {
                delete m_pUndoList;
                m_pUndoList = pListLoad;
            }
			if (nSchema == 3)
			{
				ar >> m_backColor;
				ar >> m_scoreColor;
			}
        }
        CATCH_ALL(e)
        {
			if (m_pSudokuBoard)
			{
				delete m_pSudokuBoard;
				m_pSudokuBoard = NULL;
			}
			if (pListLoad) delete pListLoad;
			wchar_t msg[255];
			e->GetErrorMessage(msg, 255);
			TRACE1("Exception: %s\n", msg);
			return;
        }
        END_CATCH_ALL;
    }
    if (ar.IsLoading() && !m_pUndoList->IsEmpty())
    {
        m_bEditUndo = TRUE;
        m_bUndoTrial = TRUE;
    }
}


// CSudokuDoc Diagnose

#ifdef _DEBUG
void CSudokuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSudokuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSudokuDoc-Befehle

void CSudokuDoc::DrawSudokuBoard(CDC* pDC)
{
	m_pSudokuBoard->DrawSudokuBoard(pDC);
}

void CSudokuDoc::PrintSudokuBoard(CDC* pDC)
{
	m_pSudokuBoard->PrintSudokuBoard(pDC);
}

void CSudokuDoc::InitSudokoField(CWnd* pParent)
{
	m_pParent = pParent;

    m_pSudokuBoard->InitSudokuField(pParent);
    m_pSudokuBoard->SwitchEditMode(true);
	m_pSudokuBoard->EnableBoard(true);
}

void CSudokuDoc::SetSudokoFieldNumber(CPoint ptId, int nNumber)
{
	int nOldNum = m_pSudokuBoard->GetFieldNumberOld(ptId);
	if (m_pSudokuBoard->IsDuplicate(ptId, nNumber) ||
        !m_pSudokuBoard->IsNumberSelectedSingle(ptId, nNumber))
	{
		m_pSudokuBoard->SetFieldNumber(ptId, 0);
	}
	if (nOldNum > 0)
	{
		m_pUndoList->AddReplaceAction(ptId, nNumber, nOldNum, m_bMark);
	}
	else
	{
		m_pUndoList->AddNewAction(ActionEnterNumber, ptId, nNumber, m_bMark);
	}
	m_bEditUndo = TRUE;
	m_bEditRedo = FALSE;
	m_bUndoTrial = TRUE;
    m_bEditRedoAll = FALSE;
	m_bBeginTrial = FALSE;
	m_bMark = false;

    SetModifiedFlag();

	RefreshCandidates();
}

void CSudokuDoc::DeleteCandidates(int xPos, int yPos, int nNumber)
{
	m_pSudokuBoard->DeleteCandidates(xPos, yPos, nNumber);
}

void CSudokuDoc::RefreshCandidates(void)
{
    m_pSudokuBoard->RefreshCandidates();
    m_pSudokuBoard->ResetCandidates();
    m_pSudokuBoard->UpdateNumberCompletion();
    m_pSudokuBoard->SelectMulti(m_msMode);
    m_pSudokuBoard->SelectSingleInField(m_bAktionSingleInField);
	m_pSudokuBoard->SelectSingles(m_bAktionSingle);
	m_pSudokuBoard->IsSingleInFields(m_nSingleInFields);
	m_pSudokuBoard->IsSingleOnBoard(m_nSingleOnBoard);
	static_cast<CSudokuMDIView*>(m_pParent)->UpdateCandDisplay(m_nSingleInFields, m_nSingleOnBoard);
}

void CSudokuDoc::MoveCursor(CPoint ptId, int iMoveDir)
{
	m_pSudokuBoard->GetMovedCursor(ptId, iMoveDir);
	m_pSudokuBoard->SetEditFocus(ptId);
}

void CSudokuDoc::EvalMousePosition(CPoint ptPos)
{
	m_pSudokuBoard->EvalMousePosition(ptPos);
}

void CSudokuDoc::ToggleCandidate(CPoint ptPos)
{
	int iHoriPos, iVertPos, nNumber;

	if (m_pSudokuBoard->ToggleCandidate(ptPos, iHoriPos, iVertPos, nNumber))
	{
		m_pUndoList->AddNewAction(ActionToggleCandidate, CPoint(iHoriPos, iVertPos), nNumber, m_bMark);
		m_bEditUndo = TRUE;
		m_bEditRedo = FALSE;
		m_bUndoTrial = TRUE;
		m_bEditRedoAll = FALSE;
		m_bBeginTrial = FALSE;
		m_bMark = false;
		RefreshCandidates();
		m_pParent->UpdateWindow();
	}
}

void CSudokuDoc::OnAktionenSelectsingle()
{
	m_bAktionSingle = (m_bAktionSingle ? FALSE : TRUE);

    if (m_bAktionSingle && (m_bAktionPair || m_bAktionTriple))
    {
        m_bAktionPair = m_bAktionTriple = FALSE;
        RefreshCandidates();
    }

	m_pSudokuBoard->SelectSingles(m_bAktionSingle);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnAktionenSingleInField()
{
	m_bAktionSingleInField = (m_bAktionSingleInField ? FALSE : TRUE);

    if (m_bAktionSingleInField && (m_bAktionPair || m_bAktionTriple))
    {
        m_bAktionPair = m_bAktionTriple = FALSE;
        RefreshCandidates();
    }

    m_pSudokuBoard->SelectSingleInField(m_bAktionSingleInField);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnAktionenSelectpair()
{
	m_bAktionPair = (m_bAktionPair ? FALSE : TRUE);

    if (m_bAktionPair && (m_bAktionSingle || m_bAktionSingleInField))
    {
        m_bAktionSingle = m_bAktionSingleInField = FALSE;
        RefreshCandidates();
    }

    m_msMode = (MultiSelect_t)(m_bAktionPair ? m_msMode | msPair : m_msMode & ~msPair);
    m_pSudokuBoard->SelectMulti(m_msMode);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnAktionenSelecttriple()
{
	m_bAktionTriple = (m_bAktionTriple ? FALSE : TRUE);

    if (m_bAktionTriple && (m_bAktionSingle || m_bAktionSingleInField))
    {
        m_bAktionSingle = m_bAktionSingleInField = FALSE;
        RefreshCandidates();
    }

    m_msMode = (MultiSelect_t)(m_bAktionTriple ? m_msMode | msTriple : m_msMode & ~msTriple);
    m_pSudokuBoard->SelectMulti(m_msMode);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnUpdateAktionenSelectpair(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAktionPair);
}

void CSudokuDoc::OnUpdateAktionenSelectsingle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAktionSingle);
}

void CSudokuDoc::OnUpdateAktionenSelecttriple(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAktionTriple);
}

void CSudokuDoc::OnUpdateAktionenSingleInField(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAktionSingleInField);
}

void CSudokuDoc::SelectScan(int c, bool bSelect, BOOL bAllowMulti)
{
    m_pSudokuBoard->SelectScan(c, bSelect, bAllowMulti);
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnEditUndo()
{
	ActionType_t actionType;
	CPoint ptId;
	int nNumber;

	m_bEditUndo = m_pUndoList->UndoOneAction(actionType, ptId, nNumber, m_bMark) ? FALSE : TRUE;
	if (actionType == ActionEnterNumber)
	{
		m_pSudokuBoard->SetFieldNumber(ptId, nNumber);
	}
	else if (actionType == ActionToggleCandidate)
	{
		m_pSudokuBoard->GetSudokuField(ptId)->ToggleCandidate(nNumber);
	}
	m_pSudokuBoard->SetEditFocus(ptId);
    SetModifiedFlag(m_bEditUndo);
	m_bEditRedo = TRUE;
    m_bEditRedoAll = TRUE;
    m_bBeginTrial = m_bMark ? TRUE : FALSE;
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bEditUndo);
}

void CSudokuDoc::OnEditRedo()
{
	ActionType_t actionType;
	CPoint ptId;
	int nNumber;

	m_bEditRedo = m_pUndoList->RedoOneAction(actionType, ptId, nNumber, m_bMark) ? FALSE : TRUE;
	if (actionType == ActionEnterNumber)
	{
		m_pSudokuBoard->SetFieldNumber(ptId, nNumber);
	}
	else if (actionType == ActionToggleCandidate)
	{
		m_pSudokuBoard->GetSudokuField(ptId)->ToggleCandidate(nNumber);
	}
	m_pSudokuBoard->SetEditFocus(ptId);
    if (!IsModified()) SetModifiedFlag();
	m_bEditUndo = TRUE;
	m_bUndoTrial = TRUE;
    m_bEditRedoAll = m_bEditRedo;
    m_bBeginTrial = m_bMark ? TRUE : FALSE;
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnEditRedoAll()
{
	ActionType_t actionType;
	CPoint ptId;
	int nNumber;

    do
    {
	    m_bEditRedo = m_pUndoList->RedoOneAction(actionType, ptId, nNumber, m_bMark) ? FALSE : TRUE;
	    if (actionType == ActionEnterNumber)
	    {
    		m_pSudokuBoard->SetFieldNumber(ptId, nNumber);
	    }
	    else if (actionType == ActionToggleCandidate)
	    {
		    m_pSudokuBoard->GetSudokuField(ptId)->ToggleCandidate(nNumber);
	    }
    }
    while (m_bEditRedo);
	m_bEditUndo = TRUE;
	m_bUndoTrial = TRUE;
    m_bEditRedoAll = m_bEditRedo;
    m_bBeginTrial = m_bMark ? TRUE : FALSE;
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::ReDoAllActions()
{
    if (m_bEditRedo)
    {
        int nMBRet = AfxMessageBox(_T("Sollen die zurückgenommenen Aktionen vor dem Speichern wiederhergestellt werden ?"), 
                                    MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
        if (nMBRet == IDYES)
        {
            OnEditRedoAll();
        }
    }
}

void CSudokuDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bEditRedo);
}

void CSudokuDoc::OnUpdateEditRedoAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bEditRedoAll);
}

void CSudokuDoc::OnUndoTrial()
{
	ActionType_t actionType;
	CPoint ptId;
	int nNumber;
	bool bRetVal = false;

	do
	{
		bRetVal = m_pUndoList->UndoLastTrial(actionType, ptId, nNumber, m_bMark);
		if (actionType == ActionEnterNumber)
		{
    		m_pSudokuBoard->SetFieldNumber(ptId, nNumber);
		}
		else if (actionType == ActionToggleCandidate)
		{
    		m_pSudokuBoard->GetSudokuField(ptId)->ToggleCandidate(nNumber);
		}
	}
	while (!bRetVal);
	m_pSudokuBoard->SetEditFocus(ptId);

	m_bUndoTrial = m_bEditUndo = m_pUndoList->IsUndoEmpty() ? FALSE : TRUE;
    SetModifiedFlag(m_bUndoTrial);
	m_bEditRedo = TRUE;
    m_bEditRedoAll = TRUE;
    m_bBeginTrial = m_bMark ? TRUE : FALSE;
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnUpdateUndoTrial(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bUndoTrial);
}

void CSudokuDoc::OnBeginTrial()
{
	m_bBeginTrial = m_bBeginTrial ? FALSE : TRUE;
    m_bMark = m_pUndoList->SetTrialMarker(m_bBeginTrial ? true : false);
}

void CSudokuDoc::OnUpdateBeginTrial(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBeginTrial);
}

void CSudokuDoc::OnEditMode()
{
	m_bEditMode = m_bEditMode ? FALSE : TRUE;
	bool bEditMode = m_bEditMode ? true : false;
    if (bEditMode) 
    {
        m_pSudokuBoard->CleanHover();
    }
    m_pSudokuBoard->SwitchEditMode(bEditMode);

	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnUpdateEditMode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode);
}

int CSudokuDoc::GetCurBoardDim()
{
    return m_pSudokuBoard->GetBoardDim();
}

BoardType_t CSudokuDoc::GetBoardType()
{
    return m_BoardType;
}
void CSudokuDoc::OnAnsichtMitkandidaten()
{
	m_bAnsichtMitKand = m_bAnsichtMitKand ? FALSE : TRUE;
	bool bShowCandidates = m_bAnsichtMitKand ? true : false;
	m_pSudokuBoard->ShowCandidates(bShowCandidates);
}

void CSudokuDoc::OnUpdateAnsichtMitkandidaten(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAnsichtMitKand);
}

void CSudokuDoc::OnMoveDown()
{
	m_pSudokuBoard->MoveBoard(moveDown, 10);
}

void CSudokuDoc::OnMoveLeft()
{
	m_pSudokuBoard->MoveBoard(moveLeft, 10);
}

void CSudokuDoc::OnMoveRight()
{
	m_pSudokuBoard->MoveBoard(moveRight, 10);
}

void CSudokuDoc::OnMoveUp()
{
	m_pSudokuBoard->MoveBoard(moveUp, 10);
}

void CSudokuDoc::SetBoardProperties(SudokuProps_t props)
{
	m_backColor = props.myBackColor;
	m_scoreColor = props.myFillColor;
    m_pSudokuBoard->SetBoardProperties(props);
}

void CSudokuDoc::GetBoardProperties(SudokuProps_t& props)
{
    m_pSudokuBoard->GetBoardProperties(props);
	props.myBackColor = m_backColor;
	props.myFillColor = m_scoreColor;
}

void CSudokuDoc::GetBoardDimensions(CRect& rcBoard)
{
	SudokuProps_t props;

	int nBoardDim = m_pSudokuBoard->GetBoardDim();
	m_pSudokuBoard->GetBoardProperties(props);
	rcBoard = CRect(0,0,0,0);
	rcBoard.right  = props.myHoriIndent + nBoardDim * props.myFieldSize + 50;
	rcBoard.bottom = props.myVertIndent + nBoardDim * props.myFieldSize + 50;
}

void CSudokuDoc::OnAktionenSolveLevel1()
{
	//m_bMark = true;
	SetModifiedFlag(m_pSudokuBoard->SolveLevel1() ? TRUE : FALSE);
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnAktionenSolveLevel2()
{
	//m_bMark = true;
	SetModifiedFlag(m_pSudokuBoard->SolveLevel2() ? TRUE : FALSE);
	RefreshCandidates();
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
}

void CSudokuDoc::OnUpdateAktionenSolveLevel1(CCmdUI *pCmdUI)
{
	BOOL bEnable = (m_pSudokuBoard->IsSingleInFields(m_nSingleInFields) ? TRUE : FALSE);
	pCmdUI->Enable(bEnable);

}

void CSudokuDoc::OnUpdateAktionenSolveLevel2(CCmdUI *pCmdUI)
{
	BOOL bEnable = (m_pSudokuBoard->IsSingleOnBoard(m_nSingleOnBoard) ? TRUE : FALSE);
	pCmdUI->Enable(bEnable);
}
