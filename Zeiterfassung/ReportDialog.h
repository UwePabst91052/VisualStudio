#pragma once
#include "afxwin.h"


// CReportDialog dialog

class CReportDialog : public CDialog
{
	DECLARE_DYNAMIC(CReportDialog)

public:
	CReportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    void setReportNote(CStringArray& reportLines);
private:
    CString m_strReport;
    CEdit m_show_report;
public:
    virtual BOOL OnInitDialog();
    void setReportWp(CString& wp_test);
    void setReportDate(CString& workday);
    void clear(void);
};
