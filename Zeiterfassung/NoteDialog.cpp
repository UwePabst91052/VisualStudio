// NoteDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Zeiterfassung.h"
#include "Workpackage.h"
#include "NoteDialog.h"

#include "resource.h"

// CNoteDialog dialog

IMPLEMENT_DYNAMIC(CNoteDialog, CDialog)

CNoteDialog::CNoteDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNoteDialog::IDD, pParent)
    , m_strNote(_T(""))
    , m_date(0)
    , m_strDate(_T(""))
{

}

CNoteDialog::~CNoteDialog()
{
}

void CNoteDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RICHEDIT21, m_insert_note);
    DDX_Text(pDX, IDC_EDIT_DATE, m_strDate);
}


BEGIN_MESSAGE_MAP(CNoteDialog, CDialog)
    ON_BN_CLICKED(IDOK, &CNoteDialog::OnBnClickedOk)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BUTTON_PREV, &CNoteDialog::OnBnClickedButtonPrev)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &CNoteDialog::OnBnClickedButtonSucc)
    ON_BN_CLICKED(IDC_BUTTON_SWITCH_WD, &CNoteDialog::OnBnClickedButtonSwitchWd)
END_MESSAGE_MAP()


// CNoteDialog message handlers

void CNoteDialog::SetNote(CStringArray& noteLines)
{
    CString strLine;
    int index, length = 0, line_len;
    TCHAR* buffer;
    TCHAR* line_buf;
    int countLines = noteLines.GetCount();
    for (int i = 0; i < countLines; i++)
    {
        strLine = noteLines.GetAt(i);
        length += strLine.GetLength() + 2;
    }
    if (length == 0) 
    {
        m_strNote.Empty();
        return;
    }
    
    if (countLines > 0)
    {
        m_strNote = noteLines.GetAt(0);
        index = m_strNote.GetLength() - 1;
        buffer = m_strNote.GetBufferSetLength(length);
        for (int i = 1; i < countLines; i++)
        {
            buffer[index] = _T('\r');
            index++;
            buffer[index] = _T('\n');
            index++;
            strLine = noteLines.GetAt(i);
            line_len = strLine.GetLength();
            if (line_len > 0)
            {
                line_buf = strLine.GetBuffer();
                _tcsncpy_s(&buffer[index],length - index, line_buf, line_len);
                index += line_len - 1;
                strLine.ReleaseBuffer();
            }
        }
        buffer[index] = '\0';
        m_strNote.ReleaseBuffer();
    }
}

CStringArray& CNoteDialog::GetNote(void)
{
    return m_noteLines;
}

void CNoteDialog::OnBnClickedOk()
{
    m_noteLines.RemoveAll();
    int nLineLength, nLineIndex, nLineCount = m_insert_note.GetLineCount();
    CString strText, strLine;
    m_insert_note.GetWindowText(strLine);

    if (strLine.IsEmpty()) return;

    for (int i = 0; i < nLineCount; i++)
    {
       nLineIndex = m_insert_note.LineIndex(i);
       nLineLength = m_insert_note.LineLength(nLineIndex);
       if (nLineLength > 0)
       {
           m_insert_note.GetLine(i, strText.GetBufferSetLength(nLineLength + 1), nLineLength + 1);
           strText.SetAt(nLineLength, _T('\0')); // null terminate
           strText.ReleaseBuffer(nLineLength + 1);
       }
       else
       {
           strText = _T("");
       }
       m_noteLines.Add(strText);
    }

    OnOK();
}

int CNoteDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

BOOL CNoteDialog::OnInitDialog()
{
    CRect rcText;
    CDialog::OnInitDialog();

    //m_insert_note.GetRect(&rcText);
    ////rcText.DeflateRect(0, 0, 60, 0);
    //m_insert_note.SetRect(&rcText);
    m_insert_note.SetWindowText(m_strNote);

    HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), 
                   MAKEINTRESOURCE(IDI_ICON_NEXT), IMAGE_ICON, 0, 0, 0);
    CButton* pBtn = (CButton*)GetDlgItem(IDC_BUTTON_NEXT);
    pBtn->SetIcon(hIcon);
    hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), 
                   MAKEINTRESOURCE(IDI_ICON_PREV), IMAGE_ICON, 0, 0, 0);
    pBtn = (CButton*)GetDlgItem(IDC_BUTTON_PREV);
    pBtn->SetIcon(hIcon);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CNoteDialog::OnBnClickedButtonPrev()
{
    m_owner->PostMessage(WM_NEXT_NOTE);
}

void CNoteDialog::OnBnClickedButtonSucc()
{
    m_owner->PostMessage(WM_PREV_NOTE);
}

void CNoteDialog::SetDate(CTime& date)
{
    m_strDate = date.Format(_T("%d.%m.%Y"));
    m_date = date;
}

void CNoteDialog::setWorkday(CWorkday* pWorkday)
{
    SetNote(pWorkday->getNote());
    m_date = pWorkday->getWdDate();
    m_strDate = m_date.Format(_T("%d.%m.%Y"));
    m_insert_note.SetWindowText(m_strNote);
    UpdateData(FALSE);
}

CTime& CNoteDialog::GetDate(void)
{
    return m_date;
}

void CNoteDialog::OnBnClickedButtonSwitchWd()
{
    m_owner->PostMessage(WM_SWITCH_WD);
}
