#pragma once
#include "field.h"

class CFieldBase :
	public CField
{
	DECLARE_SERIAL(CFieldBase)
public:
	CFieldBase();
	CFieldBase(Color_t, CPoint);
	~CFieldBase(void);
	virtual void DrawField(CDC *pDC, CRect rcField);
	virtual void Serialize(CArchive& ar);
};
