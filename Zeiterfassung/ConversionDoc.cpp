// ConversionDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Zeiterfassung.h"
#include "ConversionDoc.h"
//#include "ZeiterfassungView.h"
#include "Workpackage.h"


// CConversionDoc

IMPLEMENT_DYNCREATE(CConversionDoc, CZeiterfassungDoc)

CConversionDoc::CConversionDoc()
: CZeiterfassungDoc()
{
}

BOOL CConversionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CConversionDoc::~CConversionDoc()
{
	CWorkpackage* wp;
	POSITION pos = m_workpackages.GetHeadPosition();
	while (NULL != pos)
	{
		wp = m_workpackages.GetNext(pos);
		delete wp;
	}
}


BEGIN_MESSAGE_MAP(CConversionDoc, CDocument)
END_MESSAGE_MAP()


// CConversionDoc diagnostics

#ifdef _DEBUG
void CConversionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CConversionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CConversionDoc serialization

void CConversionDoc::Serialize(CArchive& ar)
{
	POSITION pos;
	CWorkpackage* wp;
	int count_wp;
	if (ar.IsStoring())
	{
		count_wp = m_workpackages.GetCount();
		ar << count_wp;
		pos = m_workpackages.GetHeadPosition();
		while (NULL != pos)
		{
			wp = m_workpackages.GetNext(pos);
			ar << wp;
		}
	}
	else
	{
        ar >> count_wp;
	    for (int i = 0; i < count_wp; i++)
	    {
            wp = new CWorkpackage;
            wp->Serialize(ar);
		    m_workpackages.AddTail(wp);
	    }
	}
	//UpdateAllViews((CView*)m_viewList.GetTail());
}
#endif


// CConversionDoc commands
