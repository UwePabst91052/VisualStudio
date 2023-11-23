#pragma once

class CStone;
// CPlayer command target

class CPlayer : public CObject
{
	DECLARE_SERIAL(CPlayer)
public:
	CPlayer();
	CPlayer(CMADNDoc* pDoc, COLORREF clrStone, Color_t color, UINT idBitmap);
	virtual ~CPlayer();

	// methods
public:
	void SetStoneOnField(CFieldMap* pFields, CPoint ptField, int  idStone);
	void FinishMove(Move_t move);

	// members
private:
	CStone*   m_stones[4];
	COLORREF  m_clrStone;
	Color_t   m_Color;
	UINT      m_idBitmap;
	CBitmap   m_bmpStone;
	Move_t    m_Moves[4];
	int       m_Evaluation[4];
	Move_t    m_Move;
	UINT      m_TryCount;
	CMADNDoc* m_docParent;
public:
	bool MoveStone(CFieldMap* pFieldMap, int noOfSteps);

	COLORREF GetStoneColor(void)
	{
		return m_clrStone;
	}

	Move_t& GetMoveValues()
	{
		return m_Move;
	}
public:
	void ExecuteStoneHit(Color_t clrStone, Move_t moveHit);
	void GetHitMove(Move_t move, CFieldMap* pFieldMap, Move_t& moveHit);
	virtual void Serialize(CArchive& ar);
	void SetStonesOnFields(CFieldMap* pFieldMap, CMADNDoc* pDoc);
    bool areAllAtHome(void);
};


