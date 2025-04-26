// MousePad.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "MousePad.h"


// CMousePad

IMPLEMENT_DYNAMIC(CMousePad, CToolBar)

CMousePad::CMousePad()
: m_pEditField(NULL)
{
	m_cxLeftBorder = 5;
	m_cyTopBorder = 5;
	m_cxRightBorder = 5;
	m_cyBottomBorder = 5;
}

CMousePad::~CMousePad()
{
}


BEGIN_MESSAGE_MAP(CMousePad, CToolBar)
END_MESSAGE_MAP()



// CMousePad message handlers



void CMousePad::InitLayout(void)
{
	int nCount = GetToolBarCtrl().GetButtonCount();
	int nMod = (nCount == 16 ? 4 : 3);

	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % nMod) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		SetButtonStyle(i, nStyle);
	}

	Invalidate();
	GetParentFrame()->RecalcLayout();
}

void CMousePad::UpdateEditField(CWnd* pWnd)
{
	m_pEditField = pWnd;
}

void CMousePad::MousePadClicked(UINT nKeyCode)
{
	if (m_pEditField)
	{
		m_pEditField->SendMessage(WM_CHAR, (WPARAM)nKeyCode, 1);
	}
}
