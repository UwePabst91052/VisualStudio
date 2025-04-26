#include "afxwin.h"
#include "scoresort.h"
#include "scoremultiselect.h"
#if !defined(AFX_SUDOKUSCORE_H__2F12AC81_FE7A_4760_B337_1466EB8F016A__INCLUDED_)
#define AFX_SUDOKUSCORE_H__2F12AC81_FE7A_4760_B337_1466EB8F016A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SudokuScore.h : Header-Datei
//

typedef struct tagDisplayLine
{
    UCHAR ucCand;
    int   nCount;
    bool  bCompleteFlag;
    bool  bFocus;
}
DisplayLine_t;

/////////////////////////////////////////////////////////////////////////////
// Fenster CSudokuScore 

class CSudokuScore : public CWnd
{
// Konstruktion
public:
	CSudokuScore();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSudokuScore)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void ResizeScoreWindow(int nCandComplete, int nBoardDim);
	void SelectCandidatesScan(int c, bool bSelect);
	int GetCount(int nIndex);
	void UpdateScore(int* pCandCount);
	virtual ~CSudokuScore();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CSudokuScore)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg LRESULT OnSortButtonClicked(WPARAM, LPARAM);
    afx_msg LRESULT OnMultiSelectButtonClicked(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
    bool m_bFocusIsOn;
	int myBoardDim;
	int myCandComplete;
	int m_nVertOffset;
    DisplayLine_t myDisplayLines[16];
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
private:
    // implements a bubble sort algorithm
    void BubbleSort(DisplayLine_t* pDisplayLines, int len);
    // implements quick sort algorithm
    void QuickSort(DisplayLine_t* pDisplayLines, int len);
    // helper function for quick sort function
    int Partion(DisplayLine_t* pDisplayLines, int len);

    void swap(DisplayLine_t& elem1, DisplayLine_t& elem2)
    {
        DisplayLine_t temp = elem1;
        elem1 = elem2;
        elem2 = temp;
    }
	BOOL myDisplayAlpha;
    BOOL m_bSortDisplayLines;
    BOOL m_bAllowMultiSelect;
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
    CBrush* m_pBrush;
    CRect  m_rcSortButton;
    CRect  m_rcMultiSelectButton;
public:
    CScoreSort  mySortCheckBox;
    CScoreMultiSelect myMultiSelectCheckBox;
    void UndoSort(DisplayLine_t* pDisplayLines, int len);
	void SetProperties(bool bDisplayAlpha);
private:
	COLORREF m_backColor;
	COLORREF m_fillColor;
public:
	void SetColors(COLORREF backColor, COLORREF fillColor);
	void GetColors(COLORREF& backColor, COLORREF& fillColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SUDOKUSCORE_H__2F12AC81_FE7A_4760_B337_1466EB8F016A__INCLUDED_
