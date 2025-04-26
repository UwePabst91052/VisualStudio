#pragma once
#include "SudokuDoc.h"

class CSudokuBoardDuo;

// CSudokuDuoDoc document

class CSudokuDuoDoc : public CSudokuDoc
{
protected:
	CSudokuDuoDoc();
	DECLARE_DYNCREATE(CSudokuDuoDoc)

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

public:
	virtual ~CSudokuDuoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	CSudokuBoardDuo* m_pDuoBoard;
};
