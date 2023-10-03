#pragma once
#include "afxcmn.h"

class CWorkday;

// CNoteDialog dialog

class CNoteDialog : public CDialog
{
	DECLARE_DYNAMIC(CNoteDialog)

public:
	CNoteDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoteDialog();

// Dialog Data
	enum { IDD = IDD_NOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CRichEditCtrl m_insert_note;
    void SetNote(CStringArray& strNote);
    CStringArray& GetNote(void);
    void SetOwner(CWnd* owner) { m_owner = owner; }
private:
    CString m_strNote;
    CStringArray m_noteLines;
public:
    afx_msg void OnBnClickedOk();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonPrev();
    afx_msg void OnBnClickedButtonSucc();
    void SetDate(CTime& date);
private:
    CString m_strDate;
    CTime   m_date;
    CWnd*   m_owner;
public:
    void setWorkday(CWorkday* pWorkday);
    CTime& GetDate(void);
    afx_msg void OnBnClickedButtonSwitchWd();
};
