#pragma once


// CMousePad

class CMousePad : public CToolBar
{
	DECLARE_DYNAMIC(CMousePad)

public:
	CMousePad();
	virtual ~CMousePad();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_pEditField;
public:
	void InitLayout(void);
	void UpdateEditField(CWnd* pWnd);
	void MousePadClicked(UINT nKeyCode);
};


