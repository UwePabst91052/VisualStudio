// SudokuDoc.h : Schnittstelle der Klasse CSudokuDoc
//


#pragma once
#include "sudokuboard.h"
#include "SudokuField.h"
#include "UndoList.h"

class CSudokuDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CSudokuDoc();
	DECLARE_DYNCREATE(CSudokuDoc)

// Attribute
public:

// Operationen
public:

// Überschreibungen
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CSudokuDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void DrawSudokuBoard(CDC* pDC);
	void InitSudokoField(CWnd* pParent);
	void SetSudokoFieldNumber(CPoint ptId, int nNumber);
	void DeleteCandidates(int xPos, int yPos, int nNumber);
	void RefreshCandidates(void);
	void PrintSudokuBoard(CDC* pDC);
	void MoveCursor(CPoint ptId, int iMoveDir);
	void EvalMousePosition(CPoint ptPos);
    BoardType_t GetBoardType();
	int GetCurBoardDim();
	void ToggleCandidate(CPoint ptPos);
	void SelectScan(int c, bool bSelect, BOOL bAllowMulti);
    void SetBoardProperties(SudokuProps_t props);
    void GetBoardProperties(SudokuProps_t& props);
	void GetBoardDimensions(CRect&);
protected:
	bool m_bMark;
	CSudokuBoard* m_pSudokuBoard;
    BoardType_t   m_BoardType;
	CUndoList*    m_pUndoList;
    CString       m_szPathName;
	CWnd*		  m_pParent;
	int			  m_nSingleInFields;
	int           m_nSingleOnBoard;
	void ReDoAllActions();
	BOOL m_bEditMode;
    BOOL m_bAktionSingleInField;
	BOOL m_bAktionSingle;
	BOOL m_bAktionPair;
    BOOL m_bAktionTriple;
	BOOL m_bAnsichtMitKand;
	BOOL m_bBeginTrial;
	BOOL m_bUndoTrial;
	BOOL m_bEditUndo;
	BOOL m_bEditRedo;
	BOOL m_bEditRedoAll;
    MultiSelect_t m_msMode;
protected:
// Generierte Funktionen für die Meldungstabellen
    afx_msg void OnAktionenSingleInField();
	afx_msg void OnAktionenSelectsingle();
	afx_msg void OnAktionenSelectpair();
	afx_msg void OnAktionenSolveLevel1();
	afx_msg void OnAktionenSolveLevel2();
    afx_msg void OnUpdateAktionenSingleInField(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAktionenSelectpair(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAktionenSelectsingle(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedoAll();
	afx_msg void OnUpdateEditRedoAll(CCmdUI *pCmdUI);
	afx_msg void OnUndoTrial();
	afx_msg void OnUpdateUndoTrial(CCmdUI *pCmdUI);
	afx_msg void OnBeginTrial();
	afx_msg void OnUpdateBeginTrial(CCmdUI *pCmdUI);
	afx_msg void OnEditMode();
	afx_msg void OnUpdateEditMode(CCmdUI *pCmdUI);
	afx_msg void OnAnsichtMitkandidaten();
	afx_msg void OnUpdateAnsichtMitkandidaten(CCmdUI *pCmdUI);
	afx_msg void OnMoveDown();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveUp();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateAktionenSolveLevel1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAktionenSolveLevel2(CCmdUI *pCmdUI);
    afx_msg void OnAktionenSelecttriple();
    afx_msg void OnUpdateAktionenSelecttriple(CCmdUI *pCmdUI);
protected:
	COLORREF m_backColor;
	COLORREF m_scoreColor;
};


