// Workpackage.cpp

#include "stdafx.h"
#include "Zeiterfassung.h"

#include "Workpackage.h"
#include "XmlDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CWorkday, CObject, VERSIONABLE_SCHEMA | 2)

// CWorkday implementation
CWorkday::CWorkday(CTime& date)
: m_date(date)
, m_isRunning(false)
{
}

CWorkday::~CWorkday()
{
    WorkTime_t* wt;
    POSITION pos = NULL;
    pos = m_workTimes.GetHeadPosition();
    while (NULL != pos)
    {
        wt = m_workTimes.GetNext(pos);
        delete wt;
    }
    m_workTimes.RemoveAll();
}

void CWorkday::setStartTime(ATL::CTime &time)
{
    m_curTime.startTime = time;
    m_curTime.endTime = 0;
    m_curTime.duration = 0;
    m_isRunning = true;
}

void CWorkday::setEndTime(ATL::CTime &time)
{
    m_curTime.endTime = time;
    m_curTime.duration = m_curTime.endTime - m_curTime.startTime;
    m_sumDuration += m_curTime.duration;
    m_isRunning = false;
    m_workTimes.AddTail(new WorkTime_s(m_curTime));
}

void CWorkday::changeWorktimeCurrent(WorkTime_t &wt)
{
    if (wt.startTime > 0)
    {
        m_curTime.startTime = wt.startTime;
    }

    m_curTime.endTime = wt.endTime;
    m_curTime.duration = m_curTime.endTime - m_curTime.startTime;
    if ((wt.endTime > 0) &&
        (m_workTimes.GetCount() > 0))
    {
        WorkTime_t* wtarch = m_workTimes.GetTail();
        m_sumDuration -= wtarch->duration;
        wtarch->startTime = wt.startTime;
        wtarch->endTime = wt.endTime;
        wtarch->duration = wt.duration;
        m_sumDuration += wt.duration;
    }
}

void CWorkday::AddWorktime(WorkTime_t& wt)
{
    m_curTime.startTime = wt.startTime;
    m_curTime.endTime = wt.endTime;
    m_sumDuration += wt.duration;
    m_curTime.duration = m_sumDuration;
    m_workTimes.AddTail(new WorkTime_s(wt));
}

void CWorkday::deleteWorktimeCurrent(void)
{
    m_curTime.startTime = 0;
    m_curTime.endTime = 0;
    m_curTime.duration = 0;
    if (m_workTimes.GetCount() > 0)
    {
        WorkTime_t* wt = m_workTimes.GetTail();
        m_sumDuration -= wt->duration;
        m_workTimes.RemoveTail();
        delete wt;
    }
}

void CWorkday::deleteWorktimeArchive(HANDLE hwt)
{
    WorkTime_t* wt;
    POSITION posItem = (POSITION)hwt;
    POSITION pos = m_workTimes.GetTailPosition();
    POSITION pos_old;
    while (NULL != pos)
    {
        if (pos == posItem)
        {
            pos_old = pos;
            wt = m_workTimes.GetPrev(pos);
            m_sumDuration -= wt->duration;
            m_workTimes.RemoveAt(pos_old);
            delete wt;
        }
        else
        {
            wt = m_workTimes.GetPrev(pos);
        }
    }
}

void CWorkday::changeWorktimeArchive(HANDLE hwt, WorkTime_t &wtNew)
{
    WorkTime_t* wt;
    POSITION posItem = (POSITION)hwt;
    POSITION pos = m_workTimes.GetHeadPosition();
    m_sumDuration = 0;
    while (NULL != pos)
    {
        if (pos == posItem)
        {
            wt = m_workTimes.GetNext(pos);
            wt->startTime = wtNew.startTime;
            wt->endTime   = wtNew.endTime;
            wt->duration  = wtNew.duration;
        }
        else
        {
            wt = m_workTimes.GetNext(pos);
        }
        m_sumDuration += wt->duration;
    }
}

CString& CWorkday::getStartTime()
{
    m_strTime.Empty();
    if (m_curTime.startTime > 0)
        m_strTime = m_curTime.startTime.Format(_T("%H:%M"));
    return m_strTime;
}

CString& CWorkday::getEndTime()
{
    m_strTime.Empty();
    if (m_curTime.endTime > 0)
        m_strTime = m_curTime.endTime.Format(_T("%H:%M"));
    return m_strTime;
}

CString& CWorkday::getDuration()
{
    m_strTime.Empty();
    if (m_sumDuration > 0)
        m_strTime = m_sumDuration.Format(_T("%H:%M"));
    return m_strTime;
}

WorkTime_t*  CWorkday::getWorkTimes(POSITION& pos, POSITION& posItem)
{
    WorkTime_t* wt = NULL;

    if (NULL == pos)
    {
        pos = m_workTimes.GetHeadPosition();
    }
    if (NULL != pos)
    {
        posItem = pos;
        wt = m_workTimes.GetNext(pos);
    }

    return wt;
}

void CWorkday::Serialize(CArchive& ar)
{
    int count_wt, count_notes;
    WorkTime_t* wt;
    POSITION  pos;
    CString   note_line;
    UINT      nSchema;

    CObject::Serialize(ar);

    nSchema = ar.GetObjectSchema();
    if (ar.IsStoring())
    {
        ar  << m_date
            << m_curTime.startTime
            << m_curTime.endTime
            << m_curTime.duration
            << m_sumDuration;
        count_notes = m_noteList.GetCount();
        ar << count_notes;
        for (int i = 0; i < count_notes; i++)
        {
            ar << m_noteList[i];
        }
        count_wt = m_workTimes.GetCount();
        ar << count_wt;
        pos = m_workTimes.GetHeadPosition();
        while (NULL != pos)
        {
            wt = m_workTimes.GetNext(pos);
            ar  << wt->startTime
                << wt->endTime
                << wt->duration;
        }
    }
    else
    {
        ar  >> m_date 
            >> m_curTime.startTime
            >> m_curTime.endTime
            >> m_curTime.duration
            >> m_sumDuration;
        if (nSchema == 1) 
        {
            ar >> m_note;
            parseNoteString();
        }
        else if (nSchema == 2)
        {
            ar >> count_notes;
            m_noteList.SetSize(count_notes);
            for (int i = 0; i < count_notes; i++)
            {
                ar >> note_line;
                m_noteList[i] = note_line;
            }
        }
        ar >> count_wt;
        for (int i = 0; i < count_wt; i++)
        {
            wt = new WorkTime_t;
            ar  >> wt->startTime
                >> wt->endTime
                >> wt->duration;
            m_workTimes.AddTail(wt);
        }
    }
}

void CWorkday::serializeXml(CArchive & ar, CXmlDocument & doc)
{
	int count_wt;
	WorkTime_t* wt;
	POSITION  pos;

	doc.writeTag(CString("Date"), getDate());
	count_wt = m_workTimes.GetCount();
	doc.writeTag(CString("Count"), count_wt);
	doc.openTag(CString("Worktimes"));
	pos = m_workTimes.GetHeadPosition();
	while (NULL != pos)
	{
		wt = m_workTimes.GetNext(pos);
		m_strTime = wt->startTime.Format(_T("%H:%M"));
		doc.writeTag(CString("Begin"), m_strTime);
		m_strTime = wt->endTime.Format(_T("%H:%M"));
		doc.writeTag(CString("End"), m_strTime);
	}
	doc.closeTag(CString("Worktimes"));
}

void CWorkday::parseNoteString(void)
{
    int index = 0;
    int line_length;
    CString  note_line;
    int note_length = m_note.GetLength();
    do
    {
        line_length = m_note.Find(_T('\r'), index) - index;
        if (line_length > 0) 
        {
            note_line = m_note.Mid(index, line_length);
            index += line_length + 1;
        }
        else
        {
            note_line = m_note.Mid(index);
            index += note_line.GetLength() + 1;
        }
        m_noteList.Add(note_line);
    }
    while (index < note_length);
}

IMPLEMENT_SERIAL(CWorkpackage, CObject, VERSIONABLE_SCHEMA | 1)

// CWorkpackage implementation
CWorkpackage::CWorkpackage(CString& text, CTime& date)
        : m_wpckg_text(text)
{
    CWorkday* wd = new CWorkday(date);
    m_workDays.AddTail(wd);
}

CWorkpackage::~CWorkpackage()
{
    CWorkday* wd;
    POSITION pos = NULL;
    pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        delete wd;
    }
    m_workDays.RemoveAll();
}

void CWorkpackage::Serialize(CArchive& ar)
{
    POSITION pos;
    CWorkday* wd;
    int count_wd;

    CObject::Serialize(ar);

    UINT nSchema = ar.GetObjectSchema();
    if (ar.IsStoring())
    {
        count_wd = m_workDays.GetCount();
        ar << count_wd;
        ar << m_wpckg_text;
        pos = m_workDays.GetHeadPosition();
        while (NULL != pos)
        {
            wd = m_workDays.GetNext(pos);
            ar << wd;
        }
    }
    else
    {
        ar >> count_wd;
        ar >> m_wpckg_text;
        for (int i = 0; i < count_wd; i++)
        {
            if (nSchema == 1)
            {
                ar >> wd;
                m_workDays.AddTail(wd);
            }
            else
            {
                wd = new CWorkday;
                wd->Serialize(ar);
                m_workDays.AddTail(wd);
            }
        }
    }
}

void CWorkpackage::setStartTime(CTime &dateTime)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(dateTime);

    // search for existing workday entry for current date
    bool bFound = false;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if ((*wd == date) && !wd->isRunning())
        {
            wd->setStartTime(dateTime);
            bFound = true;
        }
    }

    // if no entry exists create new one
    if (!bFound)
    {
        wd = new CWorkday(date);
        wd->setStartTime(dateTime);
        m_workDays.AddTail(wd);
    }
}

void CWorkpackage::ChangeWorktimeCurrent(WorkTime_t &wt)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(wt.startTime);

    // search for existing workday entry for current date
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            wd->changeWorktimeCurrent(wt);
        }
    }
}

void CWorkpackage::DeleteWorktimeCurrent(WorkTime_t& wt)
{
    CWorkday* wd;

    if (wt.startTime == 0) return;

    // split date time
    CTime date = GetDateOfDateTime(wt.startTime);

    // search for existing workday entry for current date
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            wd->deleteWorktimeCurrent();
        }
    }
}

void CWorkpackage::ChangeWorktimeArchive(ATL::CTime &dateTime, HANDLE hwt, WorkTime_t &wt)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(dateTime);

    // search for existing workday entry for current date
    bool bFound = false;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            wd->changeWorktimeArchive(hwt, wt);
        }
    }
}

void CWorkpackage::DeleteWorktimeArchive(ATL::CTime &dateTime, HANDLE hwt)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(dateTime);

    // search for existing workday entry for current date
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            wd->deleteWorktimeArchive(hwt);
        }
    }
}

void CWorkpackage::DeleteNoteOfDay(ATL::CTime &dateTime)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(dateTime);

    // search for existing workday entry for current date
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            wd->deleteNote();
        }
    }
}

void CWorkpackage::DeleteAllNotes()
{
    CWorkday* wd;

    // search for existing workday entry for current date
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        wd->deleteNote();
    }
}

void CWorkpackage::setEndTime(CTime &dateTime)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(dateTime);

    // search for existing worktime entry for current date
    bool bFound = false;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if ((*wd == date) && wd->isRunning())
        {
            bFound = true;
            break;
        }
    }

    // update duration
    if (bFound)
    {
        wd->setEndTime(dateTime);
    }
}

CString& CWorkpackage::getStartTime(ATL::CTime &date)
{
    CWorkday* wd;
    m_strTime.Empty();
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            m_strTime = wd->getStartTime();
            break;
        }
    }
    return m_strTime;
}

CString& CWorkpackage::getEndTime(ATL::CTime &date)
{
    CWorkday* wd;
    m_strTime.Empty();
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            m_strTime = wd->getEndTime();
            break;
        }
    }
    return m_strTime;
}

CString& CWorkpackage::getDuration(ATL::CTime &date)
{
    CWorkday* wd;
    m_strTime.Empty();
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            m_strTime = wd->getDuration();
            break;
        }
    }
    return m_strTime;
}

CTimeSpan& CWorkpackage::getDurationPerDay(CTime& date)
{
    CWorkday* wd;
    CTimeSpan  retTime;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            return wd->getWholeDuration();
            break;
        }
    }
    return (retTime = 0);
}

bool CWorkpackage::isWorktimeRunning(ATL::CTime &date)
{
    CWorkday* wd;
    bool isRunning = false;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date)
        {
            isRunning = wd->isRunning();
            break;
        }
    }
    return isRunning;
}

bool CWorkpackage::HasWorkpackageDate(CTime& date)
{
    CWorkday* wd;
    POSITION pos = NULL;
    pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date) return true;
    }
    return false;
}

CWorkday* CWorkpackage::getWorkday(ATL::CTime &date)
{
    CWorkday  *wd = NULL,
              *wdret = NULL;
    POSITION pos = NULL;
    pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date) 
        {
            wdret = wd;
            break;
        }
    }
    return wdret;
}

CTime& CWorkpackage::GetDateOfDateTime(ATL::CTime &dateTime)
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

CString& CWorkday::getDate(void)
{
    m_strTime = m_date.Format(_T("%d.%m.%Y"));
    return m_strTime;
}

CWorkday*  CWorkpackage::GetWorkdays(POSITION& pos)
{
    CWorkday* wd = NULL;

    if (pos == NULL)
    {
        pos = m_workDays.GetHeadPosition();
    }
    
    if (pos != NULL)
    {
        wd = m_workDays.GetNext(pos);
    }
    return wd;
}

CTime& CWorkpackage::GetFirstDay(void)
{
    CWorkday* wd = m_workDays.GetHead();
    return wd->GetWorkdayDate();
}

CTime& CWorkpackage::GetLastDay(void)
{
    CWorkday* wd = m_workDays.GetTail();
    return wd->GetWorkdayDate();
}

void CWorkpackage::AddWorktime(CTime& selectedTime, WorkTime_t& wt)
{
    CWorkday* wd;

    // split date time
    CTime date = GetDateOfDateTime(selectedTime);

    // search for existing workday entry for current date
    bool bFound = false;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if ((*wd == date) && !wd->isRunning())
        {
            wd->AddWorktime(wt);
            bFound = true;
        }
    }

    // if no entry exists create new one
    if (!bFound)
    {
        wd = new CWorkday(date);
        wd->AddWorktime(wt);
        m_workDays.AddTail(wd);
    }
}

int CWorkday::getCountWorktimes(void)
{
    return m_workTimes.GetCount();
}

int CWorkpackage::GetCountLines(void)
{
    CWorkday* wd;
    int count, sumCount = 0;
    POSITION pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        count = wd->getNoOfNoteLines();
        if (count > 0) sumCount += count + 1;
        sumCount++;
    }
    return sumCount;
}

CWorkday* CWorkpackage::getNextWorkday(CTime& date)
{
    CWorkday  *wd = NULL,
              *wdret = NULL;
    POSITION pos = NULL;
    pos = m_workDays.GetTailPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetPrev(pos);
        if (*wd == date) 
        {
            if (NULL == pos) wdret = wd;
            else wdret = m_workDays.GetNext(pos);
            break;
        }
    }
    return wdret;
}

CWorkday* CWorkpackage::getPrevWorkday(CTime& date)
{
    CWorkday  *wd = NULL,
              *wdret = NULL;
    POSITION pos = NULL;
    pos = m_workDays.GetHeadPosition();
    while (NULL != pos)
    {
        wd = m_workDays.GetNext(pos);
        if (*wd == date) 
        {
            if (NULL == pos) wdret = wd;
            else wdret = m_workDays.GetPrev(pos);
            break;
        }
    }
    return wdret;
}


void CWorkpackage::serializeXml(CArchive & ar, CXmlDocument & doc)
{
	POSITION pos;
	CWorkday* wd;

	doc.openTag(CString("Workpackage"));
	doc.writeTag(CString("Name"), m_wpckg_text);
	doc.writeTag(CString("Count"), m_workDays.GetCount());
	doc.openTag(CString("Workdays"));
	pos = m_workDays.GetHeadPosition();
	while (NULL != pos)
	{
		wd = m_workDays.GetNext(pos);
		wd->serializeXml(ar, doc);
	}
	doc.closeTag(CString("Workdays"));
	doc.closeTag(CString("Workpackage"));
}
