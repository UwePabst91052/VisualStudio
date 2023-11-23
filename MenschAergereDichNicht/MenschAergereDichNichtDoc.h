// MenschAergereDichNichtDoc.h : Schnittstelle der Klasse CMADNDoc
//


#pragma once

class CPlayer;

class CMADNDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CMADNDoc();
	DECLARE_DYNCREATE(CMADNDoc)

// Attribute
public:

// Vorgänge
public:
	void SetPlayersStartPosition();

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CMADNDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg void OnCommandWuerfel(UINT id);
	afx_msg void OnCommandPlayer(UINT id);
	DECLARE_MESSAGE_MAP()

private: 
	CPlayer     *m_Players[4];
	static const Color_t  m_PlayOrder[4];
	Color_t      m_idxPlayer;
	CFieldMap	 myFieldMap;
	Move_t       myMovePlayer;
	Move_t       myMoveHitStone;
	static const FieldDescription_t myFieldDescriptions[];
	static const int maxDescriptions;
public:
	COLORREF GetPlayerColor(void);
	void HitStone(Color_t clrStone);
	bool InvestigateMove(int noOfSteps);
	CFieldMap* GetFieldMap(void);
	void SetHitMove(Move_t moveHit)
	{
		myMoveHitStone = moveHit;
	}
	void SetPlayerMove(Move_t movePlayer)
	{
        TRACE_FUNCTION;
		myMovePlayer = movePlayer;
	}

	bool MoveControler(void);
	void FinishMove(void);
	CPlayer* GetPlayer(Color_t clrPlayer);
    bool PlayerHasFinished(void);
};


