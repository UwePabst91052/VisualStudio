// ScoreMultiSelect.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "ScoreMultiSelect.h"


// CScoreMultiSelect

IMPLEMENT_DYNAMIC(CScoreMultiSelect, CButton)

CScoreMultiSelect::CScoreMultiSelect()
{

}

CScoreMultiSelect::~CScoreMultiSelect()
{
}


BEGIN_MESSAGE_MAP(CScoreMultiSelect, CButton)
    ON_CONTROL_REFLECT(BN_CLICKED, &CScoreMultiSelect::OnBnClicked)
END_MESSAGE_MAP()



// CScoreMultiSelect message handlers



void CScoreMultiSelect::OnBnClicked()
{
    GetParent()->SendMessage(WM_NOTIFY_MULTI_SELECT); 
}
