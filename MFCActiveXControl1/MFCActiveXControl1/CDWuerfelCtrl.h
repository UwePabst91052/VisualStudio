#pragma once
#include "afxwin.h"

// CDWuerfelCtrl.h : Declaration of the CCDWuerfelCtrl ActiveX Control class.


// CCDWuerfelCtrl : See CDWuerfelCtrl.cpp for implementation.

class CCDWuerfelCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCDWuerfelCtrl)

// Constructor
public:
	CCDWuerfelCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CCDWuerfelCtrl();

	DECLARE_OLECREATE_EX(CCDWuerfelCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCDWuerfelCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCDWuerfelCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCDWuerfelCtrl)		// Type name and misc status

	// Subclassed control support
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		eventidHabeGewuerfelt = 1L,
		dispidWuerfleZahl = 1L
	};
private:
	CBitmap* m_wuerfel[6];
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	int m_wurf;
protected:
	void WuerfleZahl(void);

	void HabeGewuerfelt(LONG zahl)
	{
		FireEvent(eventidHabeGewuerfelt, EVENT_PARAM(VTS_I4), zahl);
	}
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	int m_timer_id;
	int m_max_rotations;
public:
	virtual void OnBackColorChanged();
	virtual void OnForeColorChanged();
};

