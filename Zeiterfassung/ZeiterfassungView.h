// ZeiterfassungView.h : Schnittstelle der Klasse CZeiterfassungView
//


#pragma once
#include "notedialog.h"

#define WPTM_START		0x0001
#define WPTM_END		0x0002
#define WPTM_DURATION	0x0004
#define WPTM_SUM        0x0008

class CZeiterfassungView : public CListView
{
protected: // Nur aus Serialisierung erstellen
	CZeiterfassungView();
	DECLARE_DYNCREATE(CZeiterfassungView)

// Attribute
public:
	CZeiterfassungDoc* GetDocument() const;

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // Erster Aufruf nach Erstellung

// Implementierung
public:
	virtual ~CZeiterfassungView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void AddWorkPackage(HANDLE hwp, CWorkpackage* wp, CTime date);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDateiKonvertieren();
private:
	int m_item;
    int m_startLinePage;
    bool m_sumLineActive;
public:
	void UpdateWpTimes(UINT mask, HANDLE hwp);
	void SelectWorkpackage(HANDLE hwp);
protected:
    virtual void OnDraw(CDC* /*pDC*/);
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
    void AddSumLine();
    void UpdateSumLine(CTimeSpan& sumDuration);
};

#ifndef _DEBUG  // Debugversion in ZeiterfassungView.cpp
inline CZeiterfassungDoc* CZeiterfassungView::GetDocument() const
   { return reinterpret_cast<CZeiterfassungDoc*>(m_pDocument); }
#endif

