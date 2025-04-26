// ScoreSort.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "ScoreSort.h"


// CScoreSort

IMPLEMENT_DYNAMIC(CScoreSort, CButton)

CScoreSort::CScoreSort()
{

}

CScoreSort::~CScoreSort()
{
}


BEGIN_MESSAGE_MAP(CScoreSort, CButton)
    ON_CONTROL_REFLECT(BN_CLICKED, &CScoreSort::OnBnClicked)
END_MESSAGE_MAP()



// CScoreSort message handlers



void CScoreSort::OnBnClicked()
{
    GetParent()->SendMessage(WM_NOTIFY_SORT_BUTTON); 
}
