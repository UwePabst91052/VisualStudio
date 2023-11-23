#pragma once

// CDWuerfelPropPage.h : Declaration of the CCDWuerfelPropPage property page class.


// CCDWuerfelPropPage : See CDWuerfelPropPage.cpp for implementation.

class CCDWuerfelPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCDWuerfelPropPage)
	DECLARE_OLECREATE_EX(CCDWuerfelPropPage)

// Constructor
public:
	CCDWuerfelPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MFCACTIVEXCONTROL1 };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

