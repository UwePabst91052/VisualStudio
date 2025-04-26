// SudokuSquareDoc.cpp : Implementierung der Klasse CSudokuSquareDoc
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuMDIView.h"
#include "SudokuSquareDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BoardType_t CSudokuSquareDoc::s_BoardType = btSquare9x9;

// CSudokuSquareDoc

IMPLEMENT_DYNCREATE(CSudokuSquareDoc, CSudokuDoc)

BEGIN_MESSAGE_MAP(CSudokuSquareDoc, CSudokuDoc)
END_MESSAGE_MAP()


// CSudokuSquareDoc-Erstellung/Zerstörung

CSudokuSquareDoc::CSudokuSquareDoc()
: m_pSquareBoard(NULL)
{
	if (s_BoardType == btSquare8x8)
	{
		m_pSquareBoard = new CSudokuBoard(8);
	}
	else if (s_BoardType == btSquare9x9)
	{
		m_pSquareBoard = new CSudokuBoard(9);
	}
	else if (s_BoardType == btSquare12x12)
	{
		m_pSquareBoard = new CSudokuBoard(12);
	}
	else if (s_BoardType == btSquare16x16)
	{
		m_pSquareBoard = new CSudokuBoard(16);
	}
	m_pSudokuBoard = m_pSquareBoard;
    m_pSudokuBoard->CreateEmptyBoard();
    m_pSudokuBoard->SetBoardIndex(s_BoardType);
	m_BoardType = s_BoardType;
    m_pUndoList = new CUndoList;
    m_szPathName = "";
    m_bAnsichtMitKand = FALSE;
}

CSudokuSquareDoc::~CSudokuSquareDoc()
{
	delete m_pSquareBoard;
}

BOOL CSudokuSquareDoc::OnNewDocument()
{
	if (!CSudokuDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

// CSudokuSquareDoc-Serialisierung

void CSudokuSquareDoc::Serialize(CArchive& ar)
{
	CSudokuDoc::Serialize(ar);

	if (ar.IsLoading())
	{
		if (m_pSudokuBoard)
		{
			if (m_pSquareBoard)
			{
				delete m_pSquareBoard;
			}
			m_pSquareBoard = m_pSudokuBoard;
			m_szPathName = ar.GetFile()->GetFilePath();
			m_pSudokuBoard->InitSudokuField(m_pParent);
			m_pSudokuBoard->SwitchEditMode(true);
			m_pSudokuBoard->EnableBoard(true);
			static_cast<CSudokuMDIView*>(m_pParent)->ResizeFrameWindow();
			RefreshCandidates();
			m_pParent->Invalidate();
			m_pParent->UpdateWindow();
		}
		else
		{
			m_pSudokuBoard = m_pSquareBoard;
		}
	}
}


// CSudokuSquareDoc-Diagnose

#ifdef _DEBUG
void CSudokuSquareDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSudokuSquareDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSudokuSquareDoc-Befehle

void CSudokuSquareDoc::SetBoardType(BoardType_t boardType)
{
	s_BoardType = boardType;
}
