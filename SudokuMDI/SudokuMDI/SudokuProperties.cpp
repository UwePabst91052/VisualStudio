// SudokuProperties.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuMDI.h"
#include "SudokuProperties.h"


// CSudokuProperties dialog

IMPLEMENT_DYNAMIC(CSudokuProperties, CPropertyPage)

CSudokuProperties::CSudokuProperties(UINT idCaption)
	: CPropertyPage(CSudokuProperties::IDD, idCaption)
    , m_nHoriIndent(0)
    , m_nVertIndent(0)
    , m_nFieldSize(0)
    , m_nCandLength(0)
    , m_nHoriIndentDP(0)
    , m_nVertIndentDP(0)
    , m_nFieldSizeDP(0)
    , m_nCandLengthDP(0)
    , m_nFontSizeDP(0)
	, m_bDisplayAlpha(TRUE)
	, m_nBoardAlignment(0)
	, m_backColor(RGB(255,255,255))
	, m_fillColor(RGB(255,255,255))
{

}

CSudokuProperties::~CSudokuProperties()
{
}

void CSudokuProperties::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HORI_INDENT, m_nHoriIndent);
	DDX_Text(pDX, IDC_EDIT_VERT_INDENT, m_nVertIndent);
	DDX_Text(pDX, IDC_EDIT_FIELD_SIZE, m_nFieldSize);
	DDX_Text(pDX, IDC_EDIT_CAND_LEN, m_nCandLength);
	DDX_Text(pDX, IDC_EDIT_HORI_INDENT_DP, m_nHoriIndentDP);
	DDX_Text(pDX, IDC_EDIT_VERT_INDENT_DP, m_nVertIndentDP);
	DDX_Text(pDX, IDC_EDIT_FIELD_SIZE_DP, m_nFieldSizeDP);
	DDX_Text(pDX, IDC_EDIT_CAND_LEN_DP, m_nCandLengthDP);
	DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_nFontSizeDP);
	DDX_Control(pDX, IDC_COMBO_ALIGN, m_BoardAlignment);
	DDX_Check(pDX, IDC_CHECK_ALPHA, m_bDisplayAlpha);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGN, m_nBoardAlignment);
	DDX_Control(pDX, IDC_STATIC_LABEL, m_LabelAlignment);
	DDX_Control(pDX, IDC_STATIC_VIEW_COLOR, myViewColor);
	DDX_Control(pDX, IDC_STATIC_VIEW_COLOR2, myScoreColor);
}


BEGIN_MESSAGE_MAP(CSudokuProperties, CPropertyPage)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_HORI_INDENT, IDC_EDIT_FONT_SIZE, &CSudokuProperties::OnEnChangeEditProperty)
	ON_BN_CLICKED(IDC_CHECK_ALPHA, &CSudokuProperties::OnBnClickedCheckAlpha)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGN, &CSudokuProperties::OnCbnSelchangeComboAlign)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_VIEW, &CSudokuProperties::OnBnClickedButtonColorView)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_SCORE, &CSudokuProperties::OnBnClickedButtonColorScore)
END_MESSAGE_MAP()


void CSudokuProperties::GetBoardProperties(SudokuProps_t& props)
{
    props.myHoriIndent		= m_nHoriIndent;
    props.myVertIndent		= m_nVertIndent;
    props.myFieldSize		= m_nFieldSize;
    props.myCandLength		= m_nCandLength;
    props.myHoriIndentDP	= m_nHoriIndentDP;
    props.myVertIndentDP	= m_nVertIndentDP;
    props.myFieldSizeDP		= m_nFieldSizeDP;
    props.myCandLengthDP	= m_nCandLengthDP;
    props.myFontSizeDP		= m_nFontSizeDP;
	props.myBoardAlignment	= m_nBoardAlignment;
	props.myDisplayAlpha	= m_bDisplayAlpha;
	props.myBackColor		= m_backColor;
	props.myFillColor		= m_fillColor;
}

void CSudokuProperties::SetBoardProperties(SudokuProps_t props)
{
    m_nHoriIndent		= props.myHoriIndent;
    m_nVertIndent		= props.myVertIndent;
    m_nFieldSize		= props.myFieldSize;
    m_nCandLength		= props.myCandLength;
    m_nHoriIndentDP		= props.myHoriIndentDP;
    m_nVertIndentDP		= props.myVertIndentDP;
    m_nFieldSizeDP		= props.myFieldSizeDP;
    m_nCandLengthDP		= props.myCandLengthDP;
    m_nFontSizeDP		= props.myFontSizeDP;
	m_nBoardAlignment	= props.myBoardAlignment;
	m_bDisplayAlpha		= props.myDisplayAlpha;
	m_backColor			= props.myBackColor;
	m_fillColor			= props.myFillColor;
}

// CSudokuProperties message handlers

BOOL CSudokuProperties::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	PROPSHEETPAGE psp;
	psp = GetPSP();
	CString szTitle = psp.pszTitle;
	if (szTitle == _T("Board 12x12") ||
        szTitle == _T("Board 8x8"))
	{
		m_BoardAlignment.EnableWindow();
		m_LabelAlignment.EnableWindow();
		m_BoardAlignment.AddString(L"Horizontal");
		m_BoardAlignment.AddString(L"Vertical");
		m_BoardAlignment.SetCurSel(m_nBoardAlignment);
	}

	if (szTitle == _T("Board 12x12") ||
		szTitle == _T("Board 16x16") ||
		szTitle == _T("Board 9x9"))
	{
		GetDlgItem(IDC_CHECK_ALPHA)->EnableWindow();
	}

	if (szTitle == _T("Board 9x9"))
	{
		m_bDisplayAlpha = FALSE;
	}

	CRect rcItem;
	m_BoardAlignment.GetWindowRect(&rcItem);

	myViewColor.SetColor(m_backColor);
	myScoreColor.SetColor(m_fillColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSudokuProperties::OnEnChangeEditProperty(UINT nId)
{
	SetModified();
}

void CSudokuProperties::OnBnClickedCheckAlpha()
{
	SetModified();
}

void CSudokuProperties::OnCbnSelchangeComboAlign()
{
	SetModified();
}

void CSudokuProperties::OnBnClickedButtonColorView()
{
	CColorDialog	dlg(m_backColor);

	dlg.m_cc.lpCustColors[0] = m_backColor;
	dlg.m_cc.lpCustColors[1] = m_fillColor;
	if (IDOK == dlg.DoModal())
	{
		m_backColor = dlg.GetColor();
		myViewColor.SetColor(m_backColor);
	}
}

void CSudokuProperties::OnBnClickedButtonColorScore()
{
	CColorDialog	dlg(m_fillColor);

	dlg.m_cc.lpCustColors[0] = m_backColor;
	dlg.m_cc.lpCustColors[1] = m_fillColor;
	if (IDOK == dlg.DoModal())
	{
		m_fillColor = dlg.GetColor();
		myScoreColor.SetColor(m_fillColor);
	}
}
