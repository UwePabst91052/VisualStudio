#pragma once
#include "afxcoll.h"

// CUndoAction-Befehlsziel

typedef enum tagActionType
{
	ActionEnterNumber = 0,
	ActionToggleCandidate,
}
ActionType_t;

class CUndoAction : public CObject
{
    DECLARE_SERIAL(CUndoAction)
public:
    CUndoAction() 
        :
    m_ActionType(ActionEnterNumber),
    m_ptField(0,0),
    m_nNumber(0),
    m_nOldNumber(0),
    m_bMark(false) {};
	CUndoAction(ActionType_t enumType, CPoint ptField, int nNumber, bool bMark)
	{
		m_ActionType = enumType;
		m_ptField = ptField;
		m_nNumber = nNumber;
		m_nOldNumber = 0;
		m_bMark = bMark;
	}
	CUndoAction(CPoint ptField, int nNumber, int nOldNum, bool bMark)
	{
		m_ActionType = ActionEnterNumber;
		m_ptField = ptField;
		m_nNumber = nNumber;
		m_nOldNumber = nOldNum;
		m_bMark = bMark;
	}
	virtual ~CUndoAction() {}
	
    void Serialize (CArchive &ar) 
    {
        BYTE bMark;
        int  nActionType;

        CObject::Serialize(ar);

        if (ar.IsStoring())
        {
            bMark = m_bMark ? TRUE : FALSE;
            ar << (int)m_ActionType
               << m_ptField
               << m_nNumber
               << m_nOldNumber
               << bMark;
        }
        else
        {
            ar >> nActionType
               >> m_ptField
               >> m_nNumber
               >> m_nOldNumber
               >> bMark;
            m_ActionType = (ActionType_t)nActionType;
            m_bMark = bMark ? true : false;
        }
    };
	void GetField(CPoint& ptField) {ptField = m_ptField;}
	void SetField(CPoint ptField) {m_ptField = ptField;}
	void GetNumber(int& nNumber) {nNumber = m_nNumber;}
	void SetNumber(int nNumber) {m_nNumber = nNumber;}
	void GetOldNumber(int& nNumber) {nNumber = m_nOldNumber;}
	void SetOldNumber(int nNumber) {m_nOldNumber = nNumber;}
	bool IsMarked() {return m_bMark;}
	void SetMark(bool bMark) {m_bMark = bMark;}
	void SetActionType(ActionType_t enumType) {m_ActionType = enumType;}
	void GetActionType(ActionType_t& enumType) {enumType = m_ActionType;}
private:
	ActionType_t	m_ActionType;
	CPoint			m_ptField;
	int				m_nNumber;
	int				m_nOldNumber;
	bool			m_bMark;
};

class CUndoList :
	public CObList
{
    DECLARE_SERIAL(CUndoList)

public:
	bool SetTrialMarker(bool bMark);
	void Serialize(CArchive& ar);
	CUndoList(void);
	~CUndoList(void);
	void SetNewList(void);
	bool IsUndoEmpty() {return (m_posCurAction == NULL);}
	void AddNewAction(ActionType_t enumType, CPoint ptField, int nNumber, bool bMark);
	void AddReplaceAction(CPoint ptField, int nNumber, int nOldNum, bool bMark);
	bool UndoOneAction(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark);
	bool RedoOneAction(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark);
	bool UndoLastTrial(ActionType_t& enumType, CPoint& ptField, int& nNumber, bool& bMark);
private:
	// Position des aktuellen Action-Objekts
	POSITION m_posCurAction;
	// Position des Folgeobjekts
	POSITION m_posNextAction;
    // Position des aktuellen Markierungs Objekts
    POSITION m_posMarkAction;
};
