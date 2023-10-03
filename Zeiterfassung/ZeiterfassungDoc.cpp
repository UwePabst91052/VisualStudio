// ZeiterfassungDoc.cpp : Implementierung der Klasse CZeiterfassungDoc
//

#include "stdafx.h"
#include "Zeiterfassung.h"

#include "ZeiterfassungDoc.h"
#include "ZeiterfassungView.h"
#include "Workpackage.h"
#include "XmlDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZeiterfassungDoc

IMPLEMENT_DYNCREATE(CZeiterfassungDoc, CDocument)

BEGIN_MESSAGE_MAP(CZeiterfassungDoc, CDocument)
    ON_COMMAND(ID_BEARBEITEN_LOESCHE, &CZeiterfassungDoc::OnBearbeitenLoesche)
END_MESSAGE_MAP()


// CZeiterfassungDoc-Erstellung/Zerstörung

CZeiterfassungDoc::CZeiterfassungDoc()
: m_tmpDate(0)
, m_edit_mode(false)
{
	SYSTEMTIME	sysTime;
	m_date = CFileTime::GetCurrentTime();
	m_date.GetAsSystemTime(sysTime);
	sysTime.wHour = 0;
	sysTime.wMinute = 0;
	sysTime.wSecond = 0;
	sysTime.wMilliseconds = 0;
	CTime date(sysTime);
	m_date = date;
}

CZeiterfassungDoc::~CZeiterfassungDoc()
{
    ResetDocument();
}

BOOL CZeiterfassungDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    ResetDocument();

	return TRUE;
}

HANDLE CZeiterfassungDoc::AddWorkpackage(CString &text)
{
	SetModifiedFlag();
	CWorkpackage* wp = new CWorkpackage(text, m_date);
	POSITION pos = m_workpackages.AddTail(wp);
	return pos;
}

void CZeiterfassungDoc::DeleteWorkpackage(HANDLE hwp)
{
	SetModifiedFlag();
	CWorkpackage* wp;
	POSITION pos = (POSITION)hwp;
	wp = m_workpackages.GetAt(pos);
	m_workpackages.RemoveAt(pos);
	delete wp;

}

bool CZeiterfassungDoc::GetWorkpackage(HANDLE hwp, CWorkpackage *&wpckg)
{
    if (NULL == hwp) return false;
	wpckg = m_workpackages.GetAt((POSITION)hwp);
	return (NULL != wpckg);
}

void CZeiterfassungDoc::BeginLogging(HANDLE hwp)
{
    if (m_edit_mode) return;

	POSITION pos = (POSITION)hwp;
    CTime today = GetDateOfDateTime(CTime::GetCurrentTime());
    if (m_date != today) return;
	CWorkpackage* wp = m_workpackages.GetAt(pos);
	wp->setStartTime(CTime::GetCurrentTime());
	CZeiterfassungView* pView = (CZeiterfassungView*)m_viewList.GetHead();
	pView->UpdateWpTimes(WPTM_START, hwp);
	SetModifiedFlag();
}

void CZeiterfassungDoc::EndLogging(HANDLE hwp)
{
    if (m_edit_mode) return;

	POSITION pos = (POSITION)hwp;
	UINT mask = WPTM_DURATION;
    CTime today = GetDateOfDateTime(CTime::GetCurrentTime());
    if (m_date != today) return;
	CWorkpackage* wp = m_workpackages.GetAt(pos);
	mask |= wp->isWorktimeRunning(m_date) ? (WPTM_END | WPTM_SUM) : 0;
	wp->setEndTime(CTime::GetCurrentTime());
	CZeiterfassungView* pView = (CZeiterfassungView*)m_viewList.GetHead();
	pView->UpdateWpTimes(mask, hwp);
	SetModifiedFlag();
}

bool CZeiterfassungDoc::PauseLogging(HANDLE hwp)
{
	POSITION pos = (POSITION)hwp;
	CWorkpackage* wp = m_workpackages.GetAt(pos);
	CZeiterfassungView* pView = (CZeiterfassungView*)m_viewList.GetHead();
	if (wp->isWorktimeRunning(m_date))
	{
		wp->setEndTime(CTime::GetCurrentTime());
		pView->UpdateWpTimes(WPTM_END | WPTM_DURATION | WPTM_SUM, hwp);
	}
	else
	{
		wp->setStartTime(CTime::GetCurrentTime());
		pView->UpdateWpTimes(WPTM_START, hwp);
	}
	pView->SelectWorkpackage(hwp);
    UpdateAllViews((CView*)m_viewList.GetHead(), (LPARAM)hwp);
	SetModifiedFlag();
    return wp->isWorktimeRunning(m_date);
}

// CZeiterfassungDoc-Serialisierung

void CZeiterfassungDoc::Serialize(CArchive& ar)
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
        ResetDocument();
        ar >> count_wp;
        if (true)
        {
		    for (int i = 0; i < count_wp; i++)
		    {
			    ar >> wp;
			    m_workpackages.AddTail(wp);
		    }
        }
        else
        {
		    for (int i = 0; i < count_wp; i++)
		    {
                wp = new CWorkpackage;
                wp->Serialize(ar);
			    m_workpackages.AddTail(wp);
		    }
        }
	}
	UpdateAllViews((CView*)m_viewList.GetTail());
}



// CZeiterfassungDoc-Diagnose

#ifdef _DEBUG
void CZeiterfassungDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZeiterfassungDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CZeiterfassungDoc-Befehle

void CZeiterfassungDoc::SetCurrentDate(CTime time)
{
	SYSTEMTIME	sysTime;
	m_date = time;
	m_date.GetAsSystemTime(sysTime);
	sysTime.wHour = 0;
	sysTime.wMinute = 0;
	sysTime.wSecond = 0;
	sysTime.wMilliseconds = 0;
	CTime date(sysTime);
	m_date = date;
}

CWorkpackage* CZeiterfassungDoc::GetWorkpackages(POSITION& pos, HANDLE& hwp)
{
	CWorkpackage* wp = NULL;
	
	if (NULL == pos)
	{
		pos = m_workpackages.GetHeadPosition();
	}

	if (NULL != pos)
	{
		hwp = (HANDLE)pos;
		wp = m_workpackages.GetNext(pos);
	}

	return wp;
}

void CZeiterfassungDoc::StoreWorkpackagesXml(CArchive & ar)
{
	POSITION pos;
	CWorkpackage* wp;
	CXmlDocument xmlDoc(ar);
	if (ar.IsStoring())
	{
		pos = m_workpackages.GetHeadPosition();
		while (NULL != pos)
		{
			wp = m_workpackages.GetNext(pos);
			wp->serializeXml(ar, xmlDoc);
		}
	}
	else
	{
			
	}
}


CTime& CZeiterfassungDoc::GetDateOfDateTime(CTime& dateTime)
{
    SYSTEMTIME sysTime;

    memset(&sysTime, 0, sizeof(SYSTEMTIME));
    sysTime.wDay = dateTime.GetDay();
    sysTime.wDayOfWeek = dateTime.GetDayOfWeek();
    sysTime.wMonth = dateTime.GetMonth();
    sysTime.wYear = dateTime.GetYear();

    CTime tmpDate(sysTime);
    m_tmpDate = tmpDate;

    return m_tmpDate;
}

void CZeiterfassungDoc::SetEditMode(bool bEditMode)
{
    m_edit_mode = bEditMode;
}

void CZeiterfassungDoc::GetWorkdayLimits(CTime& firstDay, CTime& lastDay)
{
    CTime firstWorkday,
          lastWorkday;
    CTime retDay;
    CWorkpackage* wp;
    POSITION pos = m_workpackages.GetHeadPosition();
    while (NULL != pos)
    {
        wp = m_workpackages.GetNext(pos);
        retDay = wp->GetFirstDay();
        if ((retDay < firstWorkday) ||
            (firstWorkday == 0))
        {
            firstWorkday = retDay;
        }
        retDay = wp->GetLastDay();
        if (retDay > lastWorkday)
        {
            lastWorkday = retDay;
        }
    }
    firstDay = firstWorkday;
    lastDay = lastWorkday;
}

void CZeiterfassungDoc::ResetDocument(void)
{
	CWorkpackage* wp;
	POSITION pos = m_workpackages.GetHeadPosition();
	while (NULL != pos)
	{
		wp = m_workpackages.GetNext(pos);
		delete wp;
	}
    m_workpackages.RemoveAll();
}

int CZeiterfassungDoc::GetCountLinesDoc(void)
{
    CWorkpackage* wp = NULL;
    int countLines = 0;

    POSITION pos = m_workpackages.GetHeadPosition();
    while (NULL != pos)
    {
        wp = m_workpackages.GetNext(pos);
        countLines += wp->GetCountLines();
    }
    countLines += m_workpackages.GetCount() * 2;

    return countLines;
}

void CZeiterfassungDoc::PrintHeader(CDC* pDC, CPoint& pos)
{
    CString strHeader;
    CTime dateFirst;
    CTime dateLast;
    CString strFirstDay;
    CString strLastDay;

    GetWorkdayLimits(dateFirst, dateLast);
    strFirstDay = dateFirst.Format(_T("%d.%m.%Y"));
    strLastDay = dateLast.Format(_T("%d.%m.%Y"));
    strHeader.Format(_T("Zeiterfassung: %s vom %s bis %s"), GetTitle(), strFirstDay, strLastDay);
    pDC->TextOutW(pos.x, pos.y, strHeader);
}

void CZeiterfassungDoc::PrintFooter(CDC* pDC, 
                                    CString& fmt, 
                                    CPoint& pos, 
                                    int curPage, 
                                    int fromPage, 
                                    int maxPage)
{
    CString strFooter;

    strFooter.Format(fmt, curPage, fromPage, maxPage);
    pDC->TextOutW(pos.x, pos.y, strFooter);
}

void CZeiterfassungDoc::OnBearbeitenLoesche()
{
    POSITION pos = NULL;
    CWorkpackage* wp = NULL;

    pos = m_workpackages.GetHeadPosition();
    while (NULL != pos)
    {
        wp = m_workpackages.GetNext(pos);
        wp->DeleteAllNotes();
    }
}

CTimeSpan& CZeiterfassungDoc::GetWorkdaySum()
{
    POSITION pos = NULL;
    CWorkpackage* wp = NULL;
    CWorkday* wd = NULL;
    m_tmpDuration = 0;

    pos = m_workpackages.GetHeadPosition();
    while (NULL != pos)
    {
        wp = m_workpackages.GetNext(pos);
        m_tmpDuration += wp->getDurationPerDay(m_date);
    }
    return m_tmpDuration;
}

int CZeiterfassungDoc::GetCountWorkpackage(void)
{
    return m_workpackages.GetCount();
}
