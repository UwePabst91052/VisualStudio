#pragma once
#include "atltypes.h"

// CField command target

#define CLR_GELB	RGB(255,255,0)
#define CLR_GRUEN	RGB(0,255,0)
#define CLR_ROT		RGB(255,0,0)
#define CLR_SCHWARZ	RGB(0,0,0)
#define CLR_WEISS	RGB(255,255,255)

class CField : public CObject
{
	DECLARE_DYNAMIC(CField)
public:
	CField();
	CField(Color_t color, CPoint ptNext, CPoint ptAlt, Color_t clrAlt);
	virtual ~CField();
	// Abstrace function. Draw field in derived class
	virtual void DrawField(CDC *pDC, CRect rcField) = 0;
	void SetStone(CBitmap* pBmpStone, Color_t clrStone);
	CBitmap* RemoveStone(CDC* pDC = NULL, CBitmap* pBmpBackground = NULL);
protected:
	COLORREF myInnerColor;
	COLORREF myBorderColor;
	CRect myFieldRect;
	CRect myInnerRect;
	struct Connector_t {
		bool	connUp;
		bool	connLeft;
		bool	connRight;
		bool	connDown;
	} myConnectors;
	int myBorderWidth;
	CBitmap* myBmpStone;
	Color_t  myStoneColor;
public:

	bool GetSpriteRect(CRect& rcSprite)
	{
		rcSprite = myInnerRect;
		return (NULL != myBmpStone);
	}

	bool isClickedIn(CPoint ptPos)
	{
		return (myFieldRect.PtInRect(ptPos) ? true : false);
	}

	CPoint& GetNextField(Color_t clrStone)
	{
		if (m_clrAlt == clrStone)
		{
			return m_ptNextAlt;
		}
		return m_ptNextMain;
	}

	Color_t GetStoneColor()
	{
		return myStoneColor;
	}

private:
	CPoint  m_ptNextMain;
	CPoint  m_ptNextAlt;
	Color_t m_clrAlt;
	static const UINT bitmapIDs[];
public:
	virtual void Serialize(CArchive& ar);
};


