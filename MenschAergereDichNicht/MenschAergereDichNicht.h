// MenschAergereDichNicht.h : Hauptheaderdatei für die MenschAergereDichNicht-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole

#define NO_OF_FIELD_DIM 11
#define WM_SPRITE_STOPPED WM_USER+11
#define WM_NEXT_PLAYER    WM_USER+12
#define WM_FINISH_GAME    WM_USER+13
#define POINT_TO_WORD(pt, wd) wd = (WORD)((pt.x << 8) | pt.y)
#define WORD_TO_POINT(wd, pt) pt.x = wd >> 8; pt.y = wd & 0x00FF;

class CField;

typedef enum FieldTypeTag
{
	BaseField = 0,
	StartField,
	MoveField,
	TargetField
} FieldType_t;

typedef enum MoveStateTag
{
	finished = 0,
	running,
	active,
	stopped,
} MoveState_t;

typedef struct MoveTag
{
	CField*		fieldFrom;
	CField*		fieldTo;
	CPoint		ptFieldTo;
	CPoint      ptFieldFrom;
	MoveState_t moveState;
	int         noOfSteps;
} Move_t;

typedef enum ArrowDirTag
{
	left = 0,
	down,
	right,
	up
} ArrowDir_t;

typedef enum ColorTag
{
	gelb = 0,
	gruen,
	rot,
	schwarz,
	weiss,
	undef
} Color_t;

typedef struct FieldDescriptionTag
{
	FieldType_t type;
	CPoint		key;
	CPoint      next;
	CPoint      alt;
	Color_t     color;
	Color_t     clrAlt;
	bool		up;
	bool		right;
	bool		down;
	bool		left;
	int			targetPos;
} FieldDescription_t;

typedef CMap<WORD, WORD&, CField*, CField*&>	CFieldMap;

// CMADNApp:
// Siehe MenschAergereDichNicht.cpp für die Implementierung dieser Klasse
//

class CMADNApp : public CWinApp
{
public:
	CMADNApp();


// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMADNApp theApp;