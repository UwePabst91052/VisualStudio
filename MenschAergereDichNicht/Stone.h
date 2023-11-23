#pragma once

// CStone command target

class CStone : public CObject
{
	DECLARE_SERIAL(CStone)
public:
	CStone();
	CStone(CPoint, Color_t);
	virtual ~CStone();

	int GetTargetField(CFieldMap*, Move_t&, int);
	void MoveToHome(void)
	{
		m_ptCurrPos = m_ptHomePos;
	}

	bool IsAtHome()
	{
		return m_bAtHome;
	}

	CPoint GetCurrentPos()
	{
		return m_ptCurrPos;
	}

	CPoint GetHomePos()
	{
		return m_ptHomePos;
	}

	void SetNewPos(CPoint ptPos, BOOL isHomeField)
	{
		m_ptCurrPos = ptPos;
        m_bAtHome = isHomeField ? true : false;
	}

private:
	CPoint m_ptHomePos;
	CPoint m_ptCurrPos;
	bool   m_bAtHome;
	Color_t m_clrStone;
public:
	virtual void Serialize(CArchive& ar);
};


