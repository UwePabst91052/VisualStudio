#pragma once
#include "afxcoll.h"
#include "SudokuField.h"

#define CALC_MODULO					\
    if (myBoardDim == 16)			\
    {								\
        nVertMod = 4;				\
        nHoriMod = 4;				\
    }								\
    else							\
    {								\
        int factor;                 \
        if (myBoardDim % 3)         \
            factor = 2;             \
        else                        \
            factor = 3;             \
		if(!myBoardAlignment)		        \
		{							        \
			nVertMod = factor;			    \
			nHoriMod = myBoardDim / factor;	\
		}							        \
		else						        \
		{							        \
			nVertMod = myBoardDim / factor;	\
			nHoriMod = factor;			    \
		}							        \
    }								

typedef enum VectorResultTag
{
	vrInvalid = 0,
	vrValidSingle,
	vrValidDouble
}
VectorResult_t;

typedef enum MoveDirectionTag
{
	moveUp = 0,
	moveDown,
	moveLeft,
	moveRight
}
MoveDirection_t;

class CCandCounter
{
public:
    // construction / destruction
    CCandCounter(int nBoardDim)
        :mySingleArray(NULL)
    {
        myBoardDim = nBoardDim;
        mySingleArray = new UCHAR[nBoardDim * nBoardDim];
    }
    ~CCandCounter()
    {
        if (mySingleArray) delete [] mySingleArray;
    }

    // member functions
    void SetSingle(UINT nID)
    {
        int x = LOWORD(nID);
        int y = HIWORD(nID);
        UCHAR* pElement = mySingleArray + y * myBoardDim + x;
        *pElement = 1;
    }
    void Reset()
    {
        memset (mySingleArray, 0, myBoardDim * myBoardDim);
    }
    void ClearSingleFields(CCandCounter* pSingleFields)
    {
        UCHAR* pElement = mySingleArray;
        UCHAR* pClear   = pSingleFields->mySingleArray;
        for (int y=0; y<myBoardDim; y++)
        {
            for (int x=0; x<myBoardDim; x++, pElement++, pClear++)
            {
                if (*pElement && *pClear)
                {
                    *pElement = 0;
                }
            }
        }
    }
    int GetSingleCount()
    {
        UCHAR* pElement = mySingleArray;
        int nCount = 0;
        for (int y=0; y<myBoardDim; y++)
        {
            for (int x=0; x<myBoardDim; x++, pElement++)
            {
                nCount += *pElement;
            }
        }
        return nCount;
    }

private:
    int    myBoardDim;
    UCHAR* mySingleArray;
};

// CSudokuBoard-Befehlsziel

class CSudokuBoard : public CObject
{
protected:
    DECLARE_SERIAL(CSudokuBoard)
    CSudokuBoard();
public:
	CSudokuBoard(int nBoardDim);
	virtual ~CSudokuBoard();

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
    int GetBoardDim() { return myBoardDim; }
	virtual bool IsNumberSelectedSingle(CPoint ptId, int nNumber);
	virtual bool IsDuplicate(CPoint ptId, int nNumber);
	virtual bool IsSingleInFields(int& nCount);
	virtual bool IsSingleOnBoard(int& nCount);

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
    virtual void SetBoardProperties(SudokuProps_t props);
    virtual void GetBoardProperties(SudokuProps_t& props);

	// serializing, printing and drawing functions
	virtual void Serialize(CArchive& ar);
	virtual void DrawSudokuBoard(CDC* pDC);
	virtual void PrintSudokuBoard(CDC* pDC);

protected:
	// internal helper functions
    void FinalConstruction();
	void AddSudokuField(CSudokuField* aField, int iHoriPos, int iVertPos);
	void SelectSingles(CObArray* pFieldArray, BOOL bSelect);
	bool SolveInArray(CObArray* pFieldArray);
	bool IsDuplicate(CObArray& pFieldArray , int nOwnPos, int nNumber);
	void FindSingleOnBoard(CObArray* pFieldArray);
	void DeleteCandidates(CObArray& pFieldArray, int nNumber);
    void DeleteSudokuFields();
	BOOL CheckNumberCompletion(int nNumber);

	// working members
	int m_iLastVertPos;
	int m_iLastHoriPos;
	int myBoardIndex;
	CWnd* myParent;
    bool m_bConstructed;
	int* myCandCounts;
    BOOL*  arrDisable;
    CCandCounter*  myCandCounter;
    CCandCounter*  mySingleCounter;

	// serializable members
	CObArray* mySudokuSquares;
	CObArray* mySudokuColumns;
	CObArray* mySudokuRows;
    int myBoardDim;
	int myHoriIndent;
	int myHoriIndentDP;
	int myVertIndent;
	int myVertIndentDP;
	int myFieldExt;
	int myFieldExtDP;
	int myCandLen;
	int myCandLenDP;
    int myFontSizeDP;
	int myBoardAlignment;
	BOOL myDisplayAlpha;
public:
	void UpdateAlignment(int nNewAlign);
protected:
	COLORREF m_backColor;
};


