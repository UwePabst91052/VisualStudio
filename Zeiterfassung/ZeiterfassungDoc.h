// ZeiterfassungDoc.h : Schnittstelle der Klasse CZeiterfassungDoc
//


#pragma once

class CWorkpackage;

class CZeiterfassungDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CZeiterfassungDoc();
	DECLARE_DYNCREATE(CZeiterfassungDoc)

// Attribute
private:
	CList<CWorkpackage*> m_workpackages;

// Vorgänge
public:
	HANDLE AddWorkpackage(CString& text);
	void DeleteWorkpackage(HANDLE hwp);
	void BeginLogging(HANDLE hwp);
	void EndLogging(HANDLE hwp);
	void ResetLog(HANDLE hwp);
	bool GetWorkpackage(HANDLE hwp, CWorkpackage*& wpckg);
	void SetCurrentDate(CTime date);
	CWorkpackage* GetWorkpackages(POSITION& pos, HANDLE& hwp);
	void StoreWorkpackagesXml(CArchive& ar);

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CZeiterfassungDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
private:
	CTime m_date;
public:

	CTime& GetCurrentDate(void)
	{
		return m_date;
	}
	bool PauseLogging(HANDLE hwp);
private:
    CTime& GetDateOfDateTime(CTime& dateTime);
    CTime m_tmpDate;
    CTimeSpan m_tmpDuration;
    bool m_edit_mode;
public:
    void SetEditMode(bool bEditMode);
private:
    SYSTEMTIME m_firstWorkday;
    SYSTEMTIME m_lastWorkday;
public:
    void GetWorkdayLimits(CTime& firstDay, CTime& lastDay);
private:
    void ResetDocument(void);
public:
    int GetCountLinesDoc(void);
    void PrintHeader(CDC* pDC, CPoint& pos);
    void PrintFooter(CDC* pDC, 
                     CString& fmt, 
                     CPoint& pos, 
                     int curPage, 
                     int fromPage,
                     int maxPage);
    afx_msg void OnBearbeitenLoesche();
    CTimeSpan& GetWorkdaySum();
    int GetCountWorkpackage(void);
};


