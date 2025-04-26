#pragma once


// CSudokoEdit

typedef enum tagInputState
{
    isEmpty = 0,
    isEnteredOne,
    isValidNumber
}
InputState_t;

class CSudokoEdit : public CEdit
{
	DECLARE_SERIAL(CSudokoEdit)

public:
	CSudokoEdit();
	virtual ~CSudokoEdit();
	//{{AFX_VIRTUAL(CSudokuEdit)
    public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, int nBoardDim, UINT nId);
	//}}AFX_VIRTUAL

	void Serialize(CArchive& ar);


public:
	void SetProperties(BOOL bDisplayAlpha, CRect rcEdit, COLORREF backColor);
	void ShowWindow(int nCmdShow);
	void SignalNewNumber(int nNumber, bool bSolveActive);
	int myScanNumber;
	UINT m_nId;
	CRect m_rcClient;
	int myNumber;
	int myOldNumber;

protected:
	//{{AFX_MSG(CSudokuEdit)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWnd* m_pParent;
	bool m_bSelected;
	bool m_bSolveActive;
private:
	bool m_bSuspPaint;
	void EvaluateDigit(char* cDigit);

    InputState_t  myInputState;
    UINT          m_nTimerId;
    int           myBoardDim;
	BOOL		  m_bDisplayAlpha;
	COLORREF m_backColor;
};


