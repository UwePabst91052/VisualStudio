#include "StdAfx.h"
#include ".\undolist.h"

CUndoList::CUndoList(void)
:
 m_posCurAction(NULL)
,m_posNextAction(NULL)
,m_posMarkAction(NULL)
{
}

IMPLEMENT_SERIAL (CUndoAction, CObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL (CUndoList,   CObList, VERSIONABLE_SCHEMA | 1)

CUndoList::~CUndoList(void)
{
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CUndoAction* pAction = (CUndoAction*)GetNext(pos);
		delete pAction;
	}
	RemoveAll();
}

void CUndoList::AddNewAction(ActionType_t enumType, CPoint ptField, int nNumber, bool bMark)
{
	CUndoAction* pAction = NULL;

	if (m_posNextAction == NULL)
	{
		pAction = new CUndoAction(enumType, ptField, nNumber, bMark);
		m_posCurAction = AddTail(pAction);
	}
	else
	{
		m_posCurAction = m_posNextAction;
		pAction = (CUndoAction*)GetNext(m_posNextAction);
		pAction->SetActionType(enumType);
		pAction->SetField(ptField);
		pAction->SetNumber(nNumber);
		pAction->SetMark(bMark);
	}
}

void CUndoList::AddReplaceAction(CPoint ptField, int nNumber, int nOldNum, bool bMark)
{
	CUndoAction* pAction = NULL;

	if (m_posNextAction == NULL)
	{
		pAction = new CUndoAction(ptField, nNumber, nOldNum, bMark);
		m_posCurAction = AddTail(pAction);
	}
	else
	{
		m_posCurAction = m_posNextAction;
		pAction = (CUndoAction*)GetNext(m_posNextAction);
		pAction->SetField(ptField);
		pAction->SetNumber(nNumber);
		pAction->SetOldNumber(nOldNum);
		pAction->SetMark(bMark);
	}
}

bool CUndoList::UndoOneAction(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark)
{
	if (!m_posCurAction) return true;
	m_posNextAction = m_posMarkAction = m_posCurAction;
	CUndoAction* pAction = (CUndoAction*)GetPrev(m_posCurAction);
	pAction->GetActionType(enumType);
	pAction->GetField(ptField);
	if (enumType == ActionEnterNumber)
	{
		pAction->GetOldNumber(nNumber);
	}
	else if (enumType == ActionToggleCandidate)
	{
		pAction->GetNumber(nNumber);
	}
    bMark = pAction->IsMarked();
	return (m_posCurAction == NULL);
}

bool CUndoList::RedoOneAction(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark)
{
	if (!m_posNextAction) return true;
	m_posCurAction = m_posMarkAction = m_posNextAction;
	CUndoAction* pAction = (CUndoAction*)GetNext(m_posNextAction);
	pAction->GetActionType(enumType);
	pAction->GetField(ptField);
	pAction->GetNumber(nNumber);
    bMark = pAction->IsMarked();
	return (m_posNextAction == NULL);
}

bool CUndoList::UndoLastTrial(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark)
{
	if (!m_posCurAction) return true;
	m_posNextAction = m_posMarkAction = m_posCurAction;
	CUndoAction* pAction = (CUndoAction*)GetPrev(m_posCurAction);
	pAction->GetActionType(enumType);
	pAction->GetField(ptField);
	if (enumType == ActionEnterNumber)
	{
		pAction->GetOldNumber(nNumber);
	}
	else if (enumType == ActionToggleCandidate)
	{
		pAction->GetNumber(nNumber);
	}
    bMark = pAction->IsMarked();
	return (m_posCurAction == NULL) || bMark;
}

void CUndoList::SetNewList(void)
{
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		CUndoAction* pAction = (CUndoAction*)GetNext(pos);
		delete pAction;
	}
	RemoveAll();
	m_posCurAction = NULL;
	m_posNextAction = NULL;
}

void CUndoList::Serialize(CArchive &ar)
{
    if (ar.IsStoring())
    {
        POSITION pos = GetTailPosition();
        if (m_posCurAction != pos)
        {
            do
            {
                CUndoAction* pAction = NULL;
                pAction = (CUndoAction*)GetAt(pos);
                delete pAction;
                RemoveTail();
                pos = GetTailPosition();
            }
            while (pos != m_posCurAction);
        }
    }

    CObList::Serialize(ar);

    m_posCurAction = GetTailPosition();
    m_posNextAction = NULL;
}

bool CUndoList::SetTrialMarker(bool bMark)
{
    if (m_posMarkAction)
    {
        CUndoAction* pAction = (CUndoAction*)GetNext(m_posMarkAction);
        pAction->SetMark(bMark);
    }

    return bMark;
}
