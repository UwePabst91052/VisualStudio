#pragma once

#include ".\sudokoedit.h"

typedef enum tagDispMode
{
	None = 0,
    SingleIF,
	Single,
	Pair,
    Triple,
	Scan
}
DispMode_t;

typedef struct tagCandidate
{
	bool  bVisible;
	bool  bForced;
	CPoint ptFrom;
	CPoint ptTo;
	CPoint ptFromDP;
	CPoint ptToDP;
	CRect rcFrame;
	bool  bHover;
	DispMode_t  dispMode;
}
Candidate_t;

// CSudokuField2-Befehlsziel

class CSudokuField : public CObject
{
    DECLARE_SERIAL(CSudokuField)
public:
	void EnableField(bool bEnable);
	void CleanCandHover();
	void WipeField();
	CSudokuField(CPoint ptPos, CPoint ptPosDP, 
				 int nBoardDim, 
				 int iDim, int iDimDP, 
				 int iCandLen, int iCandLenDP, int iFontSizeDP);
	CSudokuField(CSudokuField& fromField);
	virtual ~CSudokuField();
    void FinalConstruction();
	void InitSudokoEdit(CWnd* pParent, CPoint ptId);
	void MoveSudokuField(CPoint ptId);
	void SetCandidate(int iCand);
	void DeleteCandidate(int iCand);
	void DrawCandidates(CDC* pDC);
	void ResetCandidates();
	bool SelectSingle(BOOL bSelect);
	bool SelectMulti(MultiSelect_t msMode);
	void ToggleCandidate(CPoint ptPos, int& number);
	void ToggleCandidate(int number);
	bool IsCandidateOn(int iCand);
	void SelectCandidate(int iCand, BOOL bSelect);
	void SelectCandidateScan(int c, bool bSelect, BOOL bAllowMulti);
	void ShowHideEditField(bool bShow);
	void SetEditMode(bool bEditMode);
	bool IsNumberSelectedSingle(int nNumber);
	virtual void Serialize(CArchive& ar);
	void EvalMousePosition(CPoint ptPos);
	void PrintSudokuField(CDC* pDC);
    void UpdateLayout(CPoint ptPos, CPoint ptPosDP,
                      int iDim, int iDimDP, 
				      int iCandLen, int iCandLenDP, 
                      int iFontSizeDP, BOOL bDisplayAlpha,
					  COLORREF backColor);
	CSudokuField& operator&(CSudokuField& Right);
	CSudokuField& operator=(CSudokuField& Right);

	CSudokoEdit*  m_pFieldEdit;
protected:
    CSudokuField();

protected:
    int          myBoardDim;
	int			 myDim;
	int			 myDimDP;
	int			 myCandLen;
	int			 myCandLenDP;
    int          myFontSizeDP;
	BOOL		 myDisplayAlpha;
	CPoint		 ptFieldPos;
	CPoint		 ptFieldPosDP;
	Candidate_t* pCandidates;
    bool  m_bShowCandidates;
	void InitCandidates(void);
	CRect m_rcNumber;
private:
	CWnd* m_pParent;
	bool  m_bEditMode;
public:
	// Schaltet die Kandidatenanzeige an oder aus
	void ShowCandidates(bool bShowCandidates);
	void SetNumber(int nNumber);
	bool SetSingleNumber(void);
	bool IsSingleInField(void);
};


