// SudokuDuoDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuMDIView.h"
#include "SudokuBoardDuo.h"
#include "SudokuDuoDoc.h"


// CSudokuDuoDoc

IMPLEMENT_DYNCREATE(CSudokuDuoDoc, CSudokuDoc)

CSudokuDuoDoc::CSudokuDuoDoc()
: m_pDuoBoard(NULL)
{
	m_pDuoBoard = new CSudokuBoardDuo(15);
	m_pSudokuBoard = m_pDuoBoard;
	m_pDuoBoard->CreateEmptyBoard();
	m_pDuoBoard->SetBoardIndex(btDuo);
	m_BoardType = btDuo;
    m_pUndoList = new CUndoList;
    m_szPathName = "";
    m_bAnsichtMitKand = FALSE;
}

BOOL CSudokuDuoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSudokuDuoDoc::~CSudokuDuoDoc()
{
	delete m_pDuoBoard;
}


BEGIN_MESSAGE_MAP(CSudokuDuoDoc, CSudokuDoc)
END_MESSAGE_MAP()


// CSudokuDuoDoc diagnostics

#ifdef _DEBUG
void CSudokuDuoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSudokuDuoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CSudokuDuoDoc serialization

void CSudokuDuoDoc::Serialize(CArchive& ar)
{
	CSudokuDoc::Serialize(ar);

	if (ar.IsLoading())
	{
		if (m_pSudokuBoard)
		{
			if (m_pDuoBoard)
			{
				delete m_pDuoBoard;
			}
			m_pDuoBoard = static_cast<CSudokuBoardDuo*>(m_pSudokuBoard);
			m_szPathName = ar.GetFile()->GetFilePath();
			m_pDuoBoard->InitSudokuField(m_pParent);
			m_pSudokuBoard->SwitchEditMode(true);
			m_pSudokuBoard->EnableBoard(true);
			static_cast<CSudokuMDIView*>(m_pParent)->ResizeFrameWindow();
			RefreshCandidates();
			m_pParent->Invalidate();
			m_pParent->UpdateWindow();
		}
		else
		{
			m_pSudokuBoard = m_pDuoBoard;
		}
	}	
}


// CSudokuDuoDoc commands
