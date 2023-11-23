// CDWuerfelCtrl.cpp : Implementation of the CCDWuerfelCtrl ActiveX Control class.

#include "stdafx.h"
#include "MFCActiveXControl1.h"
#include "CDWuerfelCtrl.h"
#include "CDWuerfelPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCDWuerfelCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CCDWuerfelCtrl, COleControl)
	ON_MESSAGE(OCM_COMMAND, &CCDWuerfelCtrl::OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// Dispatch map

BEGIN_DISPATCH_MAP(CCDWuerfelCtrl, COleControl)
	DISP_FUNCTION_ID(CCDWuerfelCtrl, "WuerfleZahl", dispidWuerfleZahl, WuerfleZahl, VT_EMPTY, VTS_NONE)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
END_DISPATCH_MAP()

// Event map

BEGIN_EVENT_MAP(CCDWuerfelCtrl, COleControl)
	EVENT_CUSTOM_ID("HabeGewuerfelt", eventidHabeGewuerfelt, HabeGewuerfelt, VTS_I4)
END_EVENT_MAP()

// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCDWuerfelCtrl, 1)
	PROPPAGEID(CCDWuerfelPropPage::guid)
END_PROPPAGEIDS(CCDWuerfelCtrl)

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCDWuerfelCtrl, "MFCACTIVEXCONTRO.CDWuerfelCtrl.1",
	0x46dea3c, 0x55c0, 0x4c75, 0x82, 0x9e, 0x94, 0xc5, 0x51, 0x5, 0xf3, 0xb3)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CCDWuerfelCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID IID_DMFCActiveXControl1 = { 0x128B1BE4, 0xF9BF, 0x4BF4, { 0xB9, 0x11, 0xF2, 0x28, 0x32, 0x7A, 0x5B, 0x64 } };
const IID IID_DMFCActiveXControl1Events = { 0x8CB960EB, 0x3788, 0x4DF0, { 0x9E, 0x15, 0x7, 0xEC, 0xA0, 0xBA, 0x75, 0xD5 } };

// Control type information

static const DWORD _dwMFCActiveXControl1OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCDWuerfelCtrl, IDS_MFCACTIVEXCONTROL1, _dwMFCActiveXControl1OleMisc)

// CCDWuerfelCtrl::CCDWuerfelCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCDWuerfelCtrl

BOOL CCDWuerfelCtrl::CCDWuerfelCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_MFCACTIVEXCONTROL1,
			IDB_MFCACTIVEXCONTROL1,
			afxRegApartmentThreading,
			_dwMFCActiveXControl1OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CCDWuerfelCtrl::CCDWuerfelCtrl - Constructor

CCDWuerfelCtrl::CCDWuerfelCtrl()
	: m_wurf(0)
	, m_timer_id(0)
	, m_max_rotations(0)
{
	InitializeIIDs(&IID_DMFCActiveXControl1, &IID_DMFCActiveXControl1Events);

	for (int i=0; i<6; i++)
	{
		m_wuerfel[i] = new CBitmap;
		m_wuerfel[i]->LoadBitmapW(IDB_BITMAP_EINS + i);
	}
}

// CCDWuerfelCtrl::~CCDWuerfelCtrl - Destructor

CCDWuerfelCtrl::~CCDWuerfelCtrl()
{
	for (int i=0; i<6; i++)
	{
		delete m_wuerfel[i];
	}
}

// CCDWuerfelCtrl::OnDraw - Drawing function

void CCDWuerfelCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	int sizeSquare = rcBounds.Width() / 3 * 2;
	int posX = rcBounds.Width() / 2 - (sizeSquare / 2);
	int posY = rcBounds.Height() / 2 - (sizeSquare / 2);

	CBrush brCircle(GetForeColor());
	CDC dcMemory;

	dcMemory.CreateCompatibleDC(pdc);
	CBitmap* oldBmp = dcMemory.SelectObject(m_wuerfel[m_wurf]);

	CBrush* oldBrush = pdc->SelectObject(&brCircle);

	pdc->SetDCPenColor(RGB(0,0,0));

	pdc->FillSolidRect(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(), GetBackColor());
	pdc->Ellipse(&rcBounds);
	pdc->TransparentBlt(posX,posY,sizeSquare,sizeSquare,&dcMemory,0,0,48,48,RGB(255,255,255));

	pdc->SelectObject(oldBmp);
	pdc->SelectObject(oldBrush);
	dcMemory.SelectObject(oldBmp);

	//DoSuperclassPaint(pdc, rcBounds);
}

// CCDWuerfelCtrl::DoPropExchange - Persistence support

void CCDWuerfelCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}


// CCDWuerfelCtrl::OnResetState - Reset control to default state

void CCDWuerfelCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


// CCDWuerfelCtrl::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL CCDWuerfelCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}

// CCDWuerfelCtrl::IsSubclassedControl - This is a subclassed control

BOOL CCDWuerfelCtrl::IsSubclassedControl()
{
	return TRUE;
}

// CCDWuerfelCtrl::OnOcmCommand - Handle command messages

LRESULT CCDWuerfelCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	// TODO: Switch on wNotifyCode here.

	return 0;
}


// CCDWuerfelCtrl message handlers


int CCDWuerfelCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	return 0;
}


void CCDWuerfelCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{

	COleControl::OnLButtonDown(nFlags, point);
}



void CCDWuerfelCtrl::WuerfleZahl(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	srand((int)time(NULL));

	m_timer_id = SetTimer(5, 200, NULL);
	
	m_max_rotations = rand() % 8 + 2;

}


void CCDWuerfelCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timer_id)
	{
		m_wurf = rand() % 6;
		if (!(m_max_rotations--))
		{
			HabeGewuerfelt(m_wurf+1);
			KillTimer(m_timer_id);
		}
	}

	COleControl::OnTimer(nIDEvent);

	InvalidateControl();
}


void CCDWuerfelCtrl::OnBackColorChanged()
{
	
	COleControl::OnBackColorChanged();
}


void CCDWuerfelCtrl::OnForeColorChanged()
{

	COleControl::OnForeColorChanged();
}
