// stdafx.cpp : Quelldatei, die nur die Standard-Includes einbindet
// MenschAergereDichNicht.pch ist der vorkompilierte Header
// stdafx.obj enthält die vorkompilierten Typinformationen

#include "stdafx.h"

TCHAR* indent_in [] = { _T("-->"), _T(" -->"), _T("  -->"), _T("   -->"), _T("    -->"), _T("     -->") };
TCHAR* indent_out [] = { _T("<--"), _T(" <--"), _T("  <--"), _T("   <--"), _T("    <--"), _T("     <--") };

int CFuncInOut::s_indent = 0;