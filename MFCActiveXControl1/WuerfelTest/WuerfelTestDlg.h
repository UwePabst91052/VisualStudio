
// WuerfelTestDlg.h: Headerdatei
//

#pragma once
#include "cdwuerfelctrl1.h"


// CWuerfelTestDlg-Dialogfeld
class CWuerfelTestDlg : public CDialogEx
{
// Konstruktion
public:
	CWuerfelTestDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_WUERFELTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCdwuerfelctrl1 m_wuerfel;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_EVENTSINK_MAP()
	void HabeGewuerfeltCdwuerfelctrl1(long zahl);
	afx_msg void OnBnClickedButtonBackColor();
	afx_msg void OnBnClickedButtonForeColor();
	int m_zahl;
};
