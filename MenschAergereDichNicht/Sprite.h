#pragma once

// CSprite command target

class CSprite : public CObject
{
public:
	CSprite();
	virtual ~CSprite();
private:
	HANDLE	m_hTimer;
	HANDLE	m_hThread;
	BOOL	m_bExitThread;

	CView* m_pView;

	int		m_SpritePosX;
	int		m_SpritePosY;
	int		m_SpritePosToX;
	int		m_SpritePosToY;
	int		m_SpriteWidth;
	int		m_SpriteHeight;
	double  m_deltaX;
	double  m_deltaY;
	double  m_realPosX;
	double  m_realPosY;
	CRect   m_rcSprite;
	CBitmap *m_bmpSprite;
	CBitmap *m_bmpBackground;
	BITMAP  m_bmpInfo;

	static DWORD WINAPI SpriteThread(LPVOID context);
public:
	bool CreateSprite(CView* pView);
	void DestroySprite(void);
	void SetStartPosition(CRect rcFrom, CBitmap* bmpSprite);
	void StartMove(CDC* pDC, CRect rcTo);
	void DeleteSprite(void);
	void DrawSprite(void);
private:
	bool MoveSprite(void);
	void StopMove(void);
public:

	CBitmap* GetBackgroundBitmap(void)
	{
		return m_bmpBackground;
	}

	int sign(int x)
	{
		int ret = (x < 0 ? -1 : 1);
		return ret;
	}
};


