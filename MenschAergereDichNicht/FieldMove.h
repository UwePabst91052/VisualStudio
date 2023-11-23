#pragma once
#include "field.h"

class CFieldMove :
	public CField
{
	DECLARE_SERIAL(CFieldMove)
public:
	CFieldMove();
	CFieldMove(CPoint ptNext, CPoint ptAlt, Color_t clrAlt, bool up, bool left, bool down, bool right);
	~CFieldMove(void);
	virtual void DrawField(CDC *pDC, CRect rcField);
private:
	int myConnectorLength;
public:
	virtual void Serialize(CArchive& ar);
};
