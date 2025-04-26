// SudokuMDIView.h : Schnittstelle der Klasse CSudokuMDIView
//


#pragma once
#include "sudokufield.h"
#include "SudokuScore.h"	// Hinzugefügt von der Klassenansicht
#include "MousePad.h"
#include "sudokuproperties.h"
#include "CandDisplay.h"

class CSudokuDoc;

class CSudokuMDIView : public CView
{
protected: // Nur aus Serialisierung erstellen
	CSudokuMDIView();
	DECLARE_DYNCREATE(CSudokuMDIView)

// Attribute
public:
	CSudokuDoc* GetDocument() const;
    CSudokuScore* GetSudokuScore() {return &mySudokuScore;} 

// Vorgänge
public:

// Überschreibungen
public:
	virtual void OnDraw(CDC* pDC);  // Überschrieben, um diese Ansicht darzustellen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementierung
public:
	void UpdateCandDisplay(int nSingleInFields, int nSingleOnBoard);
	void SwitchBoardFormat(int nIndex, int nBoardDim);
	virtual ~CSudokuMDIView();
	void ResizeFrameWindow(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg void OnBearbeitenEigenschaften();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAnsichtRefresh();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNewNumber(WPARAM, LPARAM);
	afx_msg LRESULT OnMoveCursor(WPARAM, LPARAM);
    afx_msg LRESULT OnUpdateScore(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
    int    myBoardDim;
	// current mouse position
	CPoint myMousePos;
	CSudokuScore mySudokuScore;
	CCandDisplay myCandDisplay;
	COLORREF m_backColor;
	COLORREF m_fillColor;
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // Debugversion in SudokuMDIView.cpp
inline CSudokuDoc* CSudokuMDIView::GetDocument() const
   { return reinterpret_cast<CSudokuDoc*>(m_pDocument); }
#endif

