#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "NoteDialog.h"
#include "ReportDialog.h"


// CDetailsView form view

class CDetailsView : public CFormView
{
	DECLARE_DYNCREATE(CDetailsView)

protected:
	CDetailsView();           // protected constructor used by dynamic creation
	virtual ~CDetailsView();

public:
	enum { IDD = IDD_DETAILS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnCloseupDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEditText();
	CString   m_workpackage;
	CFileTime m_date_time;
	CTime     m_date;
    CTime     m_tmpTime;
    CListCtrl m_worktimeList;
private:
	void EnableAddButton(BOOL enable = 1);
public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
    afx_msg void OnNMClickListWorktimes(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	HANDLE    m_hWorkpackage;
    HANDLE    m_hWorktime;
    HICON     m_iconStart;
    HICON     m_iconPause;
public:
    afx_msg void OnBnClickedButtonApply();
    afx_msg void OnBnClickedButtonStartStop();
private:
    CButton m_btnStartStop;
    CTime& GetTimeFromEditField(int idc);
    bool m_archiveSelected;
    bool m_lastItem;
    CNoteDialog m_noteDlg;
    CReportDialog m_reportDlg;
public:
    afx_msg void OnBnClickedCheckEditMode();
    afx_msg void OnBnClickedButtonAddWt();
    afx_msg void OnBnClickedButtonNote();
    afx_msg void OnBnClickedButtonDelWt();
    afx_msg void OnBnClickedButton2();
    afx_msg LRESULT OnNextNote(WPARAM, LPARAM);
    afx_msg LRESULT OnPrevNote(WPARAM, LPARAM);
    afx_msg LRESULT OnSwitchWorkday(WPARAM, LPARAM);
    afx_msg void OnBerichteTagesbericht();
};


