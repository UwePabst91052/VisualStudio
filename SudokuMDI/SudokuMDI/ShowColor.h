#pragma once


// CShowColor

class CShowColor : public CStatic
{
	DECLARE_DYNAMIC(CShowColor)

public:
	CShowColor();
	virtual ~CShowColor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetColor(COLORREF color);
private:
	COLORREF myColor;
};


