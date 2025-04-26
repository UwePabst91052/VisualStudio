#pragma once


// CScoreSort

class CScoreSort : public CButton
{
	DECLARE_DYNAMIC(CScoreSort)

public:
	CScoreSort();
	virtual ~CScoreSort();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClicked();
};


