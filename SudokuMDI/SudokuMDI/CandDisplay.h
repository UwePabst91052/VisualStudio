#pragma once


// CCandDisplay

class CCandDisplay : public CStatic
{
	DECLARE_DYNAMIC(CCandDisplay)

public:
	CCandDisplay();
	virtual ~CCandDisplay();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	int mySingleInFields;
	int mySingleOnBoard;
public:
	void SetSingleCounters(int nSingleInFields, int nSingleOnBoard);
private:
	CString myTitle1;
public:
	CString myTitle2;
private:
	COLORREF m_backColor;
public:
	void SetBackgroundColor(COLORREF backColor);
};


