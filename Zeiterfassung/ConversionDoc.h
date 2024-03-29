#pragma once
#include "ZeiterfassungDoc.h"

class CWorkpackage;

// CConversionDoc document

class CConversionDoc : public CZeiterfassungDoc
{
protected:
	CConversionDoc();
	DECLARE_DYNCREATE(CConversionDoc)

private:
    CList<CWorkpackage*> m_workpackages;

public:
	virtual ~CConversionDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
