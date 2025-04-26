// SudokuMDI.h : Hauptheaderdatei für die SudokuMDI-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole

#define WM_NEW_NUMBER           WM_USER + 10
#define WM_MOVE_CURSOR          WM_USER + 11
//#define WM_ENABLE_SCAN_BUTTON   WM_USER + 12
#define WM_UPDATE_SCORE         WM_USER + 13
//#define WM_CHECK_SCAN_BUTTON    WM_USER + 14
//#define WM_SIZE_SCAN_BAR        WM_USER + 15
#define WM_NOTIFY_EDIT_FIELD    WM_USER + 16
#define WM_NOTIFY_SORT_BUTTON   WM_USER + 17
#define WM_SWITCH_BOARD         WM_USER + 18
#define WM_NOTIFY_MULTI_SELECT  WM_USER + 19

#define SUDOKU_HINDENT    100
#define SUDOKU_VINDENT     50
#define SUDOKU_INDENT_DP 1500
#define FIELD_EXT        50
#define FIELD_EXT_DP     1500
#define CAND_LEN         8
#define CAND_LEN_DP	     240

#define MOVE_DIR_RIGHT   1
#define MOVE_DIR_DOWN    2
#define MOVE_DIR_LEFT    3
#define MOVE_DIR_UP      4

typedef struct tagSudokuProps
{
    int			myVertIndent;
    int			myHoriIndent;
    int			myFieldSize;
    int			myCandLength;
    int			myVertIndentDP;
    int			myHoriIndentDP;
    int			myFieldSizeDP;
    int			myCandLengthDP;
    int			myFontSizeDP;
	int			myBoardAlignment;
	BOOL		myDisplayAlpha;
	COLORREF	myBackColor;
	COLORREF	myFillColor;
}
SudokuProps_t;

typedef enum tagBoardType
{
	btSquare9x9 = 0,
	btSquare12x12,
	btSquare16x16,
	btDuo,
	btSamurai,
    btSquare8x8
}
BoardType_t;

typedef enum tagMultiSelect
{
    msNone = 0,
    msPair,
    msTriple,
    msPairTriple
}
MultiSelect_t;

// CSudokuMDIApp:
// Siehe SudokuMDI.cpp für die Implementierung dieser Klasse
//

class CSudokuMDIApp : public CWinApp
{
public:
	CSudokuMDIApp();


// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnSquare8x8();
	afx_msg void OnSquare9x9();
	afx_msg void OnSquare12x12();
	afx_msg void OnSquare16x16();
	afx_msg void OnNeuDuo();
	afx_msg void OnNeuSamurai();
    afx_msg void OnFileOpen();
    afx_msg void OnDateiUninstall();
};

extern CSudokuMDIApp theApp;