#pragma once
#include "field.h"

class CFieldTarget : 
	public CField
{
	DECLARE_SERIAL(CFieldTarget)
public:
	CFieldTarget();
	CFieldTarget(Color_t color, CPoint ptNext, int pos);
	~CFieldTarget(void);
	virtual void DrawField(CDC *pDC, CRect rcField);
private:
	int myPosition;
	static TCHAR* myCharacters;
public:
	virtual void Serialize(CArchive& ar);
};
