// Details.cpp : implementation file
//

#include "stdafx.h"
#include "Zeiterfassung.h"
#include "DetailsView.h"
#include "MainFrm.h"
#include "ZeiterfassungDoc.h"
#include "ZeiterfassungView.h"
#include "Workpackage.h"

// CDetailsView

IMPLEMENT_DYNCREATE(CDetailsView, CFormView)

CDetailsView::CDetailsView()
	: CFormView(CDetailsView::IDD)
	, m_workpackage(_T(""))
    , m_hWorkpackage(NULL)
    , m_archiveSelected(false)
    , m_lastItem(false)
{
	m_date_time = CFileTime::GetCurrentTime();
}

CDetailsView::~CDetailsView()
{
}

void CDetailsView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TEXT, m_workpackage);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date_time);
    DDX_Control(pDX, IDC_BUTTON_START_STOP, m_btnStartStop);
}

BEGIN_MESSAGE_MAP(CDetailsView, CFormView)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER1, &CDetailsView::OnDtnCloseupDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDetailsView::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDetailsView::OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CDetailsView::OnEnChangeEditText)
    ON_NOTIFY(NM_CLICK, IDC_LIST_WORKTIMES, &CDetailsView::OnNMClickListWorktimes)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDetailsView::OnBnClickedButtonApply)
    ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CDetailsView::OnBnClickedButtonStartStop)
    ON_BN_CLICKED(IDC_CHECK_EDIT_MODE, &CDetailsView::OnBnClickedCheckEditMode)
    ON_BN_CLICKED(IDC_BUTTON_ADD_WT, &CDetailsView::OnBnClickedButtonAddWt)
    ON_BN_CLICKED(IDC_BUTTON_NOTE, &CDetailsView::OnBnClickedButtonNote)
    ON_BN_CLICKED(IDC_BUTTON_DEL_WT, &CDetailsView::OnBnClickedButtonDelWt)
    ON_BN_CLICKED(IDC_BUTTON2, &CDetailsView::OnBnClickedButton2)
    ON_MESSAGE(WM_NEXT_NOTE, OnNextNote)
    ON_MESSAGE(WM_PREV_NOTE, OnPrevNote)
    ON_MESSAGE(WM_SWITCH_WD, OnSwitchWorkday)
    ON_COMMAND(ID_BERICHTE_TAGESBERICHT, &CDetailsView::OnBerichteTagesbericht)
    ON_BN_CLICKED(IDC_BTN_CHANGE, &CDetailsView::OnBnClickedBtnChange)
END_MESSAGE_MAP()


// CDetailsView diagnostics

#ifdef _DEBUG
void CDetailsView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetailsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetailsView message handlers

void CDetailsView::OnDtnCloseupDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();

	m_date = m_date_time;
	pDoc->SetCurrentDate(m_date);

    pDoc->UpdateAllViews(NULL, (LPARAM)m_hWorkpackage);
	*pResult = 0;
}

void CDetailsView::OnBnClickedButtonStartStop()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();

    if (NULL == m_hWorkpackage) return;

	bool isRunning = pDoc->PauseLogging(m_hWorkpackage);
    if (isRunning)
    {
        m_btnStartStop.SetIcon(m_iconPause);
    }
    else
    {
        m_btnStartStop.SetIcon(m_iconStart);
    }
}

void CDetailsView::OnBnClickedBtnChange()
{
    UpdateData();
    CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    pDoc->ChangeWorkpackageName(m_hWorkpackage, m_workpackage);
    pDoc->UpdateAllViews(this);
    EnableChangeButton(FALSE);
}

void CDetailsView::OnBnClickedButtonAdd()
{
	UpdateData();
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
	pDoc->AddWorkpackage(m_workpackage);
	pDoc->UpdateAllViews(this);

	TRACE1("Add workpackage: %x\n", m_hWorkpackage);
	EnableAddButton(FALSE);
}

void CDetailsView::OnBnClickedButtonDelete()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
	pDoc->DeleteWorkpackage(m_hWorkpackage);
	TRACE1("Delete workpackage: %x\n", m_hWorkpackage);
	pDoc->UpdateAllViews(this);
}

void CDetailsView::OnEnChangeEditText()
{
	EnableAddButton();
    EnableChangeButton();
}

void CDetailsView::EnableAddButton(BOOL enable)
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_ADD);
	pWnd->EnableWindow(enable);
}

void CDetailsView::EnableChangeButton(BOOL enable)
{
    CWnd* pWnd = GetDlgItem(IDC_BTN_CHANGE);
    pWnd->EnableWindow(enable);
}

void CDetailsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

    m_worktimeList.DeleteAllItems();
    SetDlgItemText(IDC_EDIT_START, _T(""));
    SetDlgItemText(IDC_EDIT_END, _T(""));
    SetDlgItemText(IDC_EDIT_DURATION, _T(""));
    m_btnStartStop.SetIcon(m_iconPause);
	EnableAddButton(FALSE);
    EnableChangeButton(FALSE);
}

void CDetailsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* /*pHint*/)
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
	CWorkpackage* wpckg;
    CWorkday*     wd;
    WorkTime_t*   wt;
    POSITION      pos = NULL,
                  posItem = NULL;
    CString       strTime;
    int           index = 0;

	if ((NULL != lHint) &&
		pDoc->GetWorkpackage((HANDLE)lHint, wpckg))
	{
		m_workpackage = wpckg->GetWpText();
		strTime = wpckg->getStartTime(pDoc->GetCurrentDate());
        SetDlgItemText(IDC_EDIT_START, strTime);
        strTime = wpckg->getEndTime(pDoc->GetCurrentDate());
        SetDlgItemText(IDC_EDIT_END, strTime);
        strTime = wpckg->getDuration(pDoc->GetCurrentDate());
        SetDlgItemText(IDC_EDIT_DURATION, strTime);
		m_hWorkpackage = (HANDLE)lHint;
        wd = wpckg->getWorkday(pDoc->GetCurrentDate());
        m_worktimeList.DeleteAllItems();
        if (NULL != wd)
        {
            do
            {
                wt = wd->getWorkTimes(pos, posItem);
                if (NULL != wt)
                {
                    m_archiveSelected = false;
                    m_lastItem = false;
                    strTime = wt->startTime.Format(_T("%H:%M"));
                    m_worktimeList.InsertItem(index, strTime);
                    strTime = wt->endTime.Format(_T("%H:%M"));
                    m_worktimeList.SetItem(index, 1, LVIF_TEXT, strTime, 0, 0, 0, 0);
                    strTime = wt->duration.Format(_T("%H:%M"));
                    m_worktimeList.SetItem(index, 2, LVIF_TEXT, strTime, 0, 0, 0, 0);
                    m_worktimeList.SetItemData(index, (DWORD_PTR)posItem);
                    index++;
                }
            }
            while ((NULL != wt) && (NULL != pos));
            if (wd->isRunning()) 
                m_btnStartStop.SetIcon(m_iconPause);
            else
                m_btnStartStop.SetIcon(m_iconStart);
        }
		UpdateData(FALSE);
	}
}

void CDetailsView::OnBnClickedButtonApply()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    WorkTime_t    wt;
    CWorkpackage* wp;
    CString       trace;

	UpdateData();

    wt.startTime = GetTimeFromEditField(IDC_EDIT_START);
    wt.endTime   = GetTimeFromEditField(IDC_EDIT_END);
    if ((wt.startTime > 0) && (wt.endTime > 0))
        wt.duration  = wt.endTime - wt.startTime;
    else
        wt.duration = 0;

    trace.Format(_T("%s %s %s\n"), wt.startTime.FormatGmt(_T("%H:%M")),
        wt.endTime.FormatGmt(_T("%H:%M")),
        wt.duration.Format(_T("%H:%M")));
    OutputDebugString(trace);

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    if (NULL != wp)
    {
        if (m_archiveSelected)
        {
            if (m_lastItem)
            {
                wp->ChangeWorktimeCurrent(wt);
            }
            else
            {
                wp->ChangeWorktimeArchive(pDoc->GetCurrentDate(), m_hWorktime, wt);
            }
        }
        else
        {
            wp->ChangeWorktimeCurrent(wt);
        }
    }
    pDoc->UpdateAllViews(NULL, (LPARAM)m_hWorkpackage);
	pDoc->SetModifiedFlag();
}

void CDetailsView::OnBnClickedButtonDelWt()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    WorkTime_t    wt;
    CWorkpackage* wp;

	UpdateData();

    wt.startTime = GetTimeFromEditField(IDC_EDIT_START);
    wt.endTime   = GetTimeFromEditField(IDC_EDIT_END);

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    if (NULL != wp)
    {
        if (m_archiveSelected)
        {
            wp->DeleteWorktimeArchive(pDoc->GetCurrentDate(), m_hWorktime);
        }
        else
        {
            wp->DeleteWorktimeCurrent(wt);
        }
    }
    pDoc->UpdateAllViews(NULL, (LPARAM)m_hWorkpackage);
	pDoc->SetModifiedFlag();
}

CTime& CDetailsView::GetTimeFromEditField(int idc)
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
	TCHAR buf[10];
	int hour = 0, 
		min = 0; 
	CTime time(pDoc->GetCurrentDate());
    m_tmpTime = 0;
	SYSTEMTIME  sysTime;

    GetDlgItemText(idc, (LPTSTR)buf, 10);
    if (_tcslen(buf) > 0)
    {
        _stscanf_s(buf, _T("%2d:%2d"), &hour, &min);

        time.GetAsSystemTime(sysTime);
        sysTime.wHour = hour;
        sysTime.wMinute = min;
        sysTime.wSecond = 0;
        CTime  tmpTime(sysTime);
        m_tmpTime = tmpTime;
    }

    return m_tmpTime;
}

void CDetailsView::OnNMClickListWorktimes(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
	CWorkpackage* wpckg;
    CWorkday*     wd;
    WorkTime_t*   wt;
    CString strTime;
    int item = pNMItemActivate->iItem;
    m_lastItem = item == (m_worktimeList.GetItemCount() - 1);
    POSITION pos = (POSITION)m_worktimeList.GetItemData(item),
             posItem = NULL;
    if (pDoc->GetWorkpackage(m_hWorkpackage, wpckg))
    {
        wd = wpckg->getWorkday(pDoc->GetCurrentDate());
        if (NULL != wd)
        {
            wt = wd->getWorkTimes(pos, posItem);
            if (NULL != wt)
            {
                m_hWorktime = (HANDLE)posItem;
                strTime = wt->startTime.Format(_T("%H:%M"));
                SetDlgItemText(IDC_EDIT_START, strTime);
                strTime = wt->endTime.Format(_T("%H:%M"));
                SetDlgItemText(IDC_EDIT_END, strTime);
                strTime = wt->duration.Format(_T("%H:%M"));
                SetDlgItemText(IDC_EDIT_DURATION, strTime);
                m_archiveSelected = true;
            }
        }
    }
    *pResult = 0;
}

int CDetailsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_worktimeList.Create(LVS_REPORT | LVS_SINGLESEL | LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP | WS_VISIBLE,
                          CRect(CPoint(7,160),CSize(230,150)), this, IDC_LIST_WORKTIMES);
    m_worktimeList.InsertColumn(0, _T("Startzeit"), LVCFMT_CENTER, 70, 0);
    m_worktimeList.InsertColumn(1, _T("Endezeit"), LVCFMT_CENTER, 70, 0);
    m_worktimeList.InsertColumn(2, _T("Dauer"), LVCFMT_CENTER, 70, 0);

    m_iconStart = (HICON)::LoadImage(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(IDI_ICON_START), IMAGE_ICON, 0, 0, 0);
    m_iconPause = (HICON)::LoadImage(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(IDI_ICON_PAUSE), IMAGE_ICON, 0, 0, 0);
    ::AfxInitRichEdit2();
    //m_noteDlg.Create(IDD_NOTE, this);
    return 0;
}


void CDetailsView::OnBnClickedCheckEditMode()
{
    CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    CButton* checkBox = (CButton*)GetDlgItem(IDC_CHECK_EDIT_MODE);

    pDoc->SetEditMode((BST_CHECKED == checkBox->GetCheck()) ? true : false);
}

void CDetailsView::OnBnClickedButtonAddWt()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    WorkTime_t    wt;
    CWorkpackage* wp = NULL;
    CString       trace;

	UpdateData();

    wt.startTime = GetTimeFromEditField(IDC_EDIT_START);
    wt.endTime   = GetTimeFromEditField(IDC_EDIT_END);
    if ((wt.startTime > 0) && (wt.endTime > 0))
        wt.duration  = wt.endTime - wt.startTime;
    else
        wt.duration = 0;

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    if (NULL != wp)
    {
        wp->AddWorktime(pDoc->GetCurrentDate(), wt);
    }
    pDoc->UpdateAllViews(NULL, (LPARAM)m_hWorkpackage);
	pDoc->SetModifiedFlag();
}

void CDetailsView::OnBnClickedButtonNote()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    CWorkpackage* wp;
    CWorkday* wd;

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    wd = wp->getWorkday(pDoc->GetCurrentDate());
    if (NULL == wd)
    {
        wd = wp->getPrevWorkday(pDoc->GetCurrentDate());
        if (NULL == wd)
        {
            wd = wp->getNextWorkday(pDoc->GetCurrentDate());
            if (NULL == wd)
            {
                CTime date = wp->GetFirstDay();
                wd = wp->getWorkday(date);
            }
        }
    }
    if (NULL == wd) return;
    m_noteDlg.SetNote(wd->getNote());
    m_noteDlg.SetDate(wd->getWdDate());
    m_noteDlg.SetOwner(this);
    if (IDOK == m_noteDlg.DoModal())
    {
        wd->setNote(m_noteDlg.GetNote());
	    pDoc->SetModifiedFlag();
    }
}

void CDetailsView::OnBnClickedButton2()
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    CTime lastDay;

    pDoc->GetWorkdayLimits(m_date, lastDay);
    if (m_date > 0)
    {
        CDateTimeCtrl* dateTimeCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1);
        dateTimeCtrl->SetTime(&m_date);
	    pDoc->SetCurrentDate(m_date);

	    pDoc->UpdateAllViews(this);
    }
}

LRESULT CDetailsView::OnNextNote(WPARAM, LPARAM)
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    CWorkpackage* wp;
    CWorkday* wd;

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    wd = wp->getNextWorkday(m_noteDlg.GetDate());
    if (NULL == wd) return TRUE;
    m_noteDlg.setWorkday(wd);

    return TRUE;
}

LRESULT CDetailsView::OnPrevNote(WPARAM, LPARAM)
{
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();
    CWorkpackage* wp;
    CWorkday* wd;

    pDoc->GetWorkpackage(m_hWorkpackage, wp);
    wd = wp->getPrevWorkday(m_noteDlg.GetDate());
    if (NULL == wd) return TRUE;
    m_noteDlg.setWorkday(wd);

    return TRUE;
}

LRESULT CDetailsView::OnSwitchWorkday(WPARAM, LPARAM)
{
    SYSTEMTIME   sysTime;
    FILETIME     fileTime;
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();

	CTime date = m_noteDlg.GetDate();
	pDoc->SetCurrentDate(date);
    date.GetAsSystemTime(sysTime);
    SystemTimeToFileTime(&sysTime, &fileTime);
    m_date_time = CFileTime(fileTime);

	pDoc->UpdateAllViews(this);
    UpdateData(FALSE);

    return TRUE;
}

void CDetailsView::OnBerichteTagesbericht()
{
    CWorkpackage* wp = NULL;
    CWorkday* wd = NULL;
    HANDLE hwp = NULL;
    POSITION pos = 0;
	CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();

    m_reportDlg.clear();
    do
    {
        wp = pDoc->GetWorkpackages(pos, hwp);
        if (NULL != wp)
        {
            wd = wp->getWorkday(pDoc->GetCurrentDate());
            if (NULL != wd)
            {
                m_reportDlg.setReportDate(wd->getDuration());
                m_reportDlg.setReportWp(wp->GetWpText()); 
                m_reportDlg.setReportNote(wd->getNote());
            }
        }
    }
    while (NULL != pos);
    m_reportDlg.DoModal();
}
