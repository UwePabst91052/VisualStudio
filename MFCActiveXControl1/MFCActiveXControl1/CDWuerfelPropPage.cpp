// CDWuerfelPropPage.cpp : Implementation of the CCDWuerfelPropPage property page class.

#include "stdafx.h"
#include "MFCActiveXControl1.h"
#include "CDWuerfelPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCDWuerfelPropPage, COlePropertyPage)

// Message map

BEGIN_MESSAGE_MAP(CCDWuerfelPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCDWuerfelPropPage, "MFCACTIVEXCONT.CDWuerfelPropPage.1",
	0xfe0acdc3, 0x88d7, 0x444c, 0xae, 0xbd, 0xd3, 0x74, 0xe6, 0xbc, 0x5a, 0xc2)

// CCDWuerfelPropPage::CCDWuerfelPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCDWuerfelPropPage

BOOL CCDWuerfelPropPage::CCDWuerfelPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MFCACTIVEXCONTROL1_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CCDWuerfelPropPage::CCDWuerfelPropPage - Constructor

CCDWuerfelPropPage::CCDWuerfelPropPage() :
	COlePropertyPage(IDD, IDS_MFCACTIVEXCONTROL1_PPG_CAPTION)
{
}

// CCDWuerfelPropPage::DoDataExchange - Moves data between page and properties

void CCDWuerfelPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CCDWuerfelPropPage message handlers
