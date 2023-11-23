// Stone.cpp : implementation file
//

#include "stdafx.h"
#include "MenschAergereDichNicht.h"
#include "Stone.h"
#include "FieldBase.h"
#include "FieldTarget.h"
#include "FieldStart.h"


// CStone

CStone::CStone()
{
}

CStone::CStone(CPoint ptHome, Color_t clrStone)
: m_ptHomePos(ptHome)
, m_ptCurrPos(ptHome)
, m_bAtHome(false)
, m_clrStone(clrStone)
{
}

CStone::~CStone()
{
}

IMPLEMENT_SERIAL(CStone, CObject, 1)

// CStone member functions
int CStone::GetTargetField(CFieldMap* pFieldMap, Move_t& move, int noOfSteps)
{
	WORD key;
	CPoint ptIterOld, ptIterNew;
	ptIterOld = ptIterNew = m_ptCurrPos;
	CField* pField = NULL;
	int stepCount = noOfSteps;

	POINT_TO_WORD(ptIterNew, key);
	pFieldMap->Lookup(key, pField);
	move.fieldFrom = pField;
	move.ptFieldFrom = m_ptCurrPos;
	// Überprüfe, ob der Stein auf einem Heimfeld sitzt
	if (pField->IsKindOf(RUNTIME_CLASS(CFieldBase)))
	{
		if (noOfSteps == 6)
		{
			ptIterNew = pField->GetNextField(m_clrStone);
			POINT_TO_WORD(ptIterNew, key);
			pFieldMap->Lookup(key, pField);
			POINT_TO_WORD(ptIterNew, key);
			pFieldMap->Lookup(key, pField);
			stepCount = 0;
		}
		else
			return 0;
	}
	while (stepCount > 0)
	{
		ptIterNew = pField->GetNextField(m_clrStone);
		POINT_TO_WORD(ptIterNew, key);
		pFieldMap->Lookup(key, pField);
		if ((pField->IsKindOf(RUNTIME_CLASS(CFieldTarget))) &&
			(pField->GetStoneColor() != undef))
		{
			// Stein wird durch eigenen Stein im Ziel behindert
			move.fieldTo = move.fieldTo;
			return 0;
		}
		if ((ptIterNew == ptIterOld) &&
			(stepCount > 1))
		{
			// Anzahl Züge ist zu gross
			move.fieldTo = move.fieldFrom;
			return 0;
		}
        ptIterOld = ptIterNew;
		stepCount--;
	}
	move.fieldTo = pField;
	move.ptFieldTo = ptIterNew;

	// Move evaluation
	int evaluation = 0;
	if (undef != pField->GetStoneColor())
	{
		if (m_clrStone == pField->GetStoneColor())
		{
			// Feld ist von eigenem Stein besetzt
			evaluation = 0;
		}
		else
		{
			// Feld ist von fremdem Stein besetzt
			evaluation = 3;
		}
	}
	else if (move.fieldFrom->IsKindOf(RUNTIME_CLASS(CFieldStart)))
	{
		// Von Feld ist ein Startfeld
		evaluation = 5;
	}
	else if (pField->IsKindOf(RUNTIME_CLASS(CFieldTarget)))
	{
		// Feld im Zielbereich
		evaluation = 4;
	}
	else if (pField->IsKindOf(RUNTIME_CLASS(CFieldStart)))
	{
		// Feld ist ein Startfeld
		evaluation = 2;
	}
	else
	{
		// Feld ist ein normales Feld
		evaluation = 1;
	}

	return evaluation;
}
void CStone::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_bAtHome
		   << (int)m_clrStone
		   << m_ptCurrPos.x
		   << m_ptCurrPos.y
		   << m_ptHomePos.x
		   << m_ptHomePos.y;
	}
	else
	{	// loading code
		ar >> m_bAtHome
		   >> (int&)m_clrStone
		   >> m_ptCurrPos.x
		   >> m_ptCurrPos.y
		   >> m_ptHomePos.x
		   >> m_ptHomePos.y;
	}
}
