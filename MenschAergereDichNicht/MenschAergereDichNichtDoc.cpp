// MenschAergereDichNichtDoc.cpp : Implementierung der Klasse CMADNDoc
//

#include "stdafx.h"

#include "MenschAergereDichNicht.h"
#include "FieldBase.h"
#include "FieldStart.h"
#include "FieldMove.h"
#include "FieldTarget.h"
#include "MenschAergereDichNichtDoc.h"
#include "MenschAergereDichNichtView.h"
#include "Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_PLAYERS 4

const Color_t CMADNDoc::m_PlayOrder[] = {gelb, gruen, rot, schwarz};
const int CMADNDoc::maxDescriptions = 72;
const FieldDescription_t CMADNDoc::myFieldDescriptions[] = 
{
	/*01*/{BaseField, CPoint(0,0), CPoint(0,4), CPoint(0,0), gelb, weiss, false, false, false, false, 0},
	/*02*/{BaseField, CPoint(0,1), CPoint(0,4), CPoint(0,0), gelb, weiss, false, false, false, false, 0},
	/*03*/{BaseField, CPoint(1,0), CPoint(0,4), CPoint(0,0), gelb, weiss, false, false, false, false, 0},
	/*04*/{BaseField, CPoint(1,1), CPoint(0,4), CPoint(0,0), gelb, weiss, false, false, false, false, 0},

	/*05*/{BaseField, CPoint(9,0),  CPoint(6,0), CPoint(0,0), gruen, weiss, false, false, false, false, 0},
	/*06*/{BaseField, CPoint(9,1),  CPoint(6,0), CPoint(0,0), gruen, weiss, false, false, false, false, 0},
	/*07*/{BaseField, CPoint(10,0), CPoint(6,0), CPoint(0,0), gruen, weiss, false, false, false, false, 0},
	/*08*/{BaseField, CPoint(10,1), CPoint(6,0), CPoint(0,0), gruen, weiss, false, false, false, false, 0},

	/*09*/{BaseField, CPoint(0,9),  CPoint(4,10), CPoint(0,0), schwarz, weiss, false, false, false, false, 0},
	/*10*/{BaseField, CPoint(0,10), CPoint(4,10), CPoint(0,0), schwarz, weiss, false, false, false, false, 0},
	/*11*/{BaseField, CPoint(1,9),  CPoint(4,10), CPoint(0,0), schwarz, weiss, false, false, false, false, 0},
	/*12*/{BaseField, CPoint(1,10), CPoint(4,10), CPoint(0,0), schwarz, weiss, false, false, false, false, 0},

	/*13*/{BaseField, CPoint(9,9),   CPoint(10,6), CPoint(0,0), rot, weiss, false, false, false, false, 0},
	/*14*/{BaseField, CPoint(9,10),  CPoint(10,6), CPoint(0,0), rot, weiss, false, false, false, false, 0},
	/*15*/{BaseField, CPoint(10,9),  CPoint(10,6), CPoint(0,0), rot, weiss, false, false, false, false, 0},
	/*16*/{BaseField, CPoint(10,10), CPoint(10,6), CPoint(0,0), rot, weiss, false, false, false, false, 0},

	/*17*/{StartField, CPoint(0,4),  CPoint(1,4), CPoint(0,0), gelb,    weiss, false, true,  true,  false, 0},
	/*18*/{StartField, CPoint(6,0),  CPoint(6,1), CPoint(0,0), gruen,   weiss, false, false, true,  true,  0},
	/*19*/{StartField, CPoint(4,10), CPoint(4,9), CPoint(0,0), schwarz, weiss, true,  true,  false, false, 0},
	/*20*/{StartField, CPoint(10,6), CPoint(9,6), CPoint(0,0), rot,     weiss, true,  false, false, true,  0},

	/*21*/{MoveField, CPoint(1,4), CPoint(2,4), CPoint(0,0), weiss, weiss, false, true, false, true, 0},
	/*22*/{MoveField, CPoint(2,4), CPoint(3,4), CPoint(0,0), weiss, weiss, false, true, false, true, 0},
	/*23*/{MoveField, CPoint(3,4), CPoint(4,4), CPoint(0,0), weiss, weiss, false, true, false, true, 0},
	/*24*/{MoveField, CPoint(4,4), CPoint(4,3), CPoint(0,0), weiss, weiss, true, false, false, true, 0},
	/*25*/{MoveField, CPoint(4,3), CPoint(4,2), CPoint(0,0), weiss, weiss, true, false, true, false, 0},
	/*26*/{MoveField, CPoint(4,2), CPoint(4,1), CPoint(0,0), weiss, weiss, true, false, true, false, 0},
	/*27*/{MoveField, CPoint(4,1), CPoint(4,0), CPoint(0,0), weiss, weiss, true, false, true, false, 0},
	/*28*/{MoveField, CPoint(4,0), CPoint(5,0), CPoint(0,0), weiss, weiss, false, true, true, false, 0},
	/*29*/{MoveField, CPoint(5,0), CPoint(6,0), CPoint(5,1), weiss, gruen, false, true, false, true, 0},

	/*30*/{MoveField, CPoint(6,1),  CPoint(6,2),  CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*31*/{MoveField, CPoint(6,2),  CPoint(6,3),  CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*32*/{MoveField, CPoint(6,3),  CPoint(6,4),  CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*33*/{MoveField, CPoint(6,4),  CPoint(7,4),  CPoint(0,0), weiss, weiss, true,  true,  false, false, 0},
	/*34*/{MoveField, CPoint(7,4),  CPoint(8,4),  CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*35*/{MoveField, CPoint(8,4),  CPoint(9,4),  CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*36*/{MoveField, CPoint(9,4),  CPoint(10,4), CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*37*/{MoveField, CPoint(10,4), CPoint(10,5), CPoint(0,0), weiss, weiss, false, false, true,  true,  0},
	/*38*/{MoveField, CPoint(10,5), CPoint(10,6), CPoint(9,5), weiss, rot,   true,  false, true,  false, 0},

	/*39*/{MoveField, CPoint(9,6),  CPoint(8,6),  CPoint(0,0), weiss, weiss,   false, true,  false, true,  0},
	/*40*/{MoveField, CPoint(8,6),  CPoint(7,6),  CPoint(0,0), weiss, weiss,   false, true,  false, true,  0},
	/*41*/{MoveField, CPoint(7,6),  CPoint(6,6),  CPoint(0,0), weiss, weiss,   false, true,  false, true,  0},
	/*42*/{MoveField, CPoint(6,6),  CPoint(6,7),  CPoint(0,0), weiss, weiss,   false, true,  true,  false, 0},
	/*43*/{MoveField, CPoint(6,7),  CPoint(6,8),  CPoint(0,0), weiss, weiss,   true,  false, true,  false, 0},
	/*44*/{MoveField, CPoint(6,8),  CPoint(6,9),  CPoint(0,0), weiss, weiss,   true,  false, true,  false, 0},
	/*45*/{MoveField, CPoint(6,9),  CPoint(6,10), CPoint(0,0), weiss, weiss,   true,  false, true,  false, 0},
	/*46*/{MoveField, CPoint(6,10), CPoint(5,10), CPoint(0,0), weiss, weiss,   true,  false, false, true,  0},
	/*47*/{MoveField, CPoint(5,10), CPoint(4,10), CPoint(5,9), weiss, schwarz, false, true,  false, true,  0},

	/*48*/{MoveField, CPoint(4,9), CPoint(4,8), CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*49*/{MoveField, CPoint(4,8), CPoint(4,7), CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*50*/{MoveField, CPoint(4,7), CPoint(4,6), CPoint(0,0), weiss, weiss, true,  false, true,  false, 0},
	/*51*/{MoveField, CPoint(4,6), CPoint(3,6), CPoint(0,0), weiss, weiss, false, false, true,  true,  0},
	/*52*/{MoveField, CPoint(3,6), CPoint(2,6), CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*53*/{MoveField, CPoint(2,6), CPoint(1,6), CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*54*/{MoveField, CPoint(1,6), CPoint(0,6), CPoint(0,0), weiss, weiss, false, true,  false, true,  0},
	/*55*/{MoveField, CPoint(0,6), CPoint(0,5), CPoint(0,0), weiss, weiss, true,  true,  false, false, 0},
	/*56*/{MoveField, CPoint(0,5), CPoint(0,4), CPoint(1,5), weiss, gelb,  true,  false, true,  false, 0},

	/*57*/{TargetField, CPoint(1,5), CPoint(2,5), CPoint(0,0), gelb, weiss, false, false, false, false, 0},
	/*58*/{TargetField, CPoint(2,5), CPoint(3,5), CPoint(0,0), gelb, weiss, false, false, false, false, 1},
	/*59*/{TargetField, CPoint(3,5), CPoint(4,5), CPoint(0,0), gelb, weiss, false, false, false, false, 2},
	/*60*/{TargetField, CPoint(4,5), CPoint(4,5), CPoint(0,0), gelb, weiss, false, false, false, false, 3},

	/*61*/{TargetField, CPoint(5,1), CPoint(5,2), CPoint(0,0), gruen, weiss, false, false, false, false, 0},
	/*62*/{TargetField, CPoint(5,2), CPoint(5,3), CPoint(0,0), gruen, weiss, false, false, false, false, 1},
	/*63*/{TargetField, CPoint(5,3), CPoint(5,4), CPoint(0,0), gruen, weiss, false, false, false, false, 2},
	/*64*/{TargetField, CPoint(5,4), CPoint(5,4), CPoint(0,0), gruen, weiss, false, false, false, false, 3},

	/*65*/{TargetField, CPoint(9,5), CPoint(8,5), CPoint(0,0), rot, weiss, false, false, false, false, 0},
	/*66*/{TargetField, CPoint(8,5), CPoint(7,5), CPoint(0,0), rot, weiss, false, false, false, false, 1},
	/*67*/{TargetField, CPoint(7,5), CPoint(6,5), CPoint(0,0), rot, weiss, false, false, false, false, 2},
	/*68*/{TargetField, CPoint(6,5), CPoint(6,5), CPoint(0,0), rot, weiss, false, false, false, false, 3},

	/*69*/{TargetField, CPoint(5,9), CPoint(5,8), CPoint(0,0),  schwarz, weiss, false, false, false, false, 0},
	/*70*/{TargetField, CPoint(5,8), CPoint(5,7), CPoint(0,0),  schwarz, weiss, false, false, false, false, 1},
	/*71*/{TargetField, CPoint(5,7), CPoint(5,6), CPoint(0,0),  schwarz, weiss, false, false, false, false, 2},
	/*72*/{TargetField, CPoint(5,6), CPoint(5,6), CPoint(0,0),  schwarz, weiss, false, false, false, false, 3},
};

// CMADNDoc

IMPLEMENT_DYNCREATE(CMADNDoc, CDocument)

BEGIN_MESSAGE_MAP(CMADNDoc, CDocument)
	ON_COMMAND_RANGE(ID_WURF_EINS, ID_WURF_SECHS, OnCommandWuerfel)
	ON_COMMAND_RANGE(ID_PLAYER_GELB, ID_PLAYER_SCHWARZ, OnCommandPlayer)
END_MESSAGE_MAP()


// CMADNDoc-Erstellung/Zerstörung

CMADNDoc::CMADNDoc()
{
	// create players
	m_Players[gelb]		= new CPlayer(this, CLR_GELB,	 gelb,	  IDB_PLAYER_YELLOW);
	m_Players[gruen]	= new CPlayer(this, CLR_GRUEN,	 gruen,   IDB_PLAYER_GREEN);
	m_Players[schwarz]	= new CPlayer(this, CLR_SCHWARZ, schwarz, IDB_PLAYER_BLACK);
	m_Players[rot]		= new CPlayer(this, CLR_ROT,	 rot,     IDB_PLAYER_RED);

	for (int i=0; i<maxDescriptions; i++)
	{
		CFieldBase*		pBaseField;
		CFieldStart*	pStartField;
		CFieldMove*		pMoveField;
		CFieldTarget*	pTargetField;
		CField*			pField;
		WORD			key;
		POINT_TO_WORD(myFieldDescriptions[i].key, key);
		switch (myFieldDescriptions[i].type)
		{
		case BaseField:
			pBaseField = new CFieldBase(myFieldDescriptions[i].color,
										myFieldDescriptions[i].next);
			pField = (CField*)pBaseField;
			break;
		case StartField:
			pStartField = new CFieldStart(	myFieldDescriptions[i].color,
											myFieldDescriptions[i].next,
											myFieldDescriptions[i].up,
											myFieldDescriptions[i].left,
											myFieldDescriptions[i].down,
											myFieldDescriptions[i].right );
			pField = (CField*)pStartField;
			break;
		case MoveField:
			pMoveField = new CFieldMove(	myFieldDescriptions[i].next,
											myFieldDescriptions[i].alt,
											myFieldDescriptions[i].clrAlt,
											myFieldDescriptions[i].up,
											myFieldDescriptions[i].left,
											myFieldDescriptions[i].down,
											myFieldDescriptions[i].right );
			pField = (CField*)pMoveField;
			break;
		case TargetField:	
			pTargetField = new CFieldTarget(myFieldDescriptions[i].color,
											myFieldDescriptions[i].next,
											myFieldDescriptions[i].targetPos );
			pField = (CField*)pTargetField;
			break;							
		default:							
			TRACE("Invalid field type\n");
		}
		myFieldMap.SetAt(key, pField);
	}

    myMoveHitStone.moveState = finished;
    myMovePlayer.moveState = finished;

	srand((UINT) time( NULL ));
	m_idxPlayer = (Color_t)(rand() % 4);
}

CMADNDoc::~CMADNDoc()
{
	CField*		pField = NULL;
	POSITION	pos;
	WORD		key;
	pos = myFieldMap.GetStartPosition();
	while (NULL != pos)
	{
		myFieldMap.GetNextAssoc(pos, key, pField);
		delete pField;
	}
	myFieldMap.RemoveAll();

	delete m_Players[0];
	delete m_Players[1];
	delete m_Players[2];
	delete m_Players[3];
}

BOOL CMADNDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetPlayersStartPosition();

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		pView->Invalidate();
		pView->UpdateWindow();
	}
	return TRUE;
}

// CMADNDoc-Serialisierung

void CMADNDoc::Serialize(CArchive& ar)
{
	for (int i=0; i<MAX_PLAYERS; i++)
	{
		m_Players[i]->Serialize(ar);
		m_Players[i]->SetStonesOnFields(&myFieldMap, this);
	}

	if (ar.IsStoring())
	{
		ar << (int)m_idxPlayer;
	}
	else
	{
		ar >> (int&)m_idxPlayer;

		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CMADNView* pView = (CMADNView*)GetNextView(pos);
			pView->Invalidate();
			pView->UpdateWindow();
			pView->SetPlayerColor(m_Players[m_idxPlayer]->GetStoneColor());
		}
	}
}


// CMADNDoc-Diagnose

#ifdef _DEBUG
void CMADNDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMADNDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMADNDoc-Befehle

void CMADNDoc::SetPlayersStartPosition()
{
    TRACE_FUNCTION; 
	// set player stones on base fields
	m_Players[gelb]->SetStoneOnField(&myFieldMap, CPoint(0,0), 0);
	m_Players[gelb]->SetStoneOnField(&myFieldMap, CPoint(0,1), 1);
	m_Players[gelb]->SetStoneOnField(&myFieldMap, CPoint(1,0), 2);
	m_Players[gelb]->SetStoneOnField(&myFieldMap, CPoint(1,1), 3);

	m_Players[gruen]->SetStoneOnField(&myFieldMap, CPoint(9,0), 0);
	m_Players[gruen]->SetStoneOnField(&myFieldMap, CPoint(10,0), 1);
	m_Players[gruen]->SetStoneOnField(&myFieldMap, CPoint(9,1), 2);
	m_Players[gruen]->SetStoneOnField(&myFieldMap, CPoint(10,1), 3);

	m_Players[schwarz]->SetStoneOnField(&myFieldMap, CPoint(0,9), 0);
	m_Players[schwarz]->SetStoneOnField(&myFieldMap, CPoint(1,9), 1);
	m_Players[schwarz]->SetStoneOnField(&myFieldMap, CPoint(0,10), 2);
	m_Players[schwarz]->SetStoneOnField(&myFieldMap, CPoint(1,10), 3);

	m_Players[rot]->SetStoneOnField(&myFieldMap, CPoint(9,9), 0);
	m_Players[rot]->SetStoneOnField(&myFieldMap, CPoint(10,9), 1);
	m_Players[rot]->SetStoneOnField(&myFieldMap, CPoint(9,10), 2);
	m_Players[rot]->SetStoneOnField(&myFieldMap, CPoint(10,10), 3);
}

COLORREF CMADNDoc::GetPlayerColor(void)
{
    TRACE_FUNCTION; 
	return m_Players[m_idxPlayer]->GetStoneColor();
}

void CMADNDoc::HitStone(Color_t clrStone)
{
    TRACE_FUNCTION; 
	m_Players[clrStone]->ExecuteStoneHit(clrStone, myMoveHitStone);
	MoveControler();
}

bool CMADNDoc::InvestigateMove(int noOfSteps)
{
    TRACE_FUNCTION; 
	if (m_Players[m_idxPlayer]->MoveStone(&myFieldMap, noOfSteps))
	{
		MoveControler();
		return true;
	}

	return false;
}

CFieldMap* CMADNDoc::GetFieldMap(void)
{
	return &myFieldMap;
}


bool CMADNDoc::MoveControler(void)
{
    TRACE_FUNCTION; 
	bool bRet = true;

    DBG_TRACE1("    moveStateIn = %d", myMovePlayer.moveState);
    DBG_TRACE1("    moveStateHit = %d", myMoveHitStone.moveState);

	// first check for hit move
	if (myMoveHitStone.moveState > finished)
	{
		switch (myMoveHitStone.moveState)
		{
		case active:
			UpdateAllViews(NULL, (LPARAM)&myMoveHitStone);
			myMoveHitStone.moveState = running;
			break;
		case running:
			myMoveHitStone.moveState = finished;
			HitStone(myMoveHitStone.fieldFrom->GetStoneColor());
			bRet = false;
			break;
		}
	}
	else
	{
		switch (myMovePlayer.moveState)
		{
		case active:
			UpdateAllViews(NULL, (LPARAM)&myMovePlayer);
			myMovePlayer.moveState = running;
			break;
		case running:
			myMovePlayer.moveState = stopped;
			FinishMove();
			break;
		case stopped:
			{
				// switch to next player
				if (myMovePlayer.noOfSteps != 6)
				{
                    int count = 0;
                    do
                    {
					    int idx = (int)m_idxPlayer;
					    idx = (idx < 3) ? ++idx : 0;
					    m_idxPlayer = (Color_t)idx;
                        count++;
                    }
                    while(PlayerHasFinished() && (count <= 4));
                    if (count > 4)
                    {
            			myMovePlayer.moveState = finished;
			            POSITION pos = GetFirstViewPosition();
			            while (pos != NULL)
			            {
			              CMADNView* pView = (CMADNView*)GetNextView(pos);
			              pView->PostMessage(WM_FINISH_GAME);
			            }   
                        bRet = false;
                        break;
                    }
				}
			}
			myMovePlayer.moveState = finished;
			POSITION pos = GetFirstViewPosition();
			while (pos != NULL)
			{
			  CMADNView* pView = (CMADNView*)GetNextView(pos);
			  pView->PostMessage(WM_NEXT_PLAYER);
			}   
			break;
		}
	}

    DBG_TRACE2("    moveStateOut = %d", myMovePlayer.moveState, bRet);

	return bRet;
}

void CMADNDoc::FinishMove(void)
{
	m_Players[m_idxPlayer]->FinishMove(myMovePlayer);
	MoveControler();
}

// Nachrichten Behandlungs Routinen
void CMADNDoc::OnCommandWuerfel(UINT id)
{
	int number = id - ID_WURF_EINS + 1;

	InvestigateMove(number);

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
	  CMADNView* pView = (CMADNView*)GetNextView(pos);
	  pView->SetPlayerColor(m_Players[m_idxPlayer]->GetStoneColor());
	}   
}

void CMADNDoc::OnCommandPlayer(UINT id)
{
	m_idxPlayer = (Color_t)(id - ID_PLAYER_GELB);

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
	  CMADNView* pView = (CMADNView*)GetNextView(pos);
	  pView->SetPlayerColor(m_Players[m_idxPlayer]->GetStoneColor());
	}   

}
CPlayer* CMADNDoc::GetPlayer(Color_t clrPlayer)
{
	return m_Players[clrPlayer];
}

bool CMADNDoc::PlayerHasFinished(void)
{
	TRACE_FUNCTION;
    return m_Players[m_idxPlayer]->areAllAtHome();
}
