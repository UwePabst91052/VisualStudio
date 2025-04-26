// SudokuSamuraiDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuMDIView.h"
#include "SudokuBoardSamurai.h"
#include "SudokuSamuraiDoc.h"


// CSudokuSamuraiDoc

IMPLEMENT_DYNCREATE(CSudokuSamuraiDoc, CSudokuDoc)

CSudokuSamuraiDoc::CSudokuSamuraiDoc()
: m_pSamuraiBoard(NULL)
{
	m_pSamuraiBoard = new CSudokuBoardSamurai(21);
	m_pSudokuBoard = m_pSamuraiBoard;
	m_pSamuraiBoard->CreateEmptyBoard();
	m_pSamuraiBoard->SetBoardIndex(btSamurai);
	m_BoardType = btSamurai;
    m_pUndoList = new CUndoList;
    m_szPathName = "";
    m_bAnsichtMitKand = FALSE;
}

BOOL CSudokuSamuraiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSudokuSamuraiDoc::~CSudokuSamuraiDoc()
{
	delete m_pSamuraiBoard;
}


BEGIN_MESSAGE_MAP(CSudokuSamuraiDoc, CSudokuDoc)
END_MESSAGE_MAP()


// CSudokuSamuraiDoc diagnostics

#ifdef _DEBUG
void CSudokuSamuraiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSudokuSamuraiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CSudokuSamuraiDoc serialization

void CSudokuSamuraiDoc::Serialize(CArchive& ar)
{
	CSudokuDoc::Serialize(ar);

	if (ar.IsLoading())
	{
		if (m_pSudokuBoard)
		{
			if (m_pSamuraiBoard)
			{
				delete m_pSamuraiBoard;
			}
			m_pSamuraiBoard = static_cast<CSudokuBoardSamurai*>(m_pSudokuBoard);
			m_szPathName = ar.GetFile()->GetFilePath();
			m_pSamuraiBoard->InitSudokuField(m_pParent);
			m_pSudokuBoard->SwitchEditMode(true);
			m_pSudokuBoard->EnableBoard(true);
			static_cast<CSudokuMDIView*>(m_pParent)->ResizeFrameWindow();
			RefreshCandidates();
			m_pParent->Invalidate();
			m_pParent->UpdateWindow();
		}
		else
		{
			m_pSudokuBoard = m_pSamuraiBoard;
		}
	}
}


// CSudokuSamuraiDoc commands
