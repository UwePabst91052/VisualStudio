// Workpackage.h

#pragma once

class CXmlDocument;

typedef struct WorkTime_s
{
    CTime       startTime;
    CTime		endTime;
    CTimeSpan	duration;
public:
    WorkTime_s() {}
    WorkTime_s(WorkTime_s& wt)
    {
        startTime = wt.startTime;
        endTime = wt.endTime;
        duration = wt.duration;
    }
} WorkTime_t;

class CWorkday : public CObject
{
    CTime			   m_date;
    bool			   m_isRunning;
    WorkTime_t         m_curTime;
    CTimeSpan          m_sumDuration;
    CList<WorkTime_t*> m_workTimes;
    CString			   m_strTime;
    CStringArray       m_noteList;
    CString            m_note;
protected:
    DECLARE_SERIAL(CWorkday);
    CWorkday() 
        :
      m_isRunning(false)
    {};
public:
    CWorkday(CTime& date);
    ~CWorkday();
    void setDate(CTime& date)
    {
        m_date = date;
    }
    void setStartTime(CTime& time);
    void setEndTime(CTime& time);
    void changeWorktimeCurrent(WorkTime_t& wt);
    void changeWorktimeArchive(HANDLE hwt, WorkTime_t& wt);
    CString& getStartTime();
    CString& getEndTime();
    CString& getDuration();
    WorkTime_t* getWorkTimes(POSITION& pos, POSITION& posItem);
    bool operator==(CTime inDate)
    {
        return (m_date == inDate);
    }
    bool isRunning()
    {
        return m_isRunning;
    }
    void Serialize(CArchive& ar);
    CString& getDate(void);
    CTime& getWdDate()
    {
        return m_date;
    }

    CTime& GetWorkdayDate(void)
    {
        return m_date;
    }
    void AddWorktime(WorkTime_t& wt);
    void setNote(CStringArray& note)
    {
        m_noteList.RemoveAll();
        m_noteList.Append(note);
    }
    CStringArray& getNote()
    {
        return m_noteList;
    }
    void deleteNote()
    {
        m_noteList.RemoveAll();
    }
    void deleteWorktimeCurrent(void);
    void deleteWorktimeArchive(HANDLE hwt);
    int getCountWorktimes(void);
    CTimeSpan& getWholeDuration(void)
    {
        return m_sumDuration;
    }
    int getNoOfNoteLines()
    {
        int count = m_noteList.GetCount();
        return (count > 0 ? count + 1 : 0);
    }
	void serializeXml(CArchive& ar, CXmlDocument& doc);
private:
    void parseNoteString(void);
};

class CWorkpackage : public CObject
{
    CString			 m_wpckg_text;
    CList<CWorkday*> m_workDays;
    CTime			 m_tmpDate;
    CString			 m_strTime;
    
protected:
    DECLARE_SERIAL(CWorkpackage);
    CWorkpackage() {}
public:
    CWorkpackage(CString& text, CTime& date);
    ~CWorkpackage();
    void setStartTime(CTime& time);
    void setEndTime(CTime& time);
    CString& getStartTime(CTime& date);
    CString& getEndTime(CTime& date);
    CString& getDuration(CTime& date);
    bool isWorktimeRunning(CTime& date);
    CString& GetWpText()
    {
        return m_wpckg_text;
    }
    void SetWpText(CString& text)
    {
        m_wpckg_text = text;
    }
    CWorkday* getWorkday(CTime& date);
    bool HasWorkpackageDate(CTime& date);
private:
    CTime& GetDateOfDateTime(CTime& dateTime);
public:
    void Serialize(CArchive& ar);
    void ChangeWorktimeCurrent(WorkTime_t& wt);
    void ChangeWorktimeArchive(CTime& date, HANDLE hwt, WorkTime_t& wt);
    CWorkday* GetWorkdays(POSITION& pos);
    CTime& GetFirstDay(void);
    CTime& GetLastDay(void);
    void AddWorktime(CTime& selectedTime, WorkTime_t& wt);
    void DeleteWorktimeCurrent(WorkTime_t& wt);
    void DeleteWorktimeArchive(CTime& dateTime, HANDLE hwt);
    void DeleteNoteOfDay(CTime& dateTime);
    void DeleteAllNotes();
    int GetCountLines(void);
    CTimeSpan& getDurationPerDay(CTime& date);
    CWorkday* getNextWorkday(CTime& date);
    CWorkday* getPrevWorkday(CTime& date);
	void serializeXml(CArchive& ar, CXmlDocument& doc);
    static bool CompareCWorkday(CWorkday*& workday1, CWorkday*& workday2);
    // sort the list of workday depentend on the date
    void SortWorkdays();
};
