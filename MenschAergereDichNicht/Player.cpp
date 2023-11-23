// Player.cpp : implementation file
//

#include "stdafx.h"
#include "MenschAergereDichNicht.h"
#include "MenschAergereDichNichtDoc.h"
#include "MenschAergereDichNichtView.h"
#include "FieldBase.h"
#include "FieldTarget.h"
#include "Stone.h"
#include "Player.h"

#define MAX_STONES  4

// CPlayer

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(CMADNDoc* pDoc, COLORREF clrStone, Color_t color, UINT idBitmap)
: m_clrStone(clrStone)
, m_Color(color)
, m_idBitmap(idBitmap)
, m_docParent(pDoc)
, m_TryCount(0)
{
	m_bmpStone.LoadBitmap(idBitmap);
	m_stones[0] = NULL;
	m_stones[1] = NULL;
	m_stones[2] = NULL;
	m_stones[3] = NULL;
}

CPlayer::~CPlayer()
{
	delete m_stones[0];
	delete m_stones[1];
	delete m_stones[2];
	delete m_stones[3];
}

IMPLEMENT_SERIAL(CPlayer, CObject, 1)

// CPlayer member functions

void CPlayer::SetStoneOnField(CFieldMap* pFields, CPoint ptField, int  idStone)
{
	WORD key;
	CField* pField = NULL; 
	CPoint ptCurrPos;
    TRACE_FUNCTION; 

	if (NULL != m_stones[idStone])
	{
		// if stone is not at home,
		ptCurrPos = m_stones[idStone]->GetCurrentPos();
		if (ptCurrPos != ptField)
		{
			// remove it from current field
			POINT_TO_WORD(ptCurrPos, key);
			if (pFields->Lookup(key, pField))
			{
				pField->RemoveStone();
			}
			// set stone on base field
			POINT_TO_WORD(ptField, key);
			if (pFields->Lookup(key, pField))
			{
				pField->SetStone(&m_bmpStone, m_Color);
			}
			m_stones[idStone]->MoveToHome();
		}
	}
	else
	{
		// set new stone onto base field
		POINT_TO_WORD(ptField, key);
		if (pFields->Lookup(key, pField))
		{
			pField->SetStone(&m_bmpStone, m_Color);
		}
		m_stones[idStone] = new CStone(ptField, m_Color);
	}
}

void CPlayer::FinishMove(Move_t move)
{
    TRACE_FUNCTION; 
	// Move stone from field to field
	Color_t color = move.fieldFrom->GetStoneColor();
	CBitmap* pBmp = move.fieldFrom->RemoveStone();
	move.fieldTo->SetStone(pBmp, color);

	// update stone position
	for (int i=0; i<MAX_STONES; i++)
	{
		if (move.ptFieldFrom == m_stones[i]->GetCurrentPos())
		{
			m_stones[i]->SetNewPos(move.ptFieldTo, move.fieldTo->IsKindOf(RUNTIME_CLASS(CFieldTarget)));
		}
	}
}

bool CPlayer::MoveStone(CFieldMap* pFieldMap, int noOfSteps)
{
    TRACE_FUNCTION; 
	int idxBestMove = 0;
	int highestEval = 0;
	for (int i=0; i<MAX_STONES; i++)
	{
		// investigate for best move
		m_Evaluation[i] = m_stones[i]->GetTargetField(pFieldMap, m_Moves[i], noOfSteps);
		if (m_Evaluation[i] > highestEval)
		{
			highestEval = m_Evaluation[i];
			idxBestMove = i;
		}
	}

    DBG_TRACE2("highestEval = %d; idxBestMove = %d", highestEval, idxBestMove);

	// check if at least one move is possible 
	if (highestEval == 0) 
	{
		if (m_Moves[idxBestMove].fieldFrom->IsKindOf(RUNTIME_CLASS(CFieldBase)))
		{
			m_TryCount++;
			if (m_TryCount == 3)
			{
				Move_t moveDummy;
				moveDummy.noOfSteps = noOfSteps;
				moveDummy.moveState = stopped;
				m_docParent->SetPlayerMove(moveDummy);
				m_TryCount = 0;
				return true;
			}
			else	
			{
				return false;
			}
		}
	}

	// detect hit move
	if (highestEval == 3)
	{
		Move_t moveHit;
		ZeroMemory(&moveHit, sizeof(Move_t));

        DBG_TRACE0("detect hit move");

		// get hit move from stones
		CPlayer* pPlayer = m_docParent->GetPlayer(m_Moves[idxBestMove].fieldTo->GetStoneColor());
		pPlayer->GetHitMove(m_Moves[idxBestMove], pFieldMap, moveHit);

		// set hit move in parent document
		m_docParent->SetHitMove(moveHit);
	}

	// set normal move
	m_Moves[idxBestMove].noOfSteps = noOfSteps;
	m_Moves[idxBestMove].moveState = active;
	m_docParent->SetPlayerMove(m_Moves[idxBestMove]);

	return true;
}

void CPlayer::ExecuteStoneHit(Color_t clrStone, Move_t moveHit)
{
    TRACE_FUNCTION; 
	// move from field to field
	CBitmap* pBmp = moveHit.fieldFrom->RemoveStone();
	moveHit.fieldTo->SetStone(pBmp, clrStone);

	// update stone position
	for (int i=0; i<MAX_STONES; i++)
	{
		if (moveHit.ptFieldFrom == m_stones[i]->GetCurrentPos())
		{
			m_stones[i]->MoveToHome();
		}
	}
}

void CPlayer::GetHitMove(Move_t move, CFieldMap* pFieldMap, Move_t& moveHit)
{
    TRACE_FUNCTION; 
	// from field is to field of player
	moveHit.fieldFrom = move.fieldTo;
	moveHit.ptFieldFrom = move.ptFieldTo;

	// find the hit stone
	for (int i=0; i<MAX_STONES; i++)
	{
		if (move.ptFieldTo == m_stones[i]->GetCurrentPos())
		{
			WORD keyHome;
			// get home field of stone
			CPoint ptHome = m_stones[i]->GetHomePos();
			POINT_TO_WORD(ptHome, keyHome);
			pFieldMap->Lookup(keyHome, moveHit.fieldTo);
			moveHit.ptFieldTo = ptHome;
			break;
		}
	}

	// validate hit move
	moveHit.moveState = active;
}

void CPlayer::Serialize(CArchive& ar)
{
	m_stones[0]->Serialize(ar);
	m_stones[1]->Serialize(ar);
	m_stones[2]->Serialize(ar);
	m_stones[3]->Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_TryCount;
	}
	else
	{
		ar >> m_TryCount;
	}
}

void CPlayer::SetStonesOnFields(CFieldMap* pFieldMap, CMADNDoc* pDoc)
{
    TRACE_FUNCTION; 
	WORD key;
	CField* pField = NULL;
	for (int i=0; i<MAX_STONES; i++)
	{
		CPoint ptCurPos = m_stones[i]->GetCurrentPos();
		CPoint ptHomePos = m_stones[i]->GetHomePos();
		if (ptHomePos != ptCurPos)
		{
			POINT_TO_WORD(ptHomePos, key);
			if (pFieldMap->Lookup(key, pField))
			{
				pField->RemoveStone();
			}
		}
		POINT_TO_WORD(ptCurPos, key);
		if (pFieldMap->Lookup(key, pField))
		{
			pField->SetStone(&m_bmpStone, m_Color);
		}
	}
	m_docParent = pDoc;
}

bool CPlayer::areAllAtHome(void)
{
    bool bRet = true;
    for (int i = 0; i < MAX_STONES; i++)
    {
        bRet &= m_stones[i]->IsAtHome();
    }
    return bRet;
}
