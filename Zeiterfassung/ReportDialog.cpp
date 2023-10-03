// ReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Zeiterfassung.h"
#include "ReportDialog.h"


// CReportDialog dialog

IMPLEMENT_DYNAMIC(CReportDialog, CDialog)

CReportDialog::CReportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDialog::IDD, pParent)
{

}

CReportDialog::~CReportDialog()
{
}

void CReportDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_REPORT, m_show_report);
}


BEGIN_MESSAGE_MAP(CReportDialog, CDialog)
END_MESSAGE_MAP()


// CReportDialog message handlers

void CReportDialog::setReportNote(CStringArray& reportLines)
{
    CString strLine;
    int index, length = 0, line_len;
    TCHAR* buffer;
    TCHAR* line_buf;
    CStringArray report_lines;
    report_lines.Copy(reportLines);
    int countLines = report_lines.GetCount();
    for (int i = 0; i < countLines; i++)
    {
        strLine = _T("        ");
        strLine += report_lines.GetAt(i);
        length += strLine.GetLength() + 2;
        report_lines.SetAt(i, strLine);
    }
    if (length == 0) 
    {
        return;
    }
    m_strReport += _T("\r\n\r\n");
    length += m_strReport.GetLength();
    
    if (countLines > 0)
    {
        m_strReport += report_lines.GetAt(0);
        index = m_strReport.GetLength();
        buffer = m_strReport.GetBufferSetLength(length);
        for (int i = 1; i < countLines; i++)
        {
            buffer[index] = _T('\r');
            index++;
            buffer[index] = _T('\n');
            index++;
            strLine = report_lines.GetAt(i);
            line_len = strLine.GetLength();
            if (line_len > 0)
            {
                line_buf = strLine.GetBuffer();
                _tcsncpy_s(&buffer[index],length - index, line_buf, line_len);
                index += line_len;
                strLine.ReleaseBuffer();
            }
        }
        buffer[index] = '\0';
        m_strReport.ReleaseBuffer();
    }
}

BOOL CReportDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_show_report.SetWindowText(m_strReport);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDialog::setReportWp(CString& wp_test)
{
    m_strReport += wp_test;
}

void CReportDialog::setReportDate(CString& workday)
{
    if (!m_strReport.IsEmpty()) m_strReport += _T("\r\n\r\n");
    m_strReport += workday + _T("     ");
}

void CReportDialog::clear(void)
{
    m_strReport.Empty();
}
