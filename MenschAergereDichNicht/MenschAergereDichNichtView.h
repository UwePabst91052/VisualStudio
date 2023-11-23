// MenschAergereDichNichtView.h : Schnittstelle der Klasse CMADNView
//

#pragma once
#include "Field.h"
#include "Sprite.h"
#include "CDWuerfel.h"

#define CLR_VIEW_BACKGROUND	RGB(242,225,15)


class CPlayer;

class CMADNView : public CView
{
protected: // Nur aus Serialisierung erstellen
	CMADNView();
	DECLARE_DYNCREATE(CMADNView)

// Attribute
public:
	CMADNDoc* GetDocument() const;

// Vorgänge
public:
	void SetPlayerColor(COLORREF clrPlayer);

// Überschreibungen
public:
	virtual void OnDraw(CDC* pDC);  // Überschrieben, um diese Ansicht darzustellen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementierung
public:
	virtual ~CMADNView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// event sink map
	void OnHabeGewuerfelt(int number);
	DECLARE_EVENTSINK_MAP()

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSpriteStopped(WPARAM, LPARAM);
	afx_msg LRESULT OnNextPlayer(WPARAM, LPARAM);
	afx_msg LRESULT OnFinishGame(WPARAM, LPARAM);
	afx_msg void OnModeAutomatic();
	afx_msg void OnUpdateModeAuto(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	Move_t   m_moveSimu;
	bool     m_bMoveStarted;
    bool     m_bInitialized;
	BOOL     m_bAutoPlay;
	CBitmap* m_pBmpSpriteMove;
	int  m_SpritePosX;
	int  m_SpritePosY;
	int  m_SpritePosToX;
	int  m_SpritePosToY;
	CBitmap     m_bmpBackground;
	CBitmap     m_bmpCurrent;
	CSprite     m_Sprite;
	CDWuerfel    m_Würfel;
	static const int clientDimension;
private:
	void DrawArrow(CDC* pDC, CRect rcArrow, ArrowDir_t arrowDir, UINT nID);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // Debugversion in MenschAergereDichNichtView.cpp
inline CMADNDoc* CMADNView::GetDocument() const
   { return reinterpret_cast<CMADNDoc*>(m_pDocument); }
#endif

