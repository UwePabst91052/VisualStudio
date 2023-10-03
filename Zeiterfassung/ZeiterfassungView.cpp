// ZeiterfassungView.cpp : Implementierung der Klasse CZeiterfassungView
//

#include "stdafx.h"
#include "Zeiterfassung.h"

#include "ZeiterfassungDoc.h"
#include "ZeiterfassungView.h"
#include "Workpackage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZeiterfassungView

IMPLEMENT_DYNCREATE(CZeiterfassungView, CListView)

BEGIN_MESSAGE_MAP(CZeiterfassungView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CZeiterfassungView::OnNMClick)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DATEI_KONVERTIEREN, &CZeiterfassungView::OnDateiKonvertieren)
END_MESSAGE_MAP()

// CZeiterfassungView-Erstellung/Zerstörung

CZeiterfassungView::CZeiterfassungView()
: m_item(-1)
, m_sumLineActive(false)
{
	// TODO: Hier Code zur Konstruktion einfügen

}

CZeiterfassungView::~CZeiterfassungView()
{
}

BOOL CZeiterfassungView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CListView::PreCreateWindow(cs);
}

void CZeiterfassungView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	m_item = -1;
}


// CZeiterfassungView-Diagnose

#ifdef _DEBUG
void CZeiterfassungView::AssertValid() const
{
	CListView::AssertValid();
}

void CZeiterfassungView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CZeiterfassungDoc* CZeiterfassungView::GetDocument() const // Nicht-Debugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZeiterfassungDoc)));
	return (CZeiterfassungDoc*)m_pDocument;
}
#endif //_DEBUG


// CZeiterfassungView-Meldungshandler

int CZeiterfassungView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= LVS_REPORT;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetListCtrl().InsertColumn(0, _T("Tätigkeit"), LVCFMT_LEFT, 300, 0);
	GetListCtrl().InsertColumn(1, _T("Anfang"), LVCFMT_LEFT, 70, 1);
	GetListCtrl().InsertColumn(2, _T("Ende"), LVCFMT_LEFT, 70, 2);
	GetListCtrl().InsertColumn(3, _T("Dauer"), LVCFMT_LEFT, 70, 3);

	return 0;
}

void CZeiterfassungView::AddWorkPackage(HANDLE hwp, CWorkpackage* wp, CTime date)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.state = 0;
	int index = GetListCtrl().GetItemCount();
	lvi.iItem = index;
	lvi.iSubItem = 0;
	lvi.lParam = (LPARAM)hwp;
	lvi.pszText = (LPTSTR)(PCTSTR)wp->GetWpText();

	if (wp->isWorktimeRunning(date))
	{
		if (index != m_item)
		{
			m_item = index;
			GetDocument()->UpdateAllViews(this, (LPARAM)hwp);
		}
		lvi.mask |= LVIF_STATE;
		lvi.state |= LVIS_SELECTED;
		lvi.stateMask = LVIS_SELECTED;
	}
	GetListCtrl().InsertItem(&lvi);
	GetListCtrl().SetItemData(index, (DWORD)hwp);
}

void CZeiterfassungView::AddSumLine()
{
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.state = 0;
	int index = GetListCtrl().GetItemCount();
	lvi.iItem = index;
	lvi.iSubItem = 0;
    lvi.pszText = _T("an diesem Tag gearbeitet:");
    GetListCtrl().InsertItem(&lvi);
    m_sumLineActive = true;
}

void CZeiterfassungView::UpdateSumLine(CTimeSpan& sumDuration)
{
    if (sumDuration == 0) return;
    LVITEM lvi;
    lvi.mask = LVIF_TEXT;
	lvi.state = 0;
	int index = GetListCtrl().GetItemCount();
	lvi.iItem = index - 1;
    lvi.iSubItem = 3;
    CString line = sumDuration.Format(_T("%H:%M"));
    lvi.pszText = (LPTSTR)(PCTSTR)line;
    GetListCtrl().SetItem(&lvi);
}

void CZeiterfassungView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CWorkpackage* wpckg = NULL;
	CZeiterfassungDoc* pDoc = GetDocument();

	CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();

	// Delete list
	GetListCtrl().DeleteAllItems();

	// read all workpackages of current date
	POSITION  pos = NULL;
	HANDLE    hwp = NULL;
	bool      noRunningTimes = true;
    m_sumLineActive = false;
	do
	{
		wpckg = pDoc->GetWorkpackages(pos, hwp);
		if (NULL != wpckg)
		{
			TRACE1("Update List: %x\n", hwp);
			AddWorkPackage(hwp, wpckg, pDoc->GetCurrentDate());
			if (wpckg->isWorktimeRunning(pDoc->GetCurrentDate()))
			{
				noRunningTimes = false;
			}
			UpdateWpTimes(WPTM_START | WPTM_END | WPTM_DURATION, hwp);
		}
	}
	while ((NULL != wpckg) && (NULL != pos));
	if (noRunningTimes) m_item = -1;
    if (pDoc->GetWorkdaySum() > 0)
    {
        AddSumLine();
        UpdateWpTimes(WPTM_SUM, NULL);
    }
	pFrame->SetActiveView(this);
}

void CZeiterfassungView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	int item = pNMItemActivate->iItem;
	if (item < 0) return;

	HANDLE hwp_prev;
	HANDLE hwp = (HANDLE)GetListCtrl().GetItemData(item);
	TRACE1("Select workpackage: %x\n", hwp);
	CZeiterfassungDoc* pDoc = GetDocument();

    if (NULL == hwp) return;

	if (m_item < 0)
	{
		pDoc->BeginLogging(hwp);
	}
	else if (m_item != item)
	{
		// finish time logging for previous workpackage
		hwp_prev = (HANDLE)GetListCtrl().GetItemData(m_item);
		pDoc->EndLogging(hwp_prev);

		// begin time logging for new item
		pDoc->BeginLogging(hwp);
	}
	//else
	//{
	//	pDoc->EndLogging(hwp);
	//}

	pDoc->UpdateAllViews(this, (LPARAM)hwp);

	// store current selection
	m_item = item;

	*pResult = 0;
}

void CZeiterfassungView::OnDateiKonvertieren()
{
	CFileDialog dlgFile(TRUE, _T(".xml"));
	dlgFile.DoModal();

	CString fullPath = dlgFile.GetPathName();

	CStdioFile * file = new CStdioFile(fullPath, CFile::modeWrite | CFile::modeCreate);
	CArchive archive(file, CArchive::store);

	CZeiterfassungDoc* pDoc = GetDocument();
	pDoc->StoreWorkpackagesXml(archive);

	archive.Close();
	file->Close();
}

void CZeiterfassungView::UpdateWpTimes(UINT mask, HANDLE hwp)
{
	LVFINDINFO  lvfi;
	LVITEM      lvi;
	CZeiterfassungDoc* pDoc = GetDocument();
	CWorkpackage* wp;
	if (pDoc->GetWorkpackage(hwp, wp))
	{
		lvfi.flags = LVFI_PARAM;
		lvfi.lParam = (LPARAM)hwp;
		int index = GetListCtrl().FindItem(&lvfi);

		lvi.mask = LVIF_TEXT;
		lvi.iItem = index;
		if (mask & WPTM_START)
		{
			lvi.iSubItem = 1;
			lvi.pszText = (LPTSTR)(PCTSTR)wp->getStartTime(pDoc->GetCurrentDate());
			GetListCtrl().SetItem(&lvi);
			lvi.iSubItem = 2;
			lvi.pszText = _T("");
			GetListCtrl().SetItem(&lvi);
		}
		if (mask & WPTM_END)
		{
			lvi.iSubItem = 2;
            lvi.pszText = (LPTSTR)(PCTSTR)wp->getEndTime(pDoc->GetCurrentDate());;
			GetListCtrl().SetItem(&lvi);
		}
		if (mask & WPTM_DURATION)
		{
			lvi.iSubItem = 3;
			lvi.pszText = (LPTSTR)(PCTSTR)wp->getDuration(pDoc->GetCurrentDate());
			GetListCtrl().SetItem(&lvi);
		}
	}
    if (mask & WPTM_SUM)
    {
        if (m_sumLineActive)
        {
            UpdateSumLine(pDoc->GetWorkdaySum());
        }
    }
}

void CZeiterfassungView::SelectWorkpackage(HANDLE hwp)
{
	LVFINDINFO  lvfi;
	LVITEM      lvi;

	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = (LPARAM)hwp;
	int index = GetListCtrl().FindItem(&lvfi);

	lvi.iItem = index;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_STATE;
	lvi.state = LVIS_SELECTED;
	lvi.stateMask = LVIS_SELECTED;
	GetListCtrl().SetItem(&lvi);
	dynamic_cast<CFrameWnd*>(AfxGetMainWnd())->SetActiveView(this);
}

void CZeiterfassungView::OnDraw(CDC* pDC)
{
    CZeiterfassungDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    CSize size = pDC->GetWindowExt();

    CWorkpackage* wp = NULL;
    CWorkday*     wd = NULL;
    HANDLE        hwp;
    TEXTMETRIC    tm;
    POSITION      posWp = NULL;
    POSITION      posWd = NULL;
    int           yPos = 500;
    int           printedLines = 1;
    int           countNoteLines = 0;
    int           printMax = m_startLinePage + 50;

    pDC->GetTextMetrics(&tm);
    yPos = 500;
    do
    {
        wp = pDoc->GetWorkpackages(posWp, hwp);
        if (NULL != wp)
        {
            //CString strPrintedLines;
            //strPrintedLines.Format(_T("%d"), printedLines);
            //pDC->TextOutW(0, yPos, strPrintedLines);
            if ((printedLines >= m_startLinePage) && (printedLines < printMax))
            {
                pDC->TextOut(500, yPos, wp->GetWpText());
                yPos += tm.tmHeight + (tm.tmHeight / 2);
            }
            printedLines ++;
            do
            {
                wd = wp->GetWorkdays(posWd);
                if (NULL != wd)
                {
                    if ((printedLines >= m_startLinePage) && (printedLines < printMax))
                    {
                        pDC->TextOut(750, yPos, wd->getDate());
                        pDC->TextOut(1500, yPos, wd->getDuration());
                        yPos += tm.tmHeight;
                    }
                    printedLines++;
                    CStringArray& noteLines = wd->getNote();
                    countNoteLines = noteLines.GetCount();
                    if (countNoteLines > 0)
                    {
                        if ((printedLines >= m_startLinePage) && (printedLines < printMax))
                        {
                            yPos += tm.tmHeight;
                        }
                        printedLines++;
                        for (int i = 0; i < countNoteLines; i++)
                        {
                            if ((printedLines >= m_startLinePage) && (printedLines < printMax))
                            {
                                pDC->TextOut(1000, yPos, noteLines[i]);
                                yPos += tm.tmHeight;
                            }
                            printedLines++;
                        }
                        if ((printedLines >= m_startLinePage) && (printedLines < printMax))
                            yPos += tm.tmHeight;
                        printedLines ++;
                    }
                }
            }
            while ((NULL != posWd) && (NULL != wd));
            if ((printedLines >= m_startLinePage) && (printedLines < printMax))
            {
                yPos += tm.tmHeight;
            }
            printedLines ++;
        }
    }
    while ((NULL != posWp) && (NULL != wp));
}


BOOL CZeiterfassungView::OnPreparePrinting(CPrintInfo* pInfo)
{
    CZeiterfassungDoc* pDoc = GetDocument();
    int lines = pDoc->GetCountLinesDoc();
    int pages = lines / 50;
    pages += ((pages * 50) < lines ? 1 : 0);

    pInfo->SetMaxPage(pages);

    return CListView::DoPreparePrinting(pInfo);
}

void CZeiterfassungView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{


    CListView::OnBeginPrinting(pDC, pInfo);
}

void CZeiterfassungView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // TODO: Add your specialized code here and/or call the base class

    CListView::OnEndPrinting(pDC, pInfo);
}

void CZeiterfassungView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    CSize size = pDC->GetWindowExt();
    CZeiterfassungDoc* pDoc = (CZeiterfassungDoc*)GetDocument();

    CString strPageDesc;
    strPageDesc = _T("Seite %u von Seiten %u - %u");
    pDoc->PrintHeader(pDC, CPoint(1000, 100));
    m_startLinePage = 50 * (pInfo->m_nCurPage - 1) + 1;
    pDoc->PrintFooter(pDC, 
                      strPageDesc, 
                      CPoint(2000, 6600), 
                      pInfo->m_nCurPage, 
                      pInfo->GetFromPage(),
                      pInfo->GetMaxPage());
    CListView::OnPrint(pDC, pInfo);
}
