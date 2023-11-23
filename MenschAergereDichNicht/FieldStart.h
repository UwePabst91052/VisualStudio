#pragma once
#include "field.h"

class CFieldStart :
	public CField
{
	DECLARE_SERIAL(CFieldStart)
public:
	CFieldStart();
	CFieldStart(Color_t color, CPoint ptNext, bool up, bool left, bool down, bool rigth);
	~CFieldStart(void);
	virtual void DrawField(CDC *pDC, CRect rcField);
private:
	int myConnectorLength;
public:
	virtual void Serialize(CArchive& ar);
};
