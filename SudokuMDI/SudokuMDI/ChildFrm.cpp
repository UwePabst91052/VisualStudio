// ChildFrm.cpp : Implementierung der Klasse CChildFrame
//
#include "stdafx.h"
#include "SudokuMDI.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame-Erstellung/Zerstörung

CChildFrame::CChildFrame()
{
	// TODO: Hier Code für die Memberinitialisierung einfügen
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie die Fensterklasse oder die Stile hier, indem Sie CREATESTRUCT ändern
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame-Diagnose

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame-Meldungshandler
