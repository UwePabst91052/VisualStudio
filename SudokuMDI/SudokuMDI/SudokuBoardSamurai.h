#pragma once
#include "sudokuboard.h"

class CSudokuBoardSamurai :
	public CSudokuBoard
{
    DECLARE_SERIAL(CSudokuBoardSamurai)
public:
	CSudokuBoardSamurai(void);
	CSudokuBoardSamurai(int nBoardDim);
	virtual ~CSudokuBoardSamurai(void);

	// Init functions
    virtual void CreateEmptyBoard();
    virtual void WipeBoard();
	virtual void EnableBoard(bool bEnable);
	virtual void CleanHover();
	virtual void RefreshCandidates();
	virtual void UpdateNumberCompletion();
	virtual void ResetCandidates();
	virtual void InitSudokuField(CWnd* pParent);
    virtual void UpdateBoardLayout();

	// query functions
	virtual int GetBoardIndex();
	virtual void GetMovedCursor(CPoint& ptId, int iMoveDir);
	virtual CSudokuField* GetSudokuField(CPoint ptId);
	virtual int GetFieldNumberOld(CPoint ptId);
	virtual int GetFieldNumber(CPoint ptId);
	virtual bool IsNumberSelectedSingle(CPoint ptId, int nNumber);
	virtual bool IsDuplicate(CPoint ptId, int nNumber);
	virtual bool IsSingleInFields(int &nCount);
	virtual bool IsSingleOnBoard(int &nCount);

	// action and set functions
	virtual void SetBoardIndex(int nIndex);
	virtual void SelectMulti(MultiSelect_t msMode);
	virtual void SelectSingleInField(BOOL bSelect);
	virtual void SelectSingles(BOOL bSelect);
	virtual bool SolveLevel1(void);
	virtual bool SolveLevel2(void);
	virtual void SwitchEditMode(bool bEditMode);
	virtual void SelectScan(int c, bool bSelect, BOOL bAllowMulti);
	virtual void SetEditFocus(CPoint ptId);
	virtual void SetFieldNumber(CPoint ptId, int nNumber);
	virtual void DeleteCandidates(int xPos, int yPos, int nNumber);
	// Schaltet die Kandidatenanzeige an oder aus
	virtual void ShowCandidates(bool bShowCandidates);
	virtual void EvalMousePosition(CPoint ptPos);
	virtual bool ToggleCandidate(CPoint ptPos, int& iHoriPos, int& iVertPos, int& nNumber);
	virtual void ToggleCandidate(CPoint ptId, int nNumber);
	virtual void MoveBoard(MoveDirection_t moveDir, int nMoveValue);

	// serializing, printing and drawing functions
	virtual void Serialize(CArchive& ar);
	virtual void DrawSudokuBoard(CDC* pDC);
	virtual void PrintSudokuBoard(CDC* pDC);

protected:
	// internal helper functions
    void FinalConstruction();
	bool AddSudokuField(CSudokuField* aField, int iHoriPos, int iVertPos);
	void SelectSingles(CObArray* pFieldArray, int nDim, BOOL bSelect);
	bool SolveInArray(CObArray* pFieldArray, int nDim);
	void FindSingleOnBoard(CObArray* pFieldArray, int nDim);
    void DeleteSudokuFields();
	BOOL CheckNumberCompletion(int nNumber);

	// serializable members
	CObArray* mySudokuUnions;


private:
	// helper functions
	VectorResult_t BoardToVector(CPoint ptBoard, CPoint* ptVect);
	int BoardToUnion(CPoint ptBoard, int& nOwnPos);
	bool IsValidField(CPoint ptBoard);
	CSudokuField* GetDisplayField(CPoint ptBoard);
};
