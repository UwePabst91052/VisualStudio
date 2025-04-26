#pragma once
#include "afxwin.h"
#include "ShowColor.h"

// CSudokuProperties dialog

class CSudokuProperties : public CPropertyPage
{
	DECLARE_DYNAMIC(CSudokuProperties)

public:
	CSudokuProperties(UINT idCaption);
	virtual ~CSudokuProperties();

// Dialog Data
	enum { IDD = IDD_SUDOKUPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    int m_nHoriIndent;
    int m_nVertIndent;
    int m_nFieldSize;
    int m_nCandLength;
    int m_nHoriIndentDP;
    int m_nVertIndentDP;
    int m_nFieldSizeDP;
    int m_nCandLengthDP;
    int m_nFontSizeDP;
	COLORREF m_backColor;
	COLORREF m_fillColor;
    void GetBoardProperties(SudokuProps_t& props);
    void SetBoardProperties(SudokuProps_t  props);
	virtual BOOL OnInitDialog();
	// select Alignment Horizontal/Vertical for odd square sudokus
	CComboBox m_BoardAlignment;
	// display Alphanumeric
	BOOL m_bDisplayAlpha;
	int m_nBoardAlignment;
	CStatic m_LabelAlignment;
	afx_msg void OnEnChangeEditProperty(UINT nId);
	afx_msg void OnBnClickedCheckAlpha();
	afx_msg void OnCbnSelchangeComboAlign();
	afx_msg void OnBnClickedButtonColorView();
	afx_msg void OnBnClickedButtonColorScore();
	CShowColor myViewColor;
	CShowColor myScoreColor;
};
