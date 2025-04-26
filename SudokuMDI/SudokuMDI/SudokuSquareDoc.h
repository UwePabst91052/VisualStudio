// SudokuSquareDoc.h : Schnittstelle der Klasse CSudokuSquareDoc
//


#pragma once
#include "SudokuDoc.h"

class CSudokuSquareDoc : public CSudokuDoc
{
protected: // Nur aus Serialisierung erstellen
	CSudokuSquareDoc();
	DECLARE_DYNCREATE(CSudokuSquareDoc)

// Attribute
public:

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	static void SetBoardType(BoardType_t boardType);

// Implementierung
public:
	virtual ~CSudokuSquareDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
private:
	static BoardType_t s_BoardType;
	CSudokuBoard* m_pSquareBoard;
};


