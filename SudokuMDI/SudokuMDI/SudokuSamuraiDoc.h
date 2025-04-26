#pragma once
#include "SudokuDoc.h"

class CSudokuBoardSamurai;

// CSudokuSamuraiDoc document

class CSudokuSamuraiDoc : public CSudokuDoc
{
protected:
	CSudokuSamuraiDoc();
	DECLARE_DYNCREATE(CSudokuSamuraiDoc)

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

public:
	virtual ~CSudokuSamuraiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	CSudokuBoardSamurai* m_pSamuraiBoard;
};
