#pragma once


// CScoreMultiSelect

class CScoreMultiSelect : public CButton
{
	DECLARE_DYNAMIC(CScoreMultiSelect)

public:
	CScoreMultiSelect();
	virtual ~CScoreMultiSelect();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClicked();
};


